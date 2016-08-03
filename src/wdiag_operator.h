#ifndef _wicked_diag_operator_h_
#define _wicked_diag_operator_h_

#include <vector>
#include <string>

#include "wicked-def.h"

/// A class to represent operators
class WDiagOperator {

public:
  /// Constructor. Set number of
  WDiagOperator(const std::string &label, const std::vector<int> &ann,
            const std::vector<int> &cre);

  /// Return the label of the operator
  std::string &label();

  /// Return the number of indices for a given space and type (cre/ann)
  int num_indices(int space, bool creation) const;

  /// Return a string representation of the operator
  std::string str() const;

private:
  /// The label of the operator
  std::string label_;

  /// The number of creation/annihilation operators in each space
  graph_t ops_;
};

#endif // _wicked_diag_operator_h_
