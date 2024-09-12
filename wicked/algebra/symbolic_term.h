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

  /// Set the tensors and operators
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

  // /// @brief Return a normal ordered version of this term
  // Expression normal_ordered() const;

  /// @return is the SQ operator product normal ordered?
  bool is_vacuum_normal_ordered() const;

  /// @return is the SQ operator product sorted in such a way that creation
  /// operators are to the left of annihilation operators?
  bool is_creation_then_annihilation() const;

  /// @brief Check if the product of operators is normal ordered with respect
  /// to the vacuum. The result of this function is independent of the value
  /// of the normal_ordered_ flag.
  bool is_labeled_normal_ordered() const;

  /// Canonicalize this term and return the overall phase factor
  // bool is_connected();

  scalar_t simplify();

  /// Comparison operator used for sorting
  bool operator<(const SymbolicTerm &term) const;

  /// Comparison operator used for sorting
  bool operator==(const SymbolicTerm &term) const;

  /// @brief Multiplication assignment operator
  /// @param rhs the term to multiply by
  SymbolicTerm &operator*=(const SymbolicTerm &rhs);

  /// Return a string representation
  std::string str() const;

  /// Return a LaTeX representation
  std::string latex() const;

  /// Return a compilable representation
  std::string compile(const std::string &format) const;

  // /// Return a normal ordered version of this term
  // std::vector<std::pair<SymbolicTerm, scalar_t>>
  // normal_order(bool only_same_index_contractions = false) const;

  /// Return a normal ordered version of this term
  std::vector<std::pair<SymbolicTerm, scalar_t>>
  vacuum_normal_order(bool only_same_index_contractions = false) const;

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

/// @brief Return the product of two symbolic terms
/// @param lhs the left-hand side term
/// @param rhs the right-hand side term
SymbolicTerm operator*(const SymbolicTerm lhs, const SymbolicTerm &rhs);

/// Print to an output stream
std::ostream &operator<<(std::ostream &os, const SymbolicTerm &term);

std::ostream &operator<<(std::ostream &os,
                         const std::pair<SymbolicTerm, scalar_t> &term);

std::pair<Product<SQOperator>, bool> operator_product(const SymbolicTerm &lhs,
                                                      const SymbolicTerm &rhs);

#endif // _wicked_symbolic_term_h_
