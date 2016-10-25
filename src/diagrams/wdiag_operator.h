#ifndef _wicked_diag_operator_h_
#define _wicked_diag_operator_h_

#include <vector>
#include <string>

#include "wicked-def.h"
#include "wdiag_vertex.h"

/// A class to represent operators
class WDiagOperator {

public:
  /// Constructor. Set number of
  WDiagOperator(const std::string &label, const std::vector<int> &cre,
                const std::vector<int> &ann);

  /// Return the label of the operator
  const std::string &label() const;

  scalar_t factor() const;

  WDiagVertex vertex() const;

  /// Return the number of indices for a given space and type (cre/ann)
  int num_indices(int space, bool creation) const;

  /// Return the number of creation operators in space
  int num_cre(int space) const;

  /// Return the number of annihilation operators in space
  int num_ann(int space) const;

  /// Return the particle rank of this operator (sum creation + sum annilation)
  int rank() const;

  /// Comparison operator used for sorting
  bool operator<(WDiagOperator const &other) const;

  /// Return a string representation of the operator
  std::string str() const;

  /// Write a string representation of the operator to a stream
  friend std::ostream &operator<<(std::ostream &os, const WDiagOperator &op);

private:
  /// The label of the operator
  std::string label_;

  /// The factor associated with this operator
  scalar_t factor_;

  /// The number of creation/annihilation operators in each space
  WDiagVertex vertex_;
};

// Helper functions

/// Create an operator
WDiagOperator make_diag_operator(const std::string &label,
                                 const std::vector<std::string> &cre_labels,
                                 const std::vector<std::string> &ann_labels);

/// Return the particle rank of a vector of operators
int operators_rank(const std::vector<WDiagOperator> &ops);

#endif // _wicked_diag_operator_h_
