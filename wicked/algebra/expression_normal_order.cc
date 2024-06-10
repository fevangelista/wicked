#include <algorithm>

#include "expression.h"

auto is_fermionic_operator_pair = [](SQOperator const &a, SQOperator const &b) {
  return a.field_type() == FieldType::Fermion and
         b.field_type() == FieldType::Fermion;
};

Expression Expression::normal_ordered(bool only_same_index_contractions) const {
  Expression new_expr;
  for (const auto &[symterm, c] : terms()) {
    const auto &new_terms = symterm.normal_order(only_same_index_contractions);
    for (const auto &[new_symterm, new_c] : new_terms) {
      new_expr.add(new_symterm, c * new_c);
    }
  }
  return new_expr;
}

void normal_order_term(const SymbolicTerm &symterm, const scalar_t &c,
                       Expression &expr, bool only_same_index_contractions,
                       int level);

Expression
Expression::vacuum_normal_ordered(bool only_same_index_contractions) const {
  // check that all terms are vacuum normal ordered
  for (const auto &[symterm, c] : terms()) {
    if (symterm.is_labeled_normal_ordered()) {
      throw std::runtime_error("Expression::vacuum_normal_ordered: "
                               "this function can be applied only to "
                               "non-normal ordered expressions");
    }
  }

  // // eliminate all terms that contain adjacent identical operators (e.g. a+
  // a+) Expression no_expr; for (const auto &[symterm, c] : terms()) {
  //   auto ops = symterm.ops();
  //   bool valid = has_no_adjacent_identical_terms<SQOperator>(
  //       ops, is_fermionic_operator_pair);
  //   if (valid) {
  //     auto valid_term = SymbolicTerm(false, ops, symterm.tensors());
  //     no_expr.add(valid_term, c);
  //   }
  // }

  // /// recursively normal order the expression
  // Expression new_expr;
  // int level = 0;
  // for (const auto &[symterm, c] : no_expr.terms()) {
  //   normal_order_term(symterm, c, new_expr, only_same_index_contractions,
  //                     level);
  //   std::cout << ">" << std::endl;
  // }
  Expression new_expr;
  for (const auto &[symterm, c] : terms()) {
    auto new_terms = symterm.vacuum_normal_order(only_same_index_contractions);
    for (const auto &[new_symterm, new_c] : new_terms) {
      new_expr.add(new_symterm, c * new_c);
    }
  }
  return new_expr;
}

/// @brief Swap two operators in a symbolic term and return the new term
std::optional<std::pair<SymbolicTerm, scalar_t>>
swap_operators2(const SymbolicTerm &symterm) {
  auto ops = symterm.ops();
  auto swapped_ops = swap_first_unordered_pair(
      ops, [](SQOperator const &a, SQOperator const &b) {
        return b.normal_ordered_less(a);
      });
  // check that the term is valid (i.e. no adjacent identical operators)
  bool valid = has_no_adjacent_identical_terms<SQOperator>(
      ops, is_fermionic_operator_pair);
  // if valid, return the new term
  if (valid) {
    scalar_t sign = ops[0].commutator_factor(ops[1]);
    auto swapped_term = SymbolicTerm(false, ops, symterm.tensors());
    return std::make_pair(swapped_term, sign);
  }
  return std::nullopt;
}

/// @brief Remove a pair of operators from a term
/// @param only_same_index_contractions if true, only remove pairs of operators
/// labeled with the same index
std::optional<SymbolicTerm> remove_pair2(const SymbolicTerm &symterm,
                                         bool only_same_index_contractions) {
  auto ops = symterm.ops();
  auto opt_pair = remove_first_unordered_pair(
      ops, [](SQOperator const &a, SQOperator const &b) {
        return not a.is_quasiparticle_creation() and
               b.is_quasiparticle_creation() and (a.space() == b.space());
      });
  bool valid = has_no_adjacent_identical_terms<SQOperator>(
      ops, is_fermionic_operator_pair);
  if (opt_pair and valid) {
    bool is_same_index = opt_pair->first.index() == opt_pair->second.index();
    if (not only_same_index_contractions or is_same_index) {
      auto removed_term = SymbolicTerm(false, ops, symterm.tensors());
      if (not is_same_index) {
        auto upper = {opt_pair->first.index()};
        auto lower = {opt_pair->second.index()};
        if (opt_pair->first.is_creation()) {
          std::swap(upper, lower);
        }
        Tensor t("delta", upper, lower, SymmetryType::Nonsymmetric);
        removed_term.add(t);
      }
      return removed_term;
    }
  }
  return std::nullopt;
}

