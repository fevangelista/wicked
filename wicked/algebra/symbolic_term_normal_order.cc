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

void recursive_normal_order(const SymbolicTerm &term, scalar_t c,
                            vector<pair<SymbolicTerm, scalar_t>> &new_terms,
                            bool only_same_index_contractions) {
  // loop over all possible pairs of operators
  const auto ops = term.ops();
  const int n = static_cast<int>(ops.size());
  for (int i = 0; i < n - 1; i++) {
    auto a = ops[i];
    auto b = ops[i + 1];
    // if we have an identical pair of fermionic operators we can skip this term
    if ((a == b) and is_fermionic_operator_pair(a, b)) {
      return;
    }
  }
  bool is_normal_ordered = true;
  size_t pos = n;
  // find a pair out of order
  for (int i = 0; i < n - 1; i++) {
    auto a = ops[i];
    auto b = ops[i + 1];
    if (b < a) {
      is_normal_ordered = false;
      pos = i;
      break;
    }
  }
  if (is_normal_ordered) {
    new_terms.push_back({term, c});
  } else {
    const auto a = ops[pos];
    const auto b = ops[pos + 1];
    // 1. swap the operators at position pos and pos+1
    auto new_ops = ops;
    scalar_t sign = new_ops[pos].commutator_factor(new_ops[pos + 1]);
    swap(new_ops[pos], new_ops[pos + 1]);
    auto new_term = SymbolicTerm(false, new_ops, term.tensors());
    recursive_normal_order(new_term, c * sign, new_terms,
                           only_same_index_contractions);

    // 2. contract the operators at position pos and pos+1
    // are we swapping a creation and annihilation operator of the same space?
    if ((a.is_quasiparticle_creation() == not b.is_quasiparticle_creation()) and
        (a.space() == b.space())) {
      bool is_same_index = a.index() == b.index();
      if (not only_same_index_contractions or is_same_index) {
        std::vector<SQOperator> new_ops2;
        for (int i = 0; i < pos; i++) {
          new_ops2.push_back(ops[i]);
        }
        for (int i = pos + 2; i < n; i++) {
          new_ops2.push_back(ops[i]);
        }
        auto removed_term = SymbolicTerm(false, new_ops2, term.tensors());
        if (not is_same_index) {
          auto upper = {new_ops[pos].index()};
          auto lower = {new_ops[pos + 1].index()};
          if (new_ops[pos].is_creation()) {
            std::swap(upper, lower);
          }
          Tensor t("delta", upper, lower, SymmetryType::Nonsymmetric);
          removed_term.add(t);
        }
        recursive_normal_order(removed_term, c, new_terms,
                               only_same_index_contractions);
      }
    }

    //     auto ops = symterm.ops();
    // auto opt_pair = remove_first_unordered_pair(
    //     ops, [](SQOperator const &a, SQOperator const &b) {
    //       return not a.is_quasiparticle_creation() and
    //              b.is_quasiparticle_creation() and (a.space() == b.space());
    //     });
    // bool valid = has_no_adjacent_identical_terms<SQOperator>(
    //     ops, is_fermionic_operator_pair);
    // if (opt_pair and valid) {
    //   bool is_same_index = opt_pair->first.index() ==
    //   opt_pair->second.index(); if (not only_same_index_contractions or
    //   is_same_index) {
    //     auto removed_term = SymbolicTerm(false, ops, symterm.tensors());
    //     if (not is_same_index) {
    //       auto upper = {opt_pair->first.index()};
    //       auto lower = {opt_pair->second.index()};
    //       if (opt_pair->first.is_creation()) {
    //         std::swap(upper, lower);
    //       }
    //       Tensor t("delta", upper, lower, SymmetryType::Nonsymmetric);
    //       removed_term.add(t);
    //     }
    //     return removed_term;
    //   }
    // }
  }
}

std::vector<std::pair<SymbolicTerm, scalar_t>>
SymbolicTerm::vacuum_normal_order(bool only_same_index_contractions) const {
  std::vector<std::pair<SymbolicTerm, scalar_t>> new_terms;
  recursive_normal_order(*this, 1, new_terms, only_same_index_contractions);
  return new_terms;
}

std::vector<std::pair<SymbolicTerm, scalar_t>>
SymbolicTerm::normal_order(bool only_same_index_contractions) const {
  // Step 1. Generating elementary contractions
  // std::vector<ElementaryContraction> contr_vec;
  // for (int s = 0; s < orbital_subspaces->num_spaces(); s++) {
  // }
  auto new_term = *this;
  new_term.set_normal_ordered(true);
  return {{new_term, 1}};
}