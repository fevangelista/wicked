#include <algorithm>

#include "stl_utils.hpp"

#include "wicked-def.h"
#include "helpers.h"
#include "orbital_space.h"
#include "combinatorics.h"
#include "windex.h"
#include "wsqoperator.h"
#include "wtensor.h"
#include "walgebraicterm.h"

using namespace std;

WAlgebraicTerm::WAlgebraicTerm() {}

void WAlgebraicTerm::add(const WSQOperator &op) { operators_.push_back(op); }

void WAlgebraicTerm::add(const WTensor &tensor) { tensors_.push_back(tensor); }

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

std::vector<std::pair<std::string, std::vector<int>>>
WAlgebraicTerm::tensor_connectivity(const WTensor &t, bool upper) const {
  std::vector<std::pair<std::string, std::vector<int>>> result;
  auto indices = upper ? t.upper() : t.lower();
  sort(indices.begin(), indices.end());
  for (const auto &tensor : tensors_) {
    if (not(t == tensor)) {

      std::vector<WIndex> indices2 = upper ? tensor.lower() : tensor.upper();
      sort(indices2.begin(), indices2.end());

      std::vector<WIndex> indices3 = tensor.upper();
      sort(indices3.begin(), indices3.end());

      std::vector<WIndex> common_lower_indices;
      set_intersection(indices.begin(), indices.end(), indices2.begin(),
                       indices2.end(), back_inserter(common_lower_indices));

      std::vector<WIndex> common_upper_indices;
      set_intersection(indices.begin(), indices.end(), indices3.begin(),
                       indices3.end(), back_inserter(common_upper_indices));

      result.push_back(std::make_pair(
          tensor.label(), num_indices_per_space(common_lower_indices)));
      //      result.push_back(std::make_pair(
      //          "u" + tensor.label(),
      //          num_indices_per_space(common_upper_indices)));
    }
  }
  std::sort(result.begin(), result.end());
  return result;
}

