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

  // /// Constructor. Set number of creation and annihilation operators
  // CompositeContraction(const std::vector<Vertex> &vertices);

  //   void push_back(const ElementaryContraction &c);

  //   ElementaryContraction &operator[](size_t n);
  //   const ElementaryContraction &operator[](size_t n) const;

  //   /// The number of elementary contractions
  //   size_t size() const;

  //   std::vector<ElementaryContraction>::iterator begin() {
  //     return contractions_.begin();
  //   }
  //   // overloaded on the constness of "this"
  //   std::vector<ElementaryContraction>::const_iterator begin() const {
  //     return contractions_.begin();
  //   }
  //   std::vector<ElementaryContraction>::iterator end() {
  //     return contractions_.end();
  //   }
  //   // overloaded on the constness of "this"
  //   std::vector<ElementaryContraction>::const_iterator end() const {
  //     return contractions_.end();
  //   }

  // private:
  //   /// This object stores a vector of Vertex objects
  //   std::vector<ElementaryContraction> contractions_;
};

#endif // _wicked_contraction_h_
