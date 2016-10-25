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

  /// Construct sum with one diagram
  WDiagOperatorSum(WDiagOperator dop, scalar_t factor = 1);

  /// Construct sum with a vector of diagrams
  WDiagOperatorSum(std::vector<WDiagOperator> vec_dop, scalar_t factor = 1);

  /// Add a diagram to this sum
  void add(const WDiagOperator &dop, scalar_t factor = 1);

  /// Add a vector of diagrams to this sum
  void add(const std::vector<WDiagOperator> &vec_dop, scalar_t factor = 1);

  /// Return the sum object
  dop_sum_t &sum();

  /// addition assignment
  WDiagOperatorSum &operator+=(WDiagOperatorSum &rhs);
  /// subtraction assignment
  WDiagOperatorSum &operator-=(WDiagOperatorSum &rhs);
  /// multiplication assignment
  WDiagOperatorSum &operator*=(WDiagOperatorSum &rhs);
  /// division assignment
  WDiagOperatorSum &operator/=(WDiagOperatorSum &rhs);

  //  /// Return a string representation of the operator
  //  std::string str() const;

  //  /// Write a string representation of the operator to a stream
  //  friend std::ostream &operator<<(std::ostream &os, const WDiagOperator
  //  &op);

private:
  /// A vector containing pairs ((op1, op2, ...), factor)
  dop_sum_t sum_;
};

#endif // _wicked_diag_operator_set_h_
