#ifndef _wicked_diag_vertex_h_
#define _wicked_diag_vertex_h_

#include <vector>
#include <string>

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

  /// Return a string representation of the operator
  std::string str() const;

  /// Write a string representation of the operator to a stream
  friend std::ostream &operator<<(std::ostream &os, const WDiagVertex &v);

private:
  /// The number of creation/annihilation operators in each space
  std::vector<std::pair<int, int>> vertex_;
};

#endif // _wicked_diag_vertex_h_