void normal_order_term(const SymbolicTerm &symterm, const scalar_t &c,
                       Expression &expr, bool only_same_index_contractions,
                       int level) {
  // if this term is already normal ordered, add it to the new expression
  std::cout << "normal_order_term: " << c.str(true) << " " << symterm.str()
            << " " << level << " "
            << (symterm.is_vacuum_normal_ordered() ? " added" : "")
            << std::endl;
  auto ops = symterm.ops();
  bool valid = has_no_adjacent_identical_terms<SQOperator>(
      ops, is_fermionic_operator_pair);
  if (not valid)
    return;
  if (symterm.is_vacuum_normal_ordered()) {
    expr.add(symterm, c);
  } else {
    auto swap_result = swap_operators2(symterm);
    if (swap_result) {
      const auto &[swapped_term, sign] = swap_result.value();
      // here we change the sign of the term
      normal_order_term(swapped_term, sign * c, expr,
                        only_same_index_contractions, level + 1);
    }
    auto remove_result = remove_pair2(symterm, only_same_index_contractions);
    if (remove_result) {
      const auto &removed_term = remove_result.value();
      normal_order_term(removed_term, c, expr, only_same_index_contractions,
                        level + 1);
    }
  }
}

bool Expression::is_vacuum_normal_ordered() const {
  // check that all terms are vacuum normal ordered
  for (const auto &[symterm, c] : terms()) {
    if (not symterm.is_vacuum_normal_ordered()) {
      return false;
    }
  }
  return true;
}

// /// @brief Swap two operators in a term
// void swap_operators(const SymbolicTerm &symterm, const scalar_t &c,
//                     Expression &new_expr) {
//   auto ops = symterm.ops();
//   swap_first_unordered_pair(ops, [](SQOperator const &a, SQOperator const
//   &b)
//   {
//     // return b < a;
//     return b.normal_ordered_less(a);
//   });

//   bool valid = has_no_adjacent_identical_terms(ops);
//   if (valid) {
//     auto swapped_term = SymbolicTerm(false, ops, symterm.tensors());
//     new_expr.add(swapped_term, -c);
//   }
// }

// /// @brief Remove a pair of operators from a term
// /// @param only_same_index_contractions if true, only remove pairs of
// operators
// /// labeled with the same index
// void remove_pair(const SymbolicTerm &symterm, const scalar_t &c,
//                  Expression &new_expr, bool only_same_index_contractions) {
//   auto ops = symterm.ops();
//   auto opt_pair = remove_first_unordered_pair(
//       ops, [](SQOperator const &a, SQOperator const &b) {
//         return not a.is_quasiparticle_creation() and
//                b.is_quasiparticle_creation() and (a.space() == b.space());
//       });
//   bool valid = has_no_adjacent_identical_terms(ops);
//   if (opt_pair and valid) {
//     bool is_same_index = opt_pair->first.index() ==
//     opt_pair->second.index(); if (not only_same_index_contractions or
//     is_same_index) {
//       auto removed_term = SymbolicTerm(false, ops, symterm.tensors());
//       if (not is_same_index) {
//         auto upper = {opt_pair->first.index()};
//         auto lower = {opt_pair->second.index()};
//         if (opt_pair->first.is_creation()) {
//           std::swap(upper, lower);
//         }
//         Tensor t("delta", upper, lower, SymmetryType::Nonsymmetric);
//         removed_term.add(t);
//       }
//       new_expr.add(removed_term, c);
//     }
//   }
// }

// Expression
// Expression::vacuum_normal_ordered(bool only_same_index_contractions) const
// {
//   // check that all terms are vacuum normal ordered
//   for (const auto &[symterm, c] : terms()) {
//     if (symterm.is_labeled_normal_ordered()) {
//       throw std::runtime_error("Expression::vacuum_normal_ordered: "
//                                "this function can be applied only to "
//                                "non-normal ordered expressions");
//     }
//   }

//   // loop over all terms in this expression and eliminate all terms that
//   // contain adjacent identical operators (since they give zero
//   contribution) Expression no_expr; for (const auto &[symterm, c] :
//   terms()) {
//     auto ops = symterm.ops();
//     bool valid = has_no_adjacent_identical_terms(ops);
//     if (valid) {
//       auto valid_term = SymbolicTerm(false, ops, symterm.tensors());
//       no_expr.add(valid_term, c);
//     }
//   }

//   // loop over all terms in this expression and normal order them
//   // NOTE: this is not the most efficient way to do this.
//   //       It should be replaced by a recursive function
//   while (no_expr.is_vacuum_normal_ordered() == false) {
//     Expression new_expr;
//     // loop over all terms in this expression and normal order them
//     for (const auto &[symterm, c] : no_expr.terms()) {
//       if (not symterm.is_vacuum_normal_ordered()) {

//         swap_operators(symterm, c, new_expr);
//         remove_pair(symterm, c, new_expr, only_same_index_contractions);
//       } else {
//         new_expr.add(symterm, c);
//       }
//     }
//     no_expr = new_expr;
//   }
//   return no_expr;
// }