#ifndef _wicked_term_h_
#define _wicked_term_h_

#include <map>
#include <numeric>
#include <string>
#include <vector>

#include "index.h"
#include "wicked-def.h"

class Index;
class SQOperator;
class Tensor;

/// A class to represent a term in a SQ expression. A term includes:
/// 1) a product of tensors
/// 2) a product of operators normal ordered with respect to the vacuum
class Term {
public:
  // ==> Constructor <==

  Term();

  // ==> Class public interface <==

  /// Add one or more SQOperator
  void add(const SQOperator &op);

  /// Add a tensor
  void add(const Tensor &tensor);

  /// Return the number of SQ operators
  int noperators() const;

  /// Return the number of SQ operators
  int nops() const;

  /// Return the SQ operators
  const std::vector<SQOperator> &ops() const { return operators_; }

  /// Return the tensors
  const std::vector<Tensor> &tensors() const { return tensors_; }

  /// Return a vector containing all indices used in this term
  std::vector<Index> indices() const;

  /// Reindex this term
  void reindex(index_map_t &idx_map);

  /// Canonicalize this term and return the overall phase factor
  scalar_t canonicalize();

  /// Canonicalize the indices of all tensors
  scalar_t canonicalize_tensor_indices();

  /// Comparison operator used for sorting
  bool operator<(const Term &term) const;

  /// Comparison operator used for sorting
  bool operator==(const Term &term) const;

  /// Return a string representation
  std::string str() const;

  /// Return a LaTeX representation
  std::string latex() const;

  /// Return an ambit (C++ code) representation
  std::string ambit() const;

private:
  // ==> Class private data <==

  std::vector<SQOperator> operators_;
  std::vector<Tensor> tensors_;

  // ==> Class private functions <==

  std::string operator_str() const;
  std::string tensor_str() const;

  // Used in the canonicalization routine to find how the indices of a tensor
  // connect to all the other tensors
  std::vector<std::pair<std::string, std::vector<int>>>
  tensor_connectivity(const Tensor &t, bool upper) const;
};

// Helper functions

/// Print to an output stream
std::ostream &operator<<(std::ostream &os, const Term &term);

std::ostream &operator<<(std::ostream &os,
                         const std::pair<Term, scalar_t> &term);

///// Create an operator
Term make_term(const std::string &label, const std::vector<std::string> &cre,
               const std::vector<std::string> &ann);

#endif // _wicked_term_h_
