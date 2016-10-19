#include "wterm.h"
#include "helpers.h"
#include "wicked-def.h"

WTerm::WTerm()
    : operators_(std::vector<std::vector<WSQOperator>>(osi->num_spaces())) {}

void WTerm::add(const WSQOperator &op) {
  operators_[op.index().space()].push_back(op);
}

void WTerm::add(const WTensor &tensor) { tensors_.push_back(tensor); }

scalar_t WTerm::factor() const { return factor_; }

void WTerm::set_factor(scalar_t value) { factor_ = value; }

int WTerm::noperators() const {
  int result = 0;
  for (const auto &op_space : operators_) {
    result += op_space.size();
  }
  return result;
}

std::vector<int> WTerm::noperators_per_space() const {
  std::vector<int> counter;
  for (const auto &op : operators_) {
    counter.push_back(op.size());
  }
  return counter;
}

std::vector<WIndex> WTerm::indices() const {
  std::vector<WIndex> result;
  for (const auto &t : tensors_) {
    const auto t_idx = t.indices();
    result.insert(result.end(), t_idx.begin(), t_idx.end());
  }
  for (const auto &ops : operators_) {
    for (const auto &op : ops) {
      result.push_back(op.index());
    }
  }
  return result;
}

void WTerm::reindex(index_map_t &idx_map) {
  for (auto &t : tensors_) {
    t.reindex(idx_map);
  }
  for (auto &ops : operators_) {
    for (auto &op : ops) {
      op.reindex(idx_map);
    }
  }
}

void WTerm::canonicalize() {
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

  // 3. Sort operators according to canonical form
}

bool WTerm::operator<(const WTerm &other) const {
  if (tensors_ > other.tensors_) {
    return false;
  }
  if (tensors_ < other.tensors_) {
    return true;
  }
  return operators_ < other.operators_;
}

bool WTerm::operator==(const WTerm &other) const {
  return (tensors_ == other.tensors_) and (operators_ == other.operators_);
}

std::string WTerm::str() const {
  std::vector<std::string> str_vec;

  str_vec.push_back("hello");

  str_vec.push_back(to_string(factor_));
  for (const WTensor &tensor : tensors_) {
    str_vec.push_back(tensor.str());
  }

  if (noperators() > 0) {
    str_vec.push_back("{");
    for (const auto &op_space : operators_) {
      for (const WSQOperator &q : op_space) {
        str_vec.push_back(q.str());
      }
    }
    str_vec.push_back("}");
  }

  return (to_string(str_vec, " "));
}

std::string WTerm::tensor_str() const {
  std::vector<std::string> str_vec;
  for (const WTensor &tensor : tensors_) {
    str_vec.push_back(tensor.str());
  }
  return (to_string(str_vec, " "));
}

std::string WTerm::operator_str() const {
  std::vector<std::string> str_vec;

  if (noperators() > 0) {
    str_vec.push_back("{");
    for (const auto &op_space : operators_) {
      for (const WSQOperator &q : op_space) {
        str_vec.push_back(q.str());
      }
    }
    str_vec.push_back("}");
  }
  return (to_string(str_vec, " "));
}

std::string WTerm::latex() const {
  std::vector<std::string> str_vec;
  for (const WTensor &tensor : tensors_) {
    str_vec.push_back(tensor.latex());
  }

  if (noperators() > 0) {
    str_vec.push_back("\\{");
    for (const auto &op_space : operators_) {
      for (const WSQOperator &q : op_space) {
        str_vec.push_back(q.latex());
      }
    }
    str_vec.push_back("\\}");
  }

  return (to_string(str_vec, " "));
}

std::ostream &operator<<(std::ostream &os, const WTerm &term) {
  os << term.str();
  return os;
}

WTerm make_operator(const std::string &label,
                    const std::vector<std::string> &cre,
                    const std::vector<std::string> &ann) {
  WTerm term;

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
