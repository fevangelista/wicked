#ifndef _wicked_diag_vertex_h_
#define _wicked_diag_vertex_h_

#include <string>
#include <vector>

#include "wicked-def.h"

/// A class to represent operators
class WDiagVertex {

public:
  /// Constructor. Set number of creation and annihilation operators
  WDiagVertex();

  /// Constructor. Set number of creation and annihilation operators
  WDiagVertex(const std::vector<int> &cre, const std::vector<int> &ann);

  /// Return the number of creation operators in space
  int cre(int space) const;

  /// Return the number of annihilation operators in space
  int ann(int space) const;

  /// Set the number of creation operators in space
  void cre(int space, int value);

  /// Set the number of annihilation operators in space
  void ann(int space, int value);

  /// Return the particle rank of this vertex (sum creation + sum annilation)
  int rank() const;

  /// Comparison operator used for sorting
  bool operator<(WDiagVertex const &other) const;

  // compound assignment
  WDiagVertex &operator+=(const WDiagVertex &rhs);

  /// Return a string representation of the operator
  std::string str() const;

  /// Write a string representation of the operator to a stream
  friend std::ostream &operator<<(std::ostream &os, const WDiagVertex &v);

private:
  /// The number of creation/annihilation operators in each space
  std::vector<std::pair<int, int>> vertex_;
};

// Helper functions

/// Return the particle rank of a vector of operators
int vertices_rank(const std::vector<WDiagVertex> &vertices);

/// Return the space for which the number of creation/annihilation operators !=
/// 0
int vertices_space(const std::vector<WDiagVertex> &vertices);

/// Return a nice string representation of a vector of vertices
std::string to_string(const std::vector<WDiagVertex> &vertex_vec);

/// Comparison operator used for sorting
bool operator<(std::vector<WDiagVertex> const &rhs,
               std::vector<WDiagVertex> const &lhs);

std::string signature(const WDiagVertex& vertex);

std::string signature(const std::vector<WDiagVertex>& vertex_vec);

#endif // _wicked_diag_vertex_h_
