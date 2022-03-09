#include <algorithm>

#include "helpers/combinatorics.h"
#include "helpers/helpers.h"
#include "helpers/orbital_space.h"
#include "helpers/stl_utils.hpp"

#include "term.h"

using namespace std;

SymbolicTerm::SymbolicTerm() {}

SymbolicTerm::SymbolicTerm(bool normal_ordered,
                           const std::vector<SQOperator> &operators,
                           const std::vector<Tensor> &tensors)
    : normal_ordered_(normal_ordered), operators_(operators),
      tensors_(tensors) {}

void SymbolicTerm::set_normal_ordered(bool val) { normal_ordered_ = val; }

void SymbolicTerm::set(const std::vector<SQOperator> &op) { operators_ = op; }

void SymbolicTerm::add(const SQOperator &op) { operators_.push_back(op); }

void SymbolicTerm::add(const std::vector<SQOperator> &ops) {
  for (const auto &op : ops) {
    add(op);
  }
}

void SymbolicTerm::add(const Tensor &tensor) { tensors_.push_back(tensor); }

int SymbolicTerm::nops() const { return operators_.size(); }

std::vector<Index> SymbolicTerm::indices() const {
  std::vector<Index> result;
  for (const auto &t : tensors_) {
    const auto t_idx = t.indices();
    result.insert(result.end(), t_idx.begin(), t_idx.end());
  }
  for (const auto &op : operators_) {
    result.push_back(op.index());
  }
  return result;
}

void SymbolicTerm::reindex(index_map_t &idx_map) {
  for (auto &t : tensors_) {
    t.reindex(idx_map);
  }
  for (auto &op : operators_) {
    op.reindex(idx_map);
  }
}

#define NEW_CANONICALIZATION 1
scalar_t SymbolicTerm::canonicalize() {
  scalar_t factor(1);

//
// 1. Sort the tensors according to a score function
//
#if NEW_CANONICALIZATION
  using score_t =
      std::tuple<std::string, int, std::vector<int>, std::vector<int>,
                 std::vector<std::pair<std::string, std::vector<int>>>,
                 std::vector<std::pair<std::string, std::vector<int>>>, Tensor>;
#else
  using score_t =
      std::tuple<std::string, int, std::vector<int>, std::vector<int>, Tensor>;
#endif

  std::vector<score_t> scores;

  WPRINT(std::cout << "\n Canonicalizing: " << str() << std::endl;);

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
    WPRINT(
        std::cout << "\nScore = " << label << " " << rank << " ";
        PRINT_ELEMENTS(num_low); std::cout << " "; PRINT_ELEMENTS(num_upp);
        std::cout << " "; for (const auto &str_vec
                               : lower_conn) {
          std::cout << str_vec.first << " ";
          PRINT_ELEMENTS(str_vec.second);
        } for (const auto &str_vec
               : upper_conn) {
          std::cout << " " << str_vec.first << " ";
          PRINT_ELEMENTS(str_vec.second);
        });

#else
    scores.push_back(std::make_tuple(label, rank, num_low, num_upp, tensor));
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
  // vector to keep track of how many indices in each space
  std::vector<int> sqop_index_count(osi->num_spaces(), 0);
  // vector to keep track of how many indices in each space
  std::vector<int> tens_index_count(osi->num_spaces(), 0);
  index_map_t index_map;
  std::map<Index, bool> is_operator_index;

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
          index_map[l] = Index(s, tens_index_count[s]);
          tens_index_count[s] += 1;
        }
      } else {
        // this index is shared with an operator
        if (index_map.count(l) == 0) {
          int s = l.space();
          index_map[l] = Index(s, sqop_index_count[s]);
          sqop_index_count[s] += 1;
        }
      }
    }
    // upper indices
    for (const auto &u : tensor.upper()) {
      if (is_operator_index.count(u) == 0) {
        if (index_map.count(u) == 0) {
          int s = u.space();
          index_map[u] = Index(s, tens_index_count[s]);
          tens_index_count[s] += 1;
        }
      } else {
        if (index_map.count(u) == 0) {
          int s = u.space();
          index_map[u] = Index(s, sqop_index_count[s]);
          sqop_index_count[s] += 1;
        }
      }
    }
  }
  // reindex the tensor
  reindex(index_map);

  // 3. Sort tensor indices according to canonical form
  for (auto &tensor : tensors_) {
    factor *= tensor.canonicalize();
  }

  // 4. Sort operators according to canonical form
  factor *= canonicalize_sqops(operators_, false);

  simplify();

  WPRINT(std::cout << "\n  " << str();)

  return factor;
}

