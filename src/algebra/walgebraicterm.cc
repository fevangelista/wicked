#include <algorithm>

#include "wicked-def.h"
#include "helpers.h"
#include "walgebraicterm.h"

WAlgebraicTerm::WAlgebraicTerm() {}

void WAlgebraicTerm::add(const WSQOperator &op) { operators_.push_back(op); }

void WAlgebraicTerm::add(const WTensor &tensor) { tensors_.push_back(tensor); }

scalar_t WAlgebraicTerm::factor() const { return factor_; }

void WAlgebraicTerm::set_factor(scalar_t value) { factor_ = value; }

int WAlgebraicTerm::nops() const { return operators_.size(); }

std::vector<WIndex> WAlgebraicTerm::indices() const {
  std::vector<WIndex> result;
  for (const auto &t : tensors_) {
    const auto t_idx = t.indices();
    result.insert(result.end(), t_idx.begin(), t_idx.end());
  }
  for (const auto &op : operators_) {
    result.push_back(op.index());
  }
  return result;
}

void WAlgebraicTerm::reindex(index_map_t &idx_map) {
  for (auto &t : tensors_) {
    t.reindex(idx_map);
  }
  for (auto &op : operators_) {
    op.reindex(idx_map);
  }
}

void WAlgebraicTerm::canonicalize() {
  // 1. Sort the tensors according to a score function
  using score_t =
      std::tuple<std::string, int, std::vector<int>, std::vector<int>, WTensor>;
  std::vector<score_t> scores;

  int n = 0;
  for (const auto &tensor : tensors_) {
    // a) label
    const std::string &label = tensor.label();

    // b) rank
    int rank = tensor.rank();

    // c) number of indices per space
    std::vector<int> num_low = num_indices_per_space(tensor.lower());
    std::vector<int> num_upp = num_indices_per_space(tensor.upper());

    scores.push_back(std::make_tuple(label, rank, num_low, num_upp, tensor));
    n += 1;
  }
  std::sort(scores.begin(), scores.end());

  tensors_.clear();
  for (const auto &score : scores) {
    tensors_.push_back(std::get<4>(score));
  }

  // 2. Relabel indices of tensors and operators

  std::vector<int> sqop_index_count(osi->num_spaces(), 0);
  std::vector<int> tens_index_count(osi->num_spaces(), 0);
  index_map_t index_map;
  std::map<WIndex, bool> is_operator_index;

  // a. Assign indices to free operators
  for (const auto &sqop : operators_) {
    tens_index_count[sqop.index().space()] += 1;
    is_operator_index[sqop.index()] = true;
  }

  // b. Assign indices to tensors operators
  for (const auto &tensor : tensors_) {
    for (const auto &l : tensor.lower()) {
      if (is_operator_index.count(l) == 0) {
        // this index is not shared with an operator
        if (index_map.count(l) == 0) {
          int s = l.space();
          index_map[l] = WIndex(s, tens_index_count[s]);
          tens_index_count[s] += 1;
        }
      } else {
        // this index is shared with an operator
        if (index_map.count(l) == 0) {
          int s = l.space();
          index_map[l] = WIndex(s, sqop_index_count[s]);
          sqop_index_count[s] += 1;
        }
      }
    }
    for (const auto &u : tensor.upper()) {
      if (is_operator_index.count(u) == 0) {
        if (index_map.count(u) == 0) {
          int s = u.space();
          index_map[u] = WIndex(s, tens_index_count[s]);
          tens_index_count[s] += 1;
        }
      } else {
        if (index_map.count(u) == 0) {
          int s = u.space();
          index_map[u] = WIndex(s, sqop_index_count[s]);
          sqop_index_count[s] += 1;
        }
      }
    }
  }

  reindex(index_map);

  // 3. Sort tensor indices according to canonical form
  for (auto &tensor : tensors_) {
    scalar_t sign = 1;
    {
      std::vector<std::tuple<int, int, int, WIndex>> upper_sort;
      int upos = 0;
      for (const auto &index : tensor.upper()) {
        upper_sort.push_back(
            std::make_tuple(index.space(), index.index(), upos, index));
        upos += 1;
      }
      std::sort(upper_sort.begin(), upper_sort.end());

      std::vector<int> usign;
      std::vector<WIndex> new_upper;
      for (const auto &tpl : upper_sort) {
        usign.push_back(std::get<2>(tpl));
        new_upper.push_back(std::get<3>(tpl));
      }
      tensor.set_upper(new_upper);
      sign *= permutation_sign(usign);
    }

    {
      std::vector<std::tuple<int, int, int, WIndex>> lower_sort;
      int lpos = 0;
      for (const auto &index : tensor.lower()) {
        lower_sort.push_back(
            std::make_tuple(index.space(), index.index(), lpos, index));
        lpos += 1;
      }
      std::sort(lower_sort.begin(), lower_sort.end());

      std::vector<int> lsign;
      std::vector<WIndex> new_lower;
      for (const auto &tpl : lower_sort) {
        lsign.push_back(std::get<2>(tpl));
        new_lower.push_back(std::get<3>(tpl));
      }
      tensor.set_lower(new_lower);
      sign *= permutation_sign(lsign);
    }
    factor_ *= sign;
  }

  // 4. Sort operators according to canonical form
  std::vector<int> sqops_pos(operators_.size(), -1);
  std::vector<std::tuple<int, int, int, int>> sorting_vec;
  int pos = 0;
  for (const auto &sqop : operators_) {
    int type = (sqop.type() == Creation) ? 0 : 1;
    int s = sqop.index().space();
    int index = sqop.index().index();
    sorting_vec.push_back(std::make_tuple(type, s, index, pos));
    pos += 1;
  }

  std::sort(sorting_vec.begin(), sorting_vec.end());

  std::vector<int> sign_order;
  std::vector<WSQOperator> new_sqops;
  for (const auto &tpl : sorting_vec) {
    int idx = std::get<3>(tpl);
    sign_order.push_back(idx);
    new_sqops.push_back(operators_[idx]);
  }
  factor_ *= permutation_sign(sign_order);
  operators_ = new_sqops;
}

