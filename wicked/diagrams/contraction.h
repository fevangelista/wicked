#ifndef _wicked_contraction_h_
#define _wicked_contraction_h_

#include <string>
#include <vector>

#include "../product.hpp"
#include "../wicked-def.h"

class Vertex;

/// A class to represent an elementary contraction
class ElementaryContraction : public Product<Vertex> {
public:
  /// Constructor
  ElementaryContraction() : Product<Vertex>() {}

  /// Constructor. Set number of creation and annihilation operators
  ElementaryContraction(const std::vector<Vertex> &vertices)
      : Product<Vertex>(vertices) {}

  /// The number of second quantization operator contracted
  int num_ops() const;

  std::vector<int> spaces_in_vertices() const;
};

/// A class to represent an elementary contraction
class CompositeContraction : public Product<ElementaryContraction> {
public:
  /// Constructor
  CompositeContraction() : Product<ElementaryContraction>() {}
};

#endif // _wicked_contraction_h_