scalar_t SymbolicTerm::simplify() {
  WPRINT(cout << "\nSymbolic term simplification " << endl;);
  scalar_t factor = 1;
  // Canonicalize each space separately
  for (int s = 0; s < osi->num_spaces(); s++) {
    WPRINT(cout << "\nSpace " << osi->label(s) << endl;);

    std::vector<std::vector<Index>> equivalent_classes;
    std::vector<std::pair<std::bitset<64>, std::bitset<64>>> ul_bit_masks;
    for (const auto &tensor : tensors_) {
      std::bitset<64> upper_bits, lower_bits;
      {
        std::vector<Index> equivalent;
        for (const auto &u : tensor.upper()) {
          if (u.space() == s) {
            equivalent.push_back(u);
            upper_bits[u.pos()] = true;
          }
        }
        if (equivalent.size() > 0)
          equivalent_classes.push_back(equivalent);
      }
      {
        std::vector<Index> equivalent;
        for (const auto &l : tensor.lower()) {
          if (l.space() == s) {
            equivalent.push_back(l);
            lower_bits[l.pos()] = true;
          }
        }
        if (equivalent.size() > 0)
          equivalent_classes.push_back(equivalent);
      }
      ul_bit_masks.push_back(std::make_pair(upper_bits, lower_bits));
    }
    WPRINT(
        for (const auto &v
             : equivalent_classes) {
          PRINT_ELEMENTS(v);
          cout << endl;
        } for (const auto &[ub, lb]
               : ul_bit_masks) {
          cout << ub << endl;
          cout << lb << endl;
        });
  }

  return factor;
}

bool SymbolicTerm::operator<(const SymbolicTerm &other) const {
  if (tensors_ > other.tensors_) {
    return false;
  }
  if (tensors_ < other.tensors_) {
    return true;
  }
  return operators_ < other.operators_;
}

bool SymbolicTerm::operator==(const SymbolicTerm &other) const {
  return (tensors_ == other.tensors_) and (operators_ == other.operators_);
}

std::string SymbolicTerm::str() const {
  std::vector<std::string> str_vec;
  for (const Tensor &tensor : tensors_) {
    str_vec.push_back(tensor.str());
  }
  if (nops()) {
    if (normal_ordered())
      str_vec.push_back("{");
    for (const auto &op : operators_) {
      str_vec.push_back(op.str());
    }
    if (normal_ordered())
      str_vec.push_back("}");
  }

  return (join(str_vec, " "));
}

std::string SymbolicTerm::tensor_str() const {
  std::vector<std::string> str_vec;
  for (const Tensor &tensor : tensors_) {
    str_vec.push_back(tensor.str());
  }
  return (join(str_vec, " "));
}

std::string SymbolicTerm::operator_str() const {
  std::vector<std::string> str_vec;
  str_vec.push_back("{");
  for (const auto &op : operators_) {
    str_vec.push_back(op.str());
  }
  str_vec.push_back("}");
  return (join(str_vec, " "));
}

