#ifndef _wicked_algebraic_term_h_
#define _wicked_algebraic_term_h_

#include <map>
#include <numeric>
#include <string>
#include <vector>

#include "wicked-def.h"
#include "windex.h"

class WSQOperator;
class WTensor;
class WIndex;

/// A class to represent a term in a SQ expression. A term includes:
/// 1) a product of tensors
/// 2) a product of operators normal ordered with respect to the vacuum
class WAlgebraicTerm {
public:
  // ==> Constructor <==

  WAlgebraicTerm();

  // ==> Class public interface <==

  /// Add one or more WSQOperator
  void add(const WSQOperator &op);

  /// Add a tensor
  void add(const WTensor &tensor);

  /// Return the number of SQ operators
  int noperators() const;

  /// Return the number of SQ operators
  int nops() const;

  /// Return the SQ operators
  const std::vector<WSQOperator> &ops() const { return operators_; }

  /// Return the tensors
  const std::vector<WTensor> &tensors() const { return tensors_; }

  /// Return a vector containing all indices used in this term
  std::vector<WIndex> indices() const;

  /// Reindex this term
  void reindex(index_map_t &idx_map);

  /// Canonicalize this term and return the overall phase factor
  scalar_t canonicalize();

  /// Comparison operator used for sorting
  bool operator<(const WAlgebraicTerm &term) const;

  /// Comparison operator used for sorting
  bool operator==(const WAlgebraicTerm &term) const;

  /// Return a string representation
  std::string str() const;

  /// Return a LaTeX representation
  std::string latex() const;

  /// Return an ambit (C++ code) representation
  std::string ambit() const;

private:
  // ==> Class private data <==

  std::vector<WSQOperator> operators_;
  std::vector<WTensor> tensors_;

  // ==> Class private functions <==

  std::string operator_str() const;
  std::string tensor_str() const;

  // Used in the canonicalization routine to find how the indices of a tensor
  // connect to all the other tensors
  std::vector<std::pair<std::vector<int>, std::string>>
  tensor_connectivity(std::vector<WIndex> indices) const;
};

// Helper functions

/// Print to an output stream
std::ostream &operator<<(std::ostream &os, const WAlgebraicTerm &term);

std::ostream &operator<<(std::ostream &os,
                         const std::pair<WAlgebraicTerm, scalar_t> &term);

///// Create an operator
WAlgebraicTerm make_algebraic_term(const std::string &label,
                                   const std::vector<std::string> &cre,
                                   const std::vector<std::string> &ann);

///// A class to represent a term in a SQ expression. A term includes:
///// 1) a product of tensors
///// 2) a product of operators normal ordered with respect to the vacuum
// class WScala {
// public:
//}

#endif // _wicked_algebraic_term_h_
