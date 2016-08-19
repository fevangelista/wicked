#ifndef _wicked_wterm_h_
#define _wicked_wterm_h_

#include <string>
#include <vector>
#include <map>
#include <numeric>

#include "combinatorics.h"
#include "windex.h"
#include "wsqoperator.h"
#include "wtensor.h"

/// A class to represent a term in a SQ expression. A term includes:
/// 1) a product of tensors
/// 2) a product of operators normal ordered with respect to the vacuum
class WTerm {
public:
  // ==> Constructor <==

  WTerm();

  // ==> Class public interface <==

  /// Add one or more WSQOperator
  void add(const WSQOperator &op);

  /// Add a tensor
  void add(const WTensor& tensor);

  /// Return the scalar factor
  scalar_t factor() const;

  /// Return the scalar factor
  void set_factor(scalar_t value);

  /// Return the number of SQ operators
  int noperators() const;

  /// Return the number of SQ operators
  int noperators(int n) const { return (operators_[n].size()); }

  /// Return the number of SQ operators in each orbital space
  std::vector<int> noperators_per_space() const;

  /// Return the SQ operators
  const std::vector<std::vector<WSQOperator>> &operators() const { return operators_; }

  /// Return the SQ operators in space n
  const std::vector<WSQOperator> &operators(int n) const { return operators_[n]; }

  /// Return the tensors
  const std::vector<WTensor> &tensors() const { return tensors_; }

  /// Return a vector containing all indices used in this term
  std::vector<WIndex> indices() const;

  /// Reindex this term
  void reindex(index_map_t &idx_map);

  /// Canonicalize this term
  void canonicalize();

  /// Comparison operator used for sorting
  bool operator<(const WTerm &term) const;

  /// Comparison operator used for sorting
  bool operator==(const WTerm &term) const;

  /// Return a string representation
  std::string str() const;

  /// Return a LaTeX representation
  std::string latex() const;

private:
  // ==> Class private data <==

  scalar_t factor_ = 1;
  std::vector<std::vector<WSQOperator>> operators_;
  std::vector<WTensor> tensors_;

  // ==> Class private functions <==

  std::string operator_str() const;
  std::string tensor_str() const;
};

// Helper functions
/// Create an operator
WTerm make_operator(const std::string &label,
                    const std::vector<std::string> &cre,
                    const std::vector<std::string> &ann);

#endif // _wicked_wterm_h_
