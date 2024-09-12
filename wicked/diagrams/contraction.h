#pragma once

#include <string>
#include <vector>

#include "../wicked-def.h"
#include "graph_matrix.h"
#include "helpers/product.hpp"

/// A class to represent an elementary contraction
class ElementaryContraction : public Product<GraphMatrix> {
public:
  /// Constructor
  ElementaryContraction() : Product<GraphMatrix>() {}

  /// Constructor. Set number of creation and annihilation operators
  ElementaryContraction(const std::vector<GraphMatrix> &graph_matrix)
      : Product<GraphMatrix>(graph_matrix) {}

  /// The number of second quantization operator contracted
  int num_ops() const;

  std::vector<int> spaces_in_elementary_contraction() const;
};

/// A class to represent an elementary contraction
class CompositeContraction : public Product<ElementaryContraction> {
public:
  /// Constructor
  CompositeContraction() : Product<ElementaryContraction>() {}
};
