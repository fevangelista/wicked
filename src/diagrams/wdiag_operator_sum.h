#ifndef _wicked_diag_operator_set_h_
#define _wicked_diag_operator_set_h_

#include <map>
#include <vector>

#include "wicked-def.h"
#include "wdiag_operator.h"

using dop_sum_t = std::map<std::vector<WDiagOperator>, scalar_t>;

/// A class to represent operators
class WDiagOperatorSum {
public:
  /// Construct an empty sum
  WDiagOperatorSum();

  //  /// Construct sum with a vector of diagrams
  //  WDiagOperatorSum(const std::vector<WDiagOperator> &vec_dop,
  //                   scalar_t factor = scalar_t(1));

  /// Construct sum with a vector of diagrams
  WDiagOperatorSum(const std::vector<std::vector<WDiagOperator>> &vec_vec_dop,
                   scalar_t factor = scalar_t(1));

  /// Add a vector of diagrams to this sum
  void add(const std::vector<WDiagOperator> &vec_dop, scalar_t factor = 1);

  /// Return the sum object
  const dop_sum_t &sum() const;

  /// addition assignment
  WDiagOperatorSum &operator+=(WDiagOperatorSum &rhs);
  /// subtraction assignment
  WDiagOperatorSum &operator-=(WDiagOperatorSum &rhs);
  /// multiplication assignment (scalar)
  WDiagOperatorSum &operator*=(scalar_t factor);
  /// division assignment (scalar)
  WDiagOperatorSum &operator/=(scalar_t factor);

  /// Return a string representation of the operator
  std::string str() const;

private:
  /// A vector containing pairs ((op1, op2, ...), factor)
  dop_sum_t sum_;
};

/// Write a string representation of the operator to a stream
std::ostream &operator<<(std::ostream &os, const WDiagOperatorSum &opsum);

// Helper functions

/// Helper function to create a sum of diagrammatic operators
/// @param components a vector of strings of the form "oo->vv" which specify the
/// components of this operator
/// E.g.
/// auto T1 = make_operator("T1", {"o->v"});
/// auto F = make_operator("F", {"o->o","o->v","v->o","v->v"});
WDiagOperatorSum make_operator(const std::string &label,
                               const std::vector<std::string> &components);

/// Creates a new object with the commutator [A,B]
WDiagOperatorSum commutator(const WDiagOperatorSum &A,
                            const WDiagOperatorSum &B);

/// Creates a new object with the exponential exp(A) truncated at a given order
WDiagOperatorSum exp(const WDiagOperatorSum &A, int order);

/// Creates a new object with the Baker-Campbell-Hausdorff expansion of the
/// quantity exp(-A) B exp(A) truncated at a given order
WDiagOperatorSum BCH_expansion(const WDiagOperatorSum &A,
                               const WDiagOperatorSum &B);

#endif // _wicked_diag_operator_set_h_
