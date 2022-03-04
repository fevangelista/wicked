#ifndef _wicked_diag_operator_set_h_
#define _wicked_diag_operator_set_h_

#include <map>
#include <vector>

#include "operator_product.h"
#include "wicked-def.h"

using dop_expr_t = std::map<OperatorProduct, scalar_t>;

/// A class to represent operators
class DiagOpExpression {
public:
  /// Construct an empty sum
  DiagOpExpression();

  /// Construct sum with a vector of diagrams
  DiagOpExpression(const std::vector<OperatorProduct> &vec_vec_dop,
                   scalar_t factor = scalar_t(1));

  /// Add a vector of diagrams to this sum
  void add(const OperatorProduct &vec_dop, scalar_t factor = 1);

  void add2(const DiagOpExpression &expr, scalar_t factor = 1);

  /// Return the sum object
  const dop_expr_t &sum() const;
  const dop_expr_t &terms() const;

  void canonicalize();

  /// addition assignment
  DiagOpExpression &operator+=(const DiagOpExpression &rhs);
  /// subtraction assignment
  DiagOpExpression &operator-=(const DiagOpExpression &rhs);
  /// multiplication assignment (scalar)
  DiagOpExpression &operator*=(const DiagOpExpression &rhs);
  /// multiplication assignment (scalar)
  DiagOpExpression &operator*=(scalar_t factor);
  /// division assignment (scalar)
  DiagOpExpression &operator/=(scalar_t factor);

  /// Return a string representation of the operator
  std::string str() const;

private:
  /// A vector containing pairs ((op1, op2, ...), factor)
  dop_expr_t terms_;
};

/// multiplication
DiagOpExpression operator*(DiagOpExpression lhs, const DiagOpExpression &rhs);

/// Write a string representation of the operator to a stream
std::ostream &operator<<(std::ostream &os, const DiagOpExpression &opsum);

// Helper functions

/// Helper function to create a sum of diagrammatic operators
/// @param components a vector of strings of the form "oo->vv" which specify the
/// components of this operator
/// E.g.
/// auto T1 = make_diag_operator("T1", {"o->v"});
/// auto F = make_diag_operator("F", {"o->o","o->v","v->o","v->v"});
DiagOpExpression
make_diag_operator_expression(const std::string &label,
                              const std::vector<std::string> &components);

/// Helper function to create a sum of diagrammatic operators
/// @param components a vector of strings of the form "oo->vv" which specify the
/// components of this operator
/// E.g.
/// auto T1 = make_operator("T1", {"v+ o"});
/// auto F = make_operator("F", {"o+ o","v+ o","o+ v","v+ v"});
DiagOpExpression
make_diag_operator_expression2(const std::string &label,
                               const std::vector<std::string> &components);

/// Creates a new object with the commutator [A,B]
DiagOpExpression commutator(const DiagOpExpression &A,
                            const DiagOpExpression &B);

// /// Creates a new object with the exponential exp(A) truncated at a given
// order DiagOpExpression exp(const DiagOpExpression &A, int order);

/// Creates a new object with the Baker-Campbell-Hausdorff expansion of the
/// quantity exp(-B) A exp(B) truncated at a given order n
DiagOpExpression bch_series(const DiagOpExpression &A,
                            const DiagOpExpression &B, int n);

#endif // _wicked_diag_operator_set_h_