#define NEW_CANONICALIZATION 1
scalar_t WAlgebraicTerm::canonicalize() {
  scalar_t factor(1);

// 1. Sort the tensors according to a score function
#if NEW_CANONICALIZATION
  using score_t =
      std::tuple<std::string, int, std::vector<int>, std::vector<int>,
                 std::vector<std::pair<std::string, std::vector<int>>>,
                 std::vector<std::pair<std::string, std::vector<int>>>,
                 WTensor>;
#else
  using score_t =
      std::tuple<std::string, int, std::vector<int>, std::vector<int>, WTensor>;
#endif

  std::vector<score_t> scores;

  //  std::cout << "\n Canonicalizing: " << str() << std::endl;

  int n = 0;
  for (const auto &tensor : tensors_) {
    // a) label
    const std::string &label = tensor.label();

    // b) rank
    int rank = tensor.rank();

    // c) number of indices per space
    std::vector<int> num_low = num_indices_per_space(tensor.lower());
    std::vector<int> num_upp = num_indices_per_space(tensor.upper());

    // d) connectivity of lower indices
    auto lower_conn = tensor_connectivity(tensor, false);

    // d) connectivity of upper indices
    auto upper_conn = tensor_connectivity(tensor, true);

// e) store the score
#if NEW_CANONICALIZATION
    scores.push_back(std::make_tuple(label, rank, num_low, num_upp, lower_conn,
                                     upper_conn, tensor));
//    std::cout << "\nScore = " << label << " " << rank << " ";
//    PRINT_ELEMENTS(num_low);
//    std::cout << " ";
//    PRINT_ELEMENTS(num_upp);
//    std::cout << " ";
//    for (const auto &str_vec : lower_conn) {
//      std::cout << str_vec.first << " ";
//      PRINT_ELEMENTS(str_vec.second);
//    }
//    for (const auto &str_vec : upper_conn) {
//      std::cout << " " << str_vec.first << " ";
//      PRINT_ELEMENTS(str_vec.second);
//    }

#else
    scores.push_back(std::make_tuple(label, rank, num_low, num_upp, tensor));
    n += 1;
#endif
  }

  // sort and rearrange tensors
  std::sort(scores.begin(), scores.end());
  tensors_.clear();
  for (const auto &score : scores) {
#if NEW_CANONICALIZATION
    tensors_.push_back(std::get<6>(score));
#else
    tensors_.push_back(std::get<4>(score));
#endif
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
    // lower indices
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
    // upper indices
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
    factor *= sign;
  }

  // 4. Sort operators according to canonical form
  std::vector<int> sqops_pos(operators_.size(), -1);
  std::vector<std::tuple<int, int, int, int>> sorting_vec;
  int pos = 0;
  for (const auto &sqop : operators_) {
    int type = (sqop.type() == Creation) ? 0 : 1;
    int s = sqop.index().space();
    // Annihilation operators are written in reverse order
    int index = (sqop.type() == Creation) ? sqop.index().index()
                                          : -sqop.index().index();
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
  factor *= permutation_sign(sign_order);
  operators_ = new_sqops;

  //  std::cout << "\n  " << str();

//  canonicalize_best();

  return factor;
}

scalar_t WAlgebraicTerm::canonicalize_tensor_indices() {
  scalar_t sign(1);
  // Sort tensor indices according to canonical form
  for (auto &tensor : tensors_) {
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
  }
  return sign;
}

scalar_t WAlgebraicTerm::canonicalize_best() {
  scalar_t factor(1);

  // find classes of equivalent indices
  std::map<std::pair<std::string, int>, std::vector<WIndex>> equiv_classes;
  for (const auto &tensor : tensors_) {
    std::string label = tensor.label();
    for (int i : num_indices_per_space(tensor.upper())) {
      label += "_" + to_string(i);
    }
    for (int i : num_indices_per_space(tensor.lower())) {
      label += "_" + to_string(i);
    }
    for (const auto &idx : tensor.upper()) {
      auto fingerprint = std::make_tuple(label + "u", idx.space());
      equiv_classes[fingerprint].push_back(idx);
    }
    for (const auto &idx : tensor.lower()) {
      auto fingerprint = std::make_tuple(label + "l", idx.space());
      equiv_classes[fingerprint].push_back(idx);
    }
  }

  cout << str() << '\n';
  cout << "Index equivalence classes:\n";
  for (const auto &kv : equiv_classes) {
    cout << kv.first.first << " " << kv.first.second << " ";
    PRINT_ELEMENTS(kv.second);
    cout << '\n';
  }

  std::set<WIndex> sqops_indices;
  for (const auto &sqop : operators_) {
    sqops_indices.insert(sqop.index());
  }

  // find the unique classes
  std::set<std::vector<WIndex>> unique_equiv_classes;
  for (const auto &kv : equiv_classes) {
    // must have at least two elements (otherwise there is no need to relabel)
    if (kv.second.size() > 1) {
      bool is_op_index = false;
      for (const auto &idx : kv.second) {
        if (sqops_indices.count(idx) != 0)
          is_op_index = true;
      }
      if (not is_op_index) {
        std::vector<WIndex> indices = kv.second;
        std::sort(indices.begin(), indices.end());
        unique_equiv_classes.insert(indices);
      }
    }
  }

  int numeqcl = unique_equiv_classes.size();
  cout << "Index unique equivalence classes:\n";
  for (const auto &v : unique_equiv_classes) {
    PRINT_ELEMENTS(v);
    cout << '\n';
  }

//  // for each equivalence class holds all permutations of indices
//  std::vector<std::vector<std::vector<WIndex>>>
//      unique_equiv_classes_permutations;

//  std::vector<int> r;
//  for (const auto &v : unique_equiv_classes) {
//    std::vector<std::vector<WIndex>> permutations;
//    do {
//      permutations.push_back(v);
//    } while (std::next_permutation(v.begin(), v.end()));
//    r.push_back(permutations.size());
//    unique_equiv_classes_permutations.push_back(permutations);
//  }

//  auto prod_space = product_space(r);

//  for (const auto &el : prod_space) {
//    cout << "Considering the following permutations" << '\n';
//    for (int n = 0; n < numeqcl; n++) {
//        PRINT_ELEMENTS(unique_equiv_classes_permutations[n][0]);
//        cout << " -> ";
//        PRINT_ELEMENTS(unique_equiv_classes_permutations[n][el[n]]);
//        cout << '\n';
//    }
//  }

  //

  // loop over direct product of unique equivalence classes and generate
  // permutations of indices

  // resort indices in the tensors
  // find "best" representation

  return factor;
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

std::string WAlgebraicTerm::ambit() const {
  std::vector<std::string> str_vec;
  for (const WTensor &tensor : tensors_) {
    str_vec.push_back(tensor.ambit());
  }
  if (operators_.size()) {
    throw "Trying to convert an WAlgebraicTerm object with operator terms to "
          "ambit.";
  }
  return (to_string(str_vec, " * "));
}

std::ostream &operator<<(std::ostream &os, const WAlgebraicTerm &term) {
  os << term.str();
  return os;
}

std::ostream &
operator<<(std::ostream &os,
           const std::pair<WAlgebraicTerm, scalar_t> &term_factor) {
  os << term_factor.second << ' ' << term_factor.second;
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