std::string SymbolicTerm::latex() const {
  std::vector<std::string> str_vec;
  for (const Tensor &tensor : tensors_) {
    str_vec.push_back(tensor.latex());
  }
  if (nops()) {
    if (normal_ordered())
      str_vec.push_back("\\{");
    for (const auto &op : operators_) {
      str_vec.push_back(op.latex());
    }
    if (normal_ordered())
      str_vec.push_back("\\}");
  }
  return (join(str_vec, " "));
}

std::string SymbolicTerm::compile(const std::string &format) const {
  if (format == "ambit") {
    std::vector<std::string> str_vec;
    for (const Tensor &tensor : tensors_) {
      str_vec.push_back(tensor.compile(format));
    }
    if (nops()) {
      throw "Trying to convert an SymbolicTerm object with operator terms to "
            "ambit.";
    }
    return (join(str_vec, " * "));
  }
  return "";
}

std::ostream &operator<<(std::ostream &os, const SymbolicTerm &term) {
  os << term.str();
  return os;
}

std::ostream &operator<<(std::ostream &os,
                         const std::pair<SymbolicTerm, scalar_t> &term_factor) {
  os << term_factor.second << ' ' << term_factor.second;
  return os;
}

std::vector<std::pair<std::string, std::vector<int>>>
SymbolicTerm::tensor_connectivity(const Tensor &t, bool upper) const {
  std::vector<std::pair<std::string, std::vector<int>>> result;
  auto indices = upper ? t.upper() : t.lower();
  sort(indices.begin(), indices.end());
  for (const auto &tensor : tensors_) {
    if (not(t == tensor)) {

      std::vector<Index> indices2 = upper ? tensor.lower() : tensor.upper();
      sort(indices2.begin(), indices2.end());

      std::vector<Index> indices3 = tensor.upper();
      sort(indices3.begin(), indices3.end());

      std::vector<Index> common_lower_indices;
      set_intersection(indices.begin(), indices.end(), indices2.begin(),
                       indices2.end(), back_inserter(common_lower_indices));

      std::vector<Index> common_upper_indices;
      set_intersection(indices.begin(), indices.end(), indices3.begin(),
                       indices3.end(), back_inserter(common_upper_indices));

      result.push_back(std::make_pair(
          tensor.label(), num_indices_per_space(common_lower_indices)));
    }
  }
  std::sort(result.begin(), result.end());
  return result;
}

// SymbolicTerm make_algebraic_term(const std::string &label,
//                                  const std::vector<char> &cre,
//                                  const std::vector<char> &ann) {
//   SymbolicTerm term;

//   auto indices = make_indices_from_space_labels({cre, ann});
//   std::vector<Index> &cre_ind = indices[0];
//   std::vector<Index> &ann_ind = indices[1];

//   // Add the tensor
//   Tensor tensor(label, cre_ind, ann_ind);
//   term.add(tensor);

//   // Add the creation operators
//   for (const Index &c : cre_ind) {
//     SQOperator sqop(SQOperatorType::Creation, c);
//     term.add(sqop);
//   }

//   // Add the annihilation operators
//   std::reverse(ann_ind.begin(), ann_ind.end()); // reverse the annihilation
//   ops for (const Index &a : ann_ind) {
//     SQOperator sqop(SQOperatorType::Annihilation, a);
//     term.add(sqop);
//   }
//   return term;
// }

// scalar_t SymbolicTerm::canonicalize_best() {
//  scalar_t factor(1);

//  // find classes of equivalent indices
//  std::map<std::pair<std::string, int>, std::vector<Index>> equiv_classes;
//  for (const auto &tensor : tensors_) {
//    std::string label = tensor.label();
//    for (int i : num_indices_per_space(tensor.upper())) {
//      label += "_" + to_string(i);
//    }
//    for (int i : num_indices_per_space(tensor.lower())) {
//      label += "_" + to_string(i);
//    }
//    for (const auto &idx : tensor.upper()) {
//      auto fingerprint = std::make_tuple(label + "u", idx.space());
//      equiv_classes[fingerprint].push_back(idx);
//    }
//    for (const auto &idx : tensor.lower()) {
//      auto fingerprint = std::make_tuple(label + "l", idx.space());
//      equiv_classes[fingerprint].push_back(idx);
//    }
//  }

