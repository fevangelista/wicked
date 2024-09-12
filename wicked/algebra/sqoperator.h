#pragma once

#include <string>

#include "../helpers/product.hpp"
#include "helpers/orbital_space.h"
#include "index.h"

enum class SQOperatorType {
  Creation,
  Annihilation,
};

class SQOperator {

  using index_map_t = std::map<Index, Index>;

public:
  SQOperator(SQOperatorType type, Index index);
  ~SQOperator();

  /// @brief Compare two operators. This comparison is used to sort operators
  /// and places creation operators (with respect to the true vacuum) before
  /// annihilation operators.
  /// @param other the operator to compare to
  bool operator<(SQOperator const &other) const;
  bool operator==(SQOperator const &other) const;

  /// @brief Normal-ordered comparison of two operators. This comparison is used
  /// to define the normal ordering of a product of operators and it is
  /// different from the comparison based on the < operator (used to sort
  /// operators).
  /// @param other the operator to compare to
  bool normal_ordered_less(SQOperator const &other) const;

  /// @brief Return the type of this operator
  SQOperatorType type() const;

  /// @brief Return the space type of this operator
  SpaceType space_type() const;

  /// @return the symbol of the operator
  std::string op_symbol() const;

  /// @return is this a creation operator
  bool is_creation() const;

  /// @return is this a quasi-particle creation operator
  bool is_quasiparticle_creation() const;

  /// Return the type of this operator
  FieldType field_type() const;

  /// Return the index of this operator
  const Index &index() const;

  /// Return the space on which this operator acts
  int space() const;

  /// Reindex this operator
  void reindex(index_map_t &idx_map);

  /// Return a string representation
  std::string str() const;

  /// Return a LaTeX representation
  std::string latex() const;

  /// Return a compilable representation
  std::string compile(const std::string &format) const;

  /// Return the adjoint of this operator
  SQOperator adjoint() const;

  /// Return the scalar factor that multiplies the commutator of two operators
  scalar_t commutator_factor(const SQOperator &other) const;

private:
  std::pair<SQOperatorType, Index> operator_;
};

// Helper functions

/// Helper function to make an SQOperator object from an index and the type
SQOperator make_sqoperator(const std::string &index, SQOperatorType type);

/// Print to an output stream
std::ostream &operator<<(std::ostream &os, const SQOperator &op);

/// Canonicalize a product of operators
scalar_t canonicalize_sqops(Product<SQOperator> &sqops, bool reversed);
