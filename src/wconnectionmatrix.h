#ifndef _wicked_connection_matrix_h_
#define _wicked_connection_matrix_h_

#include <vector>
#include <string>

/// A class to contract a product of operators
class WConnectionMatrix {

public:
  /// Constructor
  WConnectionMatrix();

  /// Get the value of an element
  int get(int i, int j, int type, int space);

  /// Set the value of an element
  void set(int i, int j, int type, int space, int value);

  /// Produce a string representation
  std::string str();

private:
  static const short maxop_ = 4;
  static const short ntype_ = 2;
  static const short nspace_ = 3;
  std::vector<short> data_;
};

#endif // _wicked_connection_matrix_h_
