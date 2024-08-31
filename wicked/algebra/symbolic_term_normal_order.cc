#include <algorithm>

#include "helpers/combinatorics.h"
#include "helpers/helpers.h"
#include "helpers/orbital_space.h"
#include "helpers/stl_utils.hpp"

#include "symbolic_term.h"

using namespace std;

auto is_fermionic_operator_pair = [](SQOperator const &a, SQOperator const &b) {
  return a.field_type() == FieldType::Fermion and
         b.field_type() == FieldType::Fermion;
};

void recursive_vacuum_normal_order(
    const SymbolicTerm &term, scalar_t c,
    vector<pair<SymbolicTerm, scalar_t>> &new_terms,
    bool only_same_index_contractions) {
  const auto &ops = term.ops();
  const int n = static_cast<int>(ops.size());
  // if there are no operators, we are done
  if (n == 0) {
    new_terms.push_back({term, c});
    return;
  }

  // check that the term is valid (no adjacent identical fermionic operators)
  bool valid = has_no_adjacent_identical_terms<SQOperator>(
      ops, is_fermionic_operator_pair);

  if (not valid)
    return;

  // find the first pair of operators that are not in normal order
  auto it = std::adjacent_find(
      ops.begin(), ops.end(),
      [](SQOperator const &a, SQOperator const &b) { return b < a; });
  const bool is_normal_ordered = (it == ops.end());

  if (is_normal_ordered) {
    new_terms.push_back({term, c});
  } else {
    const size_t pos = std::distance(ops.begin(), it);
    const auto a = ops[pos];
    const auto b = ops[pos + 1];

    // 1. swap the operators at position pos and pos+1

    // compute the sign of the commutator
    const scalar_t sign = a.commutator_factor(b);
    // create a new term with the operators swapped
    auto swap_ops = ops;
    std::swap(swap_ops[pos], swap_ops[pos + 1]);
    auto swap_term = SymbolicTerm(false, swap_ops, term.tensors());
    // normal order the new term
    recursive_vacuum_normal_order(swap_term, c * sign, new_terms,
                                  only_same_index_contractions);

    // 2. contract the operators at position pos and pos+1

    // Insert a delta if we swap a creation and annihilation of the same space
    if ((a.is_creation() != b.is_creation()) and (a.space() == b.space())) {
      bool is_same_index = a.index() == b.index();
      if (not only_same_index_contractions or is_same_index) {
        std::vector<SQOperator> cont_ops;
        cont_ops.insert(cont_ops.end(), ops.begin(), ops.begin() + pos);
        cont_ops.insert(cont_ops.end(), ops.begin() + pos + 2, ops.end());
        // for (int i = 0; i < pos; i++) {
        //   cont_ops.push_back(ops[i]);
        // }
        // for (int i = pos + 2; i < n; i++) {
        //   cont_ops.push_back(ops[i]);
        // }
        auto removed_term = SymbolicTerm(false, cont_ops, term.tensors());
        // add the delta tensor if the indices are different
        if (not is_same_index) {
          auto lower = b.is_creation() ? a.index() : b.index();
          auto upper = b.is_creation() ? b.index() : a.index();
          Tensor t("delta", {lower}, {upper}, SymmetryType::Nonsymmetric);
          removed_term.add(t);
        }
        recursive_vacuum_normal_order(removed_term, c, new_terms,
                                      only_same_index_contractions);
      }
    }
  }
}

std::vector<std::pair<SymbolicTerm, scalar_t>>
SymbolicTerm::vacuum_normal_order(bool only_same_index_contractions) const {
  std::vector<std::pair<SymbolicTerm, scalar_t>> new_terms;
  recursive_vacuum_normal_order(*this, 1, new_terms,
                                only_same_index_contractions);
  return new_terms;
}

// std::vector<std::pair<SymbolicTerm, scalar_t>>
// SymbolicTerm::normal_order(bool only_same_index_contractions) const {
//   // Step 1. Generating elementary contractions
//   // std::vector<ElementaryContraction> contr_vec;
//   // for (int s = 0; s < orbital_subspaces->num_spaces(); s++) {
//   // }
//   auto new_term = *this;
//   new_term.set_normal_ordered(true);
//   return {{new_term, 1}};
// }