bool WAlgebraicTerm::operator<(const WAlgebraicTerm &other) const {
  if (tensors_ > other.tensors_) {
    return false;
  }
  if (tensors_ < other.tensors_) {
    return true;
  }
  return operators_ < other.operators_;
}

bool WAlgebraicTerm::operator==(const WAlgebraicTerm &other) const {
  return (tensors_ == other.tensors_) and (operators_ == other.operators_);
}

std::string WAlgebraicTerm::str() const {
  std::vector<std::string> str_vec;

  str_vec.push_back(to_string(factor_));
  for (const WTensor &tensor : tensors_) {
    str_vec.push_back(tensor.str());
  }
  if (operators_.size()) {
    str_vec.push_back("{");
    for (const auto &op : operators_) {
      str_vec.push_back(op.str());
    }
    str_vec.push_back("}");
  }

  return (to_string(str_vec, " "));
}

std::string WAlgebraicTerm::tensor_str() const {
  std::vector<std::string> str_vec;
  for (const WTensor &tensor : tensors_) {
    str_vec.push_back(tensor.str());
  }
  return (to_string(str_vec, " "));
}

std::string WAlgebraicTerm::operator_str() const {
  std::vector<std::string> str_vec;
  str_vec.push_back("{");
  for (const auto &op : operators_) {
    str_vec.push_back(op.str());
  }
  str_vec.push_back("}");
  return (to_string(str_vec, " "));
}

std::string WAlgebraicTerm::latex() const {
  std::vector<std::string> str_vec;
  for (const WTensor &tensor : tensors_) {
    str_vec.push_back(tensor.latex());
  }
  str_vec.push_back("\\{");
  for (const auto &op : operators_) {
    str_vec.push_back(op.latex());
  }
  str_vec.push_back("\\}");
  return (to_string(str_vec, " "));
}

std::ostream &operator<<(std::ostream &os, const WAlgebraicTerm &term) {
  os << term.str();
  return os;
}

WAlgebraicTerm make_algebraic_term(const std::string &label,
                                   const std::vector<std::string> &cre,
                                   const std::vector<std::string> &ann) {
  WAlgebraicTerm term;

  auto indices = make_indices_from_space_labels({cre, ann});
  std::vector<WIndex> &cre_ind = indices[0];
  std::vector<WIndex> &ann_ind = indices[1];

  // Add the tensor
  WTensor tensor(label, cre_ind, ann_ind);
  term.add(tensor);

  // Add the creation operators
  for (const WIndex c : cre_ind) {
    WSQOperator sqop(Creation, c);
    term.add(sqop);
  }

  // Add the annihilation operators
  std::reverse(ann_ind.begin(), ann_ind.end()); // reverse the annihilation ops
  for (const WIndex a : ann_ind) {
    WSQOperator sqop(Annihilation, a);
    term.add(sqop);
  }
  return term;
}
