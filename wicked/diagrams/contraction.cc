#include "contraction.h"

#include "helpers/orbital_space.h"
#include "vertex.h"

int ElementaryContraction::num_ops() const {
  int n = 0;
  for (const auto &vertex : elements_) {
    n += vertex.num_ops();
  }
  return n;
}

std::vector<int> ElementaryContraction::spaces_in_vertices() const {
  std::vector<int> vec;
  for (const auto &vertex : elements_) {
    for (int s = 0; s < osi->num_spaces(); ++s) {
      if (vertex.ann(s) + vertex.cre(s) > 0) {
        vec.push_back(s);
      }
    }
  }
  return vec;
}
