#include <algorithm>

#include "expression.h"

bool Expression::is_vacuum_normal_ordered() const {
  // check that all terms are vacuum normal ordered
  for (const auto &[symterm, c] : terms()) {
    if (not symterm.is_vacuum_normal_ordered()) {
      return false;
    }
  }
  return true;
}

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
  Expression new_expr;
  for (const auto &[symterm, c] : terms()) {
    auto new_terms = symterm.vacuum_normal_order(only_same_index_contractions);
    for (const auto &[new_symterm, new_c] : new_terms) {
      new_expr.add(new_symterm, c * new_c);
    }
  }
  return new_expr;
}

// Expression Expression::normal_ordered(bool only_same_index_contractions)
// const {
//   Expression new_expr;
//   for (const auto &[symterm, c] : terms()) {
//     const auto &new_terms =
//     symterm.normal_order(only_same_index_contractions); for (const auto
//     &[new_symterm, new_c] : new_terms) {
//       new_expr.add(new_symterm, c * new_c);
//     }
//   }
//   return new_expr;
// }