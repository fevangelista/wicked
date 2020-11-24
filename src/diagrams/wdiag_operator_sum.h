#ifndef _wicked_diag_operator_set_h_
#define _wicked_diag_operator_set_h_

#include <map>
#include <vector>

#include "wdiag_operator.h"
#include "wicked-def.h"

using dop_sum_t = std::map<std::vector<WDiagOperator>, scalar_t>;

/// A class to represent operators
class OperatorSum {
public:
  /// Construct an empty sum
  OperatorSum();

  /// Construct sum with a vector of diagrams
  OperatorSum(const std::vector<std::vector<WDiagOperator>> &vec_vec_dop,
              scalar_t factor = scalar_t(1));

  /// Add a vector of diagrams to this sum
  void add(const std::vector<WDiagOperator> &vec_dop, scalar_t factor = 1);

  /// Return the sum object
  const dop_sum_t &sum() const;

  /// addition assignment
  OperatorSum &operator+=(const OperatorSum &rhs);
  /// subtraction assignment
  OperatorSum &operator-=(const OperatorSum &rhs);
  /// multiplication assignment (scalar)
  OperatorSum &operator*=(const OperatorSum &rhs);
  /// multiplication assignment (scalar)
  OperatorSum &operator*=(scalar_t factor);
  /// division assignment (scalar)
  OperatorSum &operator/=(scalar_t factor);

  /// Return a string representation of the operator
  std::string str() const;

private:
  /// A vector containing pairs ((op1, op2, ...), factor)
  dop_sum_t sum_;
};

/// multiplication
OperatorSum operator*(OperatorSum lhs, const OperatorSum &rhs);

/// Write a string representation of the operator to a stream
std::ostream &operator<<(std::ostream &os, const OperatorSum &opsum);

// Helper functions

/// Helper function to create a sum of diagrammatic operators
/// @param components a vector of strings of the form "oo->vv" which specify the
/// components of this operator
/// E.g.
/// auto T1 = make_operator("T1", {"o->v"});
/// auto F = make_operator("F", {"o->o","o->v","v->o","v->v"});
OperatorSum make_operator(const std::string &label,
                          const std::vector<std::string> &components);

/// Creates a new object with the commutator [A,B]
OperatorSum commutator(const OperatorSum &A, const OperatorSum &B);

/// Creates a new object with the exponential exp(A) truncated at a given order
OperatorSum exp(const OperatorSum &A, int order);

/// Creates a new object with the Baker-Campbell-Hausdorff expansion of the
/// quantity exp(-B) A exp(B) truncated at a given order n
OperatorSum bch_series(const OperatorSum &A, const OperatorSum &B, int n);

#endif // _wicked_diag_operator_set_h_
