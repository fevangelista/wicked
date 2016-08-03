#ifndef _wicked_wterm_h_
#define _wicked_wterm_h_

#include <string>
#include <vector>
#include <map>

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
  void add(WSQOperator &op);

  /// Add a tensor
  void add(WTensor &tensor_);

  /// Return the scalar factor
  scalar_t factor() const;

  /// Return the scalar factor
  void set_factor(scalar_t value);

  /// Return the number of SQ operators
  int noperators() const { return (operators_.size()); }

  /// Return the SQ operators
  std::vector<WSQOperator> &operators() { return operators_; }

  /// Return a map with the number of indices per space
  std::map<OrbitalSpaceType, int> space_count();

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
  std::vector<WSQOperator> operators_;
  std::vector<WTensor> tensors_;

  // ==> Class private functions <==

  std::string operator_str() const;
  std::string tensor_str() const;
};

// Helper functions
/// Create an operator
WTerm make_operator(const std::string &label,
                    const std::vector<OrbitalSpaceType> &cre,
                    const std::vector<OrbitalSpaceType> &ann);

#endif // _wicked_wterm_h_
