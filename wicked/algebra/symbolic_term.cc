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

bool SymbolicTerm::is_labeled_normal_ordered() const { return normal_ordered_; }

bool SymbolicTerm::is_vacuum_normal_ordered() const {
  return std::is_sorted(operators_.begin(), operators_.end(),
                        [](const SQOperator &a, const SQOperator &b) {
                          // return a.normal_ordered_less(b);
                          return a < b;
                        });
}

bool SymbolicTerm::is_creation_then_annihilation() const {
  return std::is_sorted(operators_.begin(), operators_.end(),
                        [](const SQOperator &a, const SQOperator &b) {
                          return b.is_creation() < a.is_creation();
                        });
}

void SymbolicTerm::set(const std::vector<SQOperator> &ops) { operators_ = ops; }

void SymbolicTerm::set(const Product<SQOperator> &op) { operators_ = op; }

void SymbolicTerm::add(const SQOperator &op) { operators_.push_back(op); }

void SymbolicTerm::add(const std::vector<SQOperator> &ops) {
  auto product = Product<SQOperator>(ops);
  add(product);
}

void SymbolicTerm::add(const Product<SQOperator> &ops) {
  for (const auto &op : ops) {
    add(op);
  }
}

void SymbolicTerm::add(const Tensor &tensor) { tensors_.push_back(tensor); }

int SymbolicTerm::nops() const { return operators_.size(); }

SymbolicTerm SymbolicTerm::adjoint() const {
  SymbolicTerm result;
  result.set_normal_ordered(normal_ordered_);
  for (const auto &tensor : tensors_) {
    result.add(tensor.adjoint());
  }
  result.add(operators_.adjoint());
  return result;
}

SymbolicTerm &SymbolicTerm::operator*=(const SymbolicTerm &rhs) {
  const auto [ops, normal_ordered] = operator_product(*this, rhs);
  for (const auto &tensor : rhs.tensors_) {
    add(tensor);
  }
  operators_ = ops;
  return *this;
}

SymbolicTerm operator*(SymbolicTerm lhs, const SymbolicTerm &rhs) {
  lhs *= rhs;
  return lhs;
}

// std::vector<Index> SymbolicTerm::indices() const {
//   std::vector<Index> result;
//   for (const auto &t : tensors_) {
//     const auto t_idx = t.indices();
//     result.insert(result.end(), t_idx.begin(), t_idx.end());
//   }
//   for (const auto &op : operators_) {
//     result.push_back(op.index());
//   }
//   return result;
// }

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
  std::vector<int> sqop_index_count(orbital_subspaces->num_spaces(), 0);
  // vector to keep track of how many indices in each space
  std::vector<int> tens_index_count(orbital_subspaces->num_spaces(), 0);
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
  for (int s = 0; s < orbital_subspaces->num_spaces(); s++) {
    WPRINT(cout << "\nSpace " << orbital_subspaces->label(s) << endl;);

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

std::pair<Product<SQOperator>, bool>
operator_product_1(const SymbolicTerm &lhs, const SymbolicTerm &rhs) {
  // Case 1: both terms are normal ordered
  const bool normal_ordered = false;
  Product<SQOperator> result;
  throw std::runtime_error("Multiplication of symbolic terms that are normal "
                           "ordered is not (yet) implemented.");
  return std::make_pair(result, normal_ordered);
}

std::pair<Product<SQOperator>, bool>
operator_product_2(const SymbolicTerm &lhs, const SymbolicTerm &rhs) {
  // Case 2: the left-hand side is normal ordered the right-hand side is not
  const bool normal_ordered = false;
  Product<SQOperator> result;
  throw std::runtime_error("Multiplication of symbolic terms that are normal "
                           "ordered is not (yet) implemented.");
  return std::make_pair(result, normal_ordered);
}
std::pair<Product<SQOperator>, bool>
operator_product_3(const SymbolicTerm &lhs, const SymbolicTerm &rhs) {
  // Case 3: the right-hand side is normal ordered the left-hand side is not
  const bool normal_ordered = false;
  Product<SQOperator> result;
  throw std::runtime_error("Multiplication of symbolic terms that are normal "
                           "ordered is not (yet) implemented.");
  return std::make_pair(result, normal_ordered);
}

std::pair<Product<SQOperator>, bool>
operator_product_4(const SymbolicTerm &lhs, const SymbolicTerm &rhs) {
  // Case 4: neither term is normal ordered
  const bool normal_ordered = false;
  Product<SQOperator> result;
  for (const auto &op : lhs.ops()) {
    result.push_back(op);
  }
  for (const auto &op : rhs.ops()) {
    result.push_back(op);
  }
  return std::make_pair(result, normal_ordered);
}

std::pair<Product<SQOperator>, bool> operator_product(const SymbolicTerm &lhs,
                                                      const SymbolicTerm &rhs) {

  // Case 1: both terms are normal ordered
  if (lhs.is_labeled_normal_ordered() and rhs.is_labeled_normal_ordered()) {
    return operator_product_1(lhs, rhs);
  }
  // Case 2: the left-hand side is normal ordered the right-hand side is not
  else if (lhs.is_labeled_normal_ordered() and
           not rhs.is_labeled_normal_ordered()) {
    return operator_product_2(lhs, rhs);
  }
  // Case 3: the right-hand side is normal ordered the left-hand side is not
  else if (not lhs.is_labeled_normal_ordered() and
           rhs.is_labeled_normal_ordered()) {
    return operator_product_3(lhs, rhs);
  }
  // Case 4: neither term is normal ordered
  return operator_product_4(lhs, rhs);
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

std::string SymbolicTerm::latex() const {
  std::vector<std::string> str_vec;
  for (const Tensor &tensor : tensors_) {
    str_vec.push_back(tensor.latex());
  }
  if (nops()) {
    if (normal_ordered())
      str_vec.push_back("\\{");
    if (is_creation_then_annihilation()) {
      std::vector<std::string> str_vec_cre;
      std::vector<std::string> str_vec_ann;
      for (const auto &op : operators_) {
        if (op.is_creation()) {
          str_vec_cre.push_back(op.index().latex());
        } else {
          str_vec_ann.push_back(op.index().latex());
        }
      }
      // reverse the order of the annihilation operators
      std::reverse(str_vec_ann.begin(), str_vec_ann.end());
      std::string op_str = "\\hat{a}^{" + join(str_vec_cre, " ") + "}_{" +
                           join(str_vec_ann, " ") + "}";
      str_vec.push_back(op_str);
    } else {
      for (const auto &op : operators_) {
        str_vec.push_back(op.latex());
      }
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