//  cout << str() << '\n';
//  cout << "Index equivalence classes:\n";
//  for (const auto &kv : equiv_classes) {
//    cout << kv.first.first << " " << kv.first.second << " ";
//    PRINT_ELEMENTS(kv.second);
//    cout << '\n';
//  }

//  std::set<Index> sqops_indices;
//  for (const auto &sqop : operators_) {
//    sqops_indices.insert(sqop.index());
//  }

//  // find the unique classes
//  std::set<std::vector<Index>> unique_equiv_classes;
//  for (const auto &kv : equiv_classes) {
//    // must have at least two elements (otherwise there is no need to relabel)
//    if (kv.second.size() > 1) {
//      bool is_op_index = false;
//      for (const auto &idx : kv.second) {
//        if (sqops_indices.count(idx) != 0)
//          is_op_index = true;
//      }
//      if (not is_op_index) {
//        std::vector<Index> indices = kv.second;
//        std::sort(indices.begin(), indices.end());
//        unique_equiv_classes.insert(indices);
//      }
//    }
//  }

//  int numeqcl = unique_equiv_classes.size();
//  cout << "Index unique equivalence classes:\n";
//  for (const auto &v : unique_equiv_classes) {
//    PRINT_ELEMENTS(v);
//    cout << '\n';
//  }

//  // for each equivalence class holds all permutations of indices
//  std::vector<std::vector<std::vector<Index>>>
//      unique_equiv_classes_permutations;

//  std::vector<int> r;
//  for (const auto &v : unique_equiv_classes) {
//    std::vector<std::vector<Index>> permutations;
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

//  return factor;
//}

// std::vector<int> sqops_pos(nops(), -1);
// std::vector<std::tuple<int, int, int, int>> sorting_vec;
// int pos = 0;
// for (const auto &sqop : operators_) {
//   int type = (sqop.type() == SQOperatorType::Creation) ? 0 : 1;
//   int s = sqop.index().space();
//   // Annihilation operators are written in reverse order
//   int index = (sqop.type() == SQOperatorType::Creation) ?
//   sqop.index().pos()
//                                                         :
//                                                         -sqop.index().pos();
//   sorting_vec.push_back(std::make_tuple(type, s, index, pos));
//   pos += 1;
// }

// std::sort(sorting_vec.begin(), sorting_vec.end());

// std::vector<int> sign_order;
// std::vector<SQOperator> new_sqops;
// for (const auto &tpl : sorting_vec) {
//   int idx = std::get<3>(tpl);
//   sign_order.push_back(idx);
//   new_sqops.push_back(operators_[idx]);
// }
// factor *= permutation_sign(sign_order);
// operators_ = new_sqops;

// old code for SQOperator canonicalization
// std::vector<int> sqops_pos(nops(), -1);
// std::vector<std::tuple<int, int, int, int>> sorting_vec;
// int pos = 0;
// for (const auto &sqop : operators_) {
//   int type = (sqop.type() == SQOperatorType::Creation) ? 0 : 1;
//   int s = sqop.index().space();
//   // Annihilation operators are written in reverse order
//   int index = (sqop.type() == SQOperatorType::Creation)
//                   ? sqop.index().pos()
//                   : -sqop.index().pos();
//   sorting_vec.push_back(std::make_tuple(type, s, index, pos));
//   pos += 1;
// }

// std::sort(sorting_vec.begin(), sorting_vec.end());

// std::vector<int> sign_order;
// std::vector<SQOperator> new_sqops;
// for (const auto &tpl : sorting_vec) {
//   int idx = std::get<3>(tpl);
//   sign_order.push_back(idx);
//   new_sqops.push_back(operators_[idx]);
// }
// factor *= permutation_sign(sign_order);
// operators_ = new_sqops;