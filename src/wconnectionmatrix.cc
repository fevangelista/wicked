#include <iostream>

#include "wconnectionmatrix.h"

WConnectionMatrix::WConnectionMatrix()
    : data_(std::vector<short>(maxop_ * maxop_ * ntype_ * nspace_, 0)) {

  //  int k = 0;
  //  for (int i = 0; i < maxop_; i++) {
  //    for (int j = 0; j < maxop_; j++) {
  //      for (int t = 0; t < ntype_; t++) {
  //        for (int s = 0; s < nspace_; s++) {
  //          set(i, j, t, s, k);
  //          k++;
  //        }
  //      }
  //    }
  //  }
}

int WConnectionMatrix::get(int i, int j, int type, int space) {
  int I = i * ntype_ + type;
  int J = j * nspace_ + space;
  return static_cast<int>(data_[I * maxop_ * nspace_ + J]);
}

void WConnectionMatrix::set(int i, int j, int type, int space, int value) {
  int I = i * ntype_ + type;
  int J = j * nspace_ + space;
  std::cout << I << " " << J << " " << I * maxop_ * nspace_ + J << std::endl;
  data_[I * maxop_ * nspace_ + J] = value;
}

std::string WConnectionMatrix::str() {
  std::string str;
  for (int i = 0; i < maxop_; i++) {
    for (int t = 0; t < ntype_; t++) {
      for (int j = 0; j < maxop_; j++) {
        str += " ";
        for (int s = 0; s < nspace_; s++) {
          str += std::to_string(get(i, j, t, s));
          str += " ";
        }
      }
      str += "\n";
    }
    str += "\n";
  }
  return str;
}
