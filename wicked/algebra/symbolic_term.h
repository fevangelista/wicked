#ifndef _wicked_symbolic_term_h_
#define _wicked_symbolic_term_h_

#include <map>
#include <numeric>
#include <string>
#include <vector>

#include "../helpers/product.hpp"
#include "../wicked-def.h"
#include "index.h"
#include "sqoperator.h"
#include "tensor.h"

/// A class to represent a term in a SQ expression. A term includes:
/// 1) a product of tensors
/// 2) a product of operators normal ordered with respect to the vacuum
class SymbolicTerm {
public:
  // ==> Constructor <==

  SymbolicTerm();

  SymbolicTerm(bool normal_ordered, const std::vector<SQOperator> &op,
               const std::vector<Tensor> &tensors);

  // ==> Class public interface <==

  /// Add one or more SQOperator
  void set_normal_ordered(bool val);
  void set(const std::vector<Tensor> &tensors);
  void set(const std::vector<SQOperator> &op);
  void set(const Product<SQOperator> &op);

  /// Add one or more SQOperators to the left
  void add(const SQOperator &op);
  void add(const std::vector<SQOperator> &ops);
  void add(const Product<SQOperator> &ops);

  /// Add a tensor
  void add(const Tensor &tensor);

  /// Return the number of SQ operators
  int noperators() const;

  /// Return the number of SQ operators
  int nops() const;

  /// @return is the SQ operator product normal ordered?
  bool normal_ordered() const { return normal_ordered_; }

  /// Return the SQ operators
  const std::vector<SQOperator> &ops() const { return operators_.vec(); }

  /// Return the tensors
  const std::vector<Tensor> &tensors() const { return tensors_; }

  /// Apply a re-indexing map to this symbolic term
  void reindex(index_map_t &idx_map);

  /// Canonicalize this term and return the overall phase factor
  scalar_t canonicalize();

  /// Return the adjoint of this symbolic term
  SymbolicTerm adjoint() const;

  /// Canonicalize this term and return the overall phase factor
  // bool is_connected();

  scalar_t simplify();

  /// Comparison operator used for sorting
  bool operator<(const SymbolicTerm &term) const;

  /// Comparison operator used for sorting
  bool operator==(const SymbolicTerm &term) const;

  /// Return a string representation
  std::string str() const;

  /// Return a LaTeX representation
  std::string latex() const;

  /// Return a compilable representation
  std::string compile(const std::string &format) const;

protected:
  // ==> Class private data <==
  bool normal_ordered_ = false;
  Product<SQOperator> operators_;
  std::vector<Tensor> tensors_;

  // ==> Class private functions <==

  // Used in the canonicalization routine to find how the indices of a tensor
  // connect to all the other tensors
  std::vector<std::pair<std::string, std::vector<int>>>
  tensor_connectivity(const Tensor &t, bool upper) const;
};

// Helper functions

/// Print to an output stream
std::ostream &operator<<(std::ostream &os, const SymbolicTerm &term);

std::ostream &operator<<(std::ostream &os,
                         const std::pair<SymbolicTerm, scalar_t> &term);

#endif // _wicked_symbolic_term_h_
