#include <cassert>
#include <numeric>

#include "graph_matrix.h"
#include "helpers/helpers.h"
#include "helpers/orbital_space.h"

using namespace std;

GraphMatrix::GraphMatrix() {}

GraphMatrix::GraphMatrix(const std::vector<int> &cre,
                         const std::vector<int> &ann) {
  for (int i = 0; i < orbital_subspaces->num_spaces(); i++) {
    elements_[i] = std::make_pair(cre[i], ann[i]);
  }
}

const GraphMatrix::graph_matrix_t &GraphMatrix::elements() const {
  return elements_;
}

const std::pair<int, int> &GraphMatrix::elements(int space) const {
  return elements_[space];
}

int GraphMatrix::cre(int space) const { return elements_[space].first; }

int GraphMatrix::ann(int space) const { return elements_[space].second; }

void GraphMatrix::set_cre(int space, int value) {
  elements_[space].first = value;
}

void GraphMatrix::set_ann(int space, int value) {
  elements_[space].second = value;
}

int GraphMatrix::num_ops(int space) const { return cre(space) + ann(space); }

int GraphMatrix::num_ops() const {
  int r = 0;
  for (const auto &pair : elements_) {
    r += pair.first + pair.second;
  }
  return r;
}

bool GraphMatrix::operator<(GraphMatrix const &other) const {
  return elements_ < other.elements_;
}

bool GraphMatrix::operator==(GraphMatrix const &other) const {
  return elements_ == other.elements_;
}

bool GraphMatrix::operator!=(GraphMatrix const &other) const {
  return elements_ != other.elements_;
}

GraphMatrix &GraphMatrix::operator+=(const GraphMatrix &rhs) {
  for (int s = 0; s < orbital_subspaces->num_spaces(); ++s) {
    elements_[s].first += rhs.elements_[s].first;
    elements_[s].second += rhs.elements_[s].second;
  }
  return *this;
}

GraphMatrix &GraphMatrix::operator-=(const GraphMatrix &rhs) {
  for (int s = 0; s < orbital_subspaces->num_spaces(); ++s) {
    elements_[s].first -= rhs.elements_[s].first;
    elements_[s].second -= rhs.elements_[s].second;
  }
  return *this;
}

GraphMatrix GraphMatrix::adjoint() const {
  std::vector<int> cre_v(orbital_subspaces->num_spaces(), 0);
  std::vector<int> ann_v(orbital_subspaces->num_spaces(), 0);
  for (int s = 0; s < orbital_subspaces->num_spaces(); ++s) {
    cre_v[s] = ann(s);
    ann_v[s] = cre(s);
  }
  return GraphMatrix(cre_v, ann_v);
}

std::string GraphMatrix::str() const {
  // std::vector<std::string> cv, av;
  // for (int s = 0; s < orbital_subspaces->num_spaces(); ++s) {
  //   cv.push_back(to_string(cre(s)));
  // }
  // for (int s = 0; s < orbital_subspaces->num_spaces(); ++s) {
  //   av.push_back(to_string(ann(s)));
  // }
  // return "[" + join(cv, " ") + "|" + join(av, " ") + "]";

  std::vector<std::string> cv, av;
  for (int s = 0; s < orbital_subspaces->num_spaces(); ++s) {
    for (int i = 0; i < cre(s); ++i) {
      std::string op_s(1, orbital_subspaces->label(s));
      cv.push_back(op_s + "+");
    }
  }
  for (int s = 0; s < orbital_subspaces->num_spaces(); ++s) {
    for (int i = 0; i < ann(s); ++i) {
      std::string op_s(1, orbital_subspaces->label(s));
      cv.push_back(op_s);
    }
  }
  return join(cv, " ");
}

std::ostream &operator<<(std::ostream &os, const GraphMatrix &v) {
  os << v.str();
  return os;
}

int sum_num_ops(const std::vector<GraphMatrix> &graph_matrices) {
  return std::accumulate(graph_matrices.begin(), graph_matrices.end(), 0,
                         [&](int a, const auto &b) { return a + b.num_ops(); });
}

std::string to_string(const std::vector<GraphMatrix> &elements_vec) {
  // print the creation operator above
  std::vector<std::string> lines;
  for (int s = 0; s < orbital_subspaces->num_spaces(); ++s) {
    std::string line;
    for (const auto &graph_matrix : elements_vec) {
      line += std::to_string(graph_matrix.cre(s)) + " " +
              std::to_string(graph_matrix.ann(s)) + "   ";
    }
    lines.push_back(line);
  }
  return join(lines, "\n");
}

std::string signature(const GraphMatrix &graph_matrix) {
  std::string str;
  for (int s = 0; s < orbital_subspaces->num_spaces(); ++s) {
    str += std::to_string(graph_matrix.cre(s));
  }
  for (int s = 0; s < orbital_subspaces->num_spaces(); ++s) {
    str += to_string(graph_matrix.ann(s));
  }
  return str;
}

std::string signature(const std::vector<GraphMatrix> &elements_vec) {
  std::string s;
  for (const auto &graph_matrix : elements_vec) {
    s += signature(graph_matrix);
  }
  return s;
}
