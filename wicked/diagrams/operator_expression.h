#ifndef _wicked_diag_operator_set_h_
#define _wicked_diag_operator_set_h_

#include <map>
#include <vector>

#include "helpers/algebra.hpp"
#include "operator_product.h"
#include "wicked-def.h"

/// A class to represent operators
class OperatorExpression : public Algebra<OperatorProduct, scalar_t> {
  using opexpr_t = Algebra<OperatorProduct, scalar_t>::vecspace_t;

public:
  /// Construct an empty sum
  OperatorExpression();

  /// Construct sum with a vector of diagrams
  OperatorExpression(const std::vector<OperatorProduct> &vec_vec_dop,
                     scalar_t factor = scalar_t(1));

  void add2(const OperatorExpression &expr, scalar_t factor = 1);

  void canonicalize();

  OperatorExpression adjoint() const {
    OperatorExpression expr;
    for (const auto &[e, c] : terms_) {
      expr.add(e.adjoint(), c);
    }
    return expr;
  }

  using Algebra<OperatorProduct, scalar_t>::operator*=;
  using Algebra<OperatorProduct, scalar_t>::is_equal;

  /// Return a string representation of the operator
  std::string str() const;
};

/// multiplication
OperatorExpression operator*(OperatorExpression lhs,
                             const OperatorExpression &rhs);

/// addition
OperatorExpression operator+(OperatorExpression lhs,
                             const OperatorExpression &rhs);

/// subtraction
OperatorExpression operator-(OperatorExpression lhs,
                             const OperatorExpression &rhs);

/// Write a string representation of the operator to a stream
std::ostream &operator<<(std::ostream &os, const OperatorExpression &opsum);

// Helper functions

/// Helper function to create a sum of diagrammatic operators
/// @param components a vector of strings of the form "v+ v+ o o" which specify
/// the components of this operator E.g. auto T1 = make_operator("T1", {"v+
/// o"}); auto F = make_operator("F", {"o+ o","v+ o","o+ v","v+ v"});
OperatorExpression
make_diag_operator_expression(const std::string &label,
                              const std::vector<std::string> &components,
                              bool unique = false);

/// Creates a new object with the commutator [A,B]
OperatorExpression commutator(const OperatorExpression &A,
                              const OperatorExpression &B);

// /// Creates a new object with the exponential exp(A) truncated at a given
// order OperatorExpression exp(const OperatorExpression &A, int order);

/// Creates a new object with the Baker-Campbell-Hausdorff expansion of the
/// quantity exp(-B) A exp(B) truncated at a given order n
OperatorExpression bch_series(const OperatorExpression &A,
                              const OperatorExpression &B, int n);

#endif // _wicked_diag_operator_set_h_
