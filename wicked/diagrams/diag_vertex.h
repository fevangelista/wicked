#ifndef _wicked_diag_vertex_h_
#define _wicked_diag_vertex_h_

#include <string>
#include <vector>

#include "../wicked-def.h"

/// A class to represent a collection of creation and annilation
/// operators, which we call a diagram vertex
class DiagVertex {

public:
  /// Constructor
  DiagVertex();

  /// Constructor. Set number of creation and annihilation operators
  DiagVertex(const std::vector<int> &cre, const std::vector<int> &ann);

  /// Return a vector of pairs of creation/annihilation operators
  const std::vector<std::pair<int, int>> &vertex() const;

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

  /// Return the particle rank of this vertex (sum creation + sum annilation)
  int rank() const;

  /// Comparison operator used for sorting
  bool operator<(DiagVertex const &other) const;

  // Adds the operator count of another object
  DiagVertex &operator+=(const DiagVertex &rhs);

  // Subtracts the operator count of another object
  DiagVertex &operator-=(const DiagVertex &rhs);

  /// Return a string representation of the operator
  std::string str() const;

  /// Write a string representation of the operator to a stream
  friend std::ostream &operator<<(std::ostream &os, const DiagVertex &v);

private:
  /// The number of creation/annihilation operators in each space
  std::vector<std::pair<int, int>> vertex_;
};

// Helper functions

/// Return the particle rank of a vector of operators
int vertices_rank(const std::vector<DiagVertex> &vertices);

/// Return the space for which the number of creation/annihilation operators !=
/// 0
int vertices_space(const std::vector<DiagVertex> &vertices);

/// Return a nice string representation of a vector of vertices
std::string to_string(const std::vector<DiagVertex> &vertex_vec);

std::string signature(const DiagVertex &vertex);

std::string signature(const std::vector<DiagVertex> &vertex_vec);

#endif // _wicked_diag_vertex_h_
