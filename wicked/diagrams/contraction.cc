#include <numeric>

#include "contraction.h"

#include "helpers/orbital_space.h"

int ElementaryContraction::num_ops() const {
  return std::accumulate(elements_.begin(), elements_.end(), 0,
                         [&](int a, const auto &b) { return a + b.num_ops(); });
}

std::vector<int>
ElementaryContraction::spaces_in_elementary_contraction() const {
  std::vector<int> vec;
  for (const auto &graph_matrix : elements_) {
    for (int s = 0; s < orbital_subspaces->num_spaces(); ++s) {
      if (graph_matrix.ann(s) + graph_matrix.cre(s) > 0) {
        vec.push_back(s);
      }
    }
  }
  return vec;
}
