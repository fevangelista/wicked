#pragma once

#include <array>
#include <string>
#include <vector>

#include "../wicked-def.h"

/// A class to keep track of creation and annilation operators,
/// and their contractions, which we call a graph matrix.
/// This object stores a vector of pairs of number for each space
class GraphMatrix {
  // Here we use an optimized way to store the graph matrix
  // We assume that we work at most with 8 different spaces, which
  // should be enough for virtually any type of application
  static constexpr int max_spaces_ = 8;
  using graph_matrix_t = std::array<std::pair<int, int>, max_spaces_>;

public:
  /// Constructor
  GraphMatrix();

  /// Constructor. Set number of creation and annihilation operators
  GraphMatrix(const std::vector<int> &cre, const std::vector<int> &ann);

  /// Return a vector of pairs of creation/annihilation operators
  const graph_matrix_t &elements() const;

  /// Return a pair of creation/annihilation operators in space
  const std::pair<int, int> &elements(int space) const;

  /// Return the number of creation operators in space
  int cre(int space) const;

  /// Return the number of annihilation operators in space
  int ann(int space) const;

  /// Set the number of creation operators in space
  void set_cre(int space, int value);

  /// Set the number of annihilation operators in space
  void set_ann(int space, int value);

  /// Return the number of creation + annilation operators represented by this
  /// graph matrix
  int num_ops() const;

  /// Return the number of creation + annilation operators represented by this
  /// graph matrix in a given space
  int num_ops(int space) const;

  /// Comparison operators used for sorting
  bool operator<(GraphMatrix const &other) const;
  bool operator==(GraphMatrix const &other) const;
  bool operator!=(GraphMatrix const &other) const;

  // Adds the operator count of another object
  GraphMatrix &operator+=(const GraphMatrix &rhs);

  // Subtracts the operator count of another object
  GraphMatrix &operator-=(const GraphMatrix &rhs);

  // Return the hermitian conjugate of this operator
  GraphMatrix adjoint() const;

  /// Return a string representation of the operator
  std::string str() const;

  /// Write a string representation of the operator to a stream
  friend std::ostream &operator<<(std::ostream &os, const GraphMatrix &v);

private:
  /// This object stores the number of creation/annihilation
  /// operators in each space. This initializes it to all zeros
  graph_matrix_t elements_ = {};
};

// Helper functions

/// Return the sum of the number creation and annihilation operator for a vector
/// of graph matrices
int sum_num_ops(const std::vector<GraphMatrix> &graph_matrix);

/// Return a nice string representation of a vector of graph matrices
std::string to_string(const std::vector<GraphMatrix> &elements_vec);

std::string signature(const GraphMatrix &graph_matrix);

std::string signature(const std::vector<GraphMatrix> &elements_vec);
