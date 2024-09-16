#include "operator_product.h"

using namespace std;

int OperatorProduct::num_ops() const {
  int r = 0;
  for (const auto &op : elements_) {
    r += op.num_ops();
  }
  return r;
}

scalar_t OperatorProduct::canonicalize() {
  int nperm = 0;
  int n = elements_.size();
  for (int i = 0; i < n - 1; i++) {
    // Last i elements are already in place
    for (int j = 0; j < n - i - 1; j++) {
      if (operator_noncommuting_less(elements_[j + 1], elements_[j])) {
        nperm += elements_[j].num_ops() * elements_[j + 1].num_ops();
        swap(elements_[j], elements_[j + 1]);
      }
    }
  }
  return nperm % 2 == 0 ? scalar_t(1, 1) : scalar_t(-1, 1);
}

OperatorProduct operator*(const OperatorProduct &l, const OperatorProduct &r) {
  OperatorProduct result;
  for (const auto &e : l) {
    result.push_back(e);
  }
  for (const auto &e : r) {
    result.push_back(e);
  }
  return result;
}
