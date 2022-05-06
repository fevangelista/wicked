#ifndef _wicked_diag_vertex_h_
#define _wicked_diag_vertex_h_

#include <array>
#include <string>
#include <vector>

#include "../wicked-def.h"

/// A class to represent a collection of creation and annilation
/// operators, which we call a diagram vertex
class Vertex {
  // Here we use an optimized way to store the vertex
  // We assume that we work at most with 8 different spaces, which
  // should be enough for virtually any type of application
  static constexpr int max_spaces_ = 8;
  using vertex_t = std::array<std::pair<int, int>, max_spaces_>;

public:
  /// Constructor
  Vertex();

  /// Constructor. Set number of creation and annihilation operators
  Vertex(const std::vector<int> &cre, const std::vector<int> &ann);

  /// Return a vector of pairs of creation/annihilation operators
  const vertex_t &vertex() const;

  /// Return a pair of creation/annihilation operators in space
  const std::pair<int, int> &vertex(int space) const;

  /// Return the number of creation operators in space
  int cre(int space) const;

  /// Return the number of annihilation operators in space
  int ann(int space) const;

  /// Set the number of creation operators in space
  void set_cre(int space, int value);

  /// Set the number of annihilation operators in space
  void set_ann(int space, int value);

  /// Return the number of creation + annilation operators represented by this
  /// vertex
  int num_ops() const;

  /// Return the number of creation + annilation operators represented by this
  /// vertex in a given space
  int num_ops(int space) const;

  /// Comparison operators used for sorting
  bool operator<(Vertex const &other) const;
  bool operator==(Vertex const &other) const;
  bool operator!=(Vertex const &other) const;

  // Adds the operator count of another object
  Vertex &operator+=(const Vertex &rhs);

  // Subtracts the operator count of another object
  Vertex &operator-=(const Vertex &rhs);

  // Return the hermitian conjugate of this operator
  Vertex adjoint() const;

  /// Return a string representation of the operator
  std::string str() const;

  /// Write a string representation of the operator to a stream
  friend std::ostream &operator<<(std::ostream &os, const Vertex &v);

private:
  /// This object stores the number of creation/annihilation
  /// operators in each space. This initializes it to all zeros
  vertex_t vertex_ = {};
};

// Helper functions

/// Return the sum of the number creation and annihilation operator for a vector
/// of vertices
int sum_num_ops(const std::vector<Vertex> &vertices);

/// Return a nice string representation of a vector of vertices
std::string to_string(const std::vector<Vertex> &vertex_vec);

std::string signature(const Vertex &vertex);

std::string signature(const std::vector<Vertex> &vertex_vec);

#endif // _wicked_diag_vertex_h_
