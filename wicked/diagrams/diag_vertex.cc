#include <cassert>

#include "diag_vertex.h"
#include "helpers.h"
#include "orbital_space.h"

using namespace std;

DiagVertex::DiagVertex()
    : vertex_(std::vector<std::pair<int, int>>(osi->num_spaces(),
                                               std::make_pair(0, 0))) {
  // for (int i = 0; i < osi->num_spaces(); i++) {
  //   vertex_[i] = std::make_pair(0, 0);
  // }
}

DiagVertex::DiagVertex(const std::vector<int> &cre, const std::vector<int> &ann)
    : vertex_(std::vector<std::pair<int, int>>(osi->num_spaces())) {
  for (int i = 0; i < osi->num_spaces(); i++) {
    vertex_[i] = std::make_pair(cre[i], ann[i]);
  }
}

const std::vector<std::pair<int, int>> &DiagVertex::vertex() const {
  return vertex_;
}

const std::pair<int, int> &DiagVertex::vertex(int space) const {
  return vertex_[space];
}

int DiagVertex::cre(int space) const { return vertex_[space].first; }

int DiagVertex::ann(int space) const { return vertex_[space].second; }

void DiagVertex::set_cre(int space, int value) { vertex_[space].first = value; }

void DiagVertex::set_ann(int space, int value) {
  vertex_[space].second = value;
}

int DiagVertex::rank() const {
  int r = 0;
  for (const auto &pair : vertex_) {
    r += pair.first + pair.second;
  }
  return r;
}

bool DiagVertex::operator<(DiagVertex const &other) const {
  return vertex_ < other.vertex_;
}

DiagVertex &DiagVertex::operator+=(const DiagVertex &rhs) {
  for (int s = 0; s < osi->num_spaces(); ++s) {
    vertex_[s].first += rhs.vertex_[s].first;
    vertex_[s].second += rhs.vertex_[s].second;
  }
  return *this;
}

DiagVertex &DiagVertex::operator-=(const DiagVertex &rhs) {
  for (int s = 0; s < osi->num_spaces(); ++s) {
    vertex_[s].first -= rhs.vertex_[s].first;
    vertex_[s].second -= rhs.vertex_[s].second;
  }
  return *this;
}

std::string DiagVertex::str() const {
  // std::vector<std::string> cv, av;
  // for (int s = 0; s < osi->num_spaces(); ++s) {
  //   cv.push_back(to_string(cre(s)));
  // }
  // for (int s = 0; s < osi->num_spaces(); ++s) {
  //   av.push_back(to_string(ann(s)));
  // }
  // return "[" + join(cv, " ") + "|" + join(av, " ") + "]";

  std::vector<std::string> cv, av;
  for (int s = 0; s < osi->num_spaces(); ++s) {
    for (int i = 0; i < cre(s); ++i) {
      std::string op_s(1, osi->label(s));
      cv.push_back(op_s + "+");
    }
  }
  for (int s = 0; s < osi->num_spaces(); ++s) {
    for (int i = 0; i < ann(s); ++i) {
      std::string op_s(1, osi->label(s));
      cv.push_back(op_s);
    }
  }
  return join(cv, " ");
}

std::ostream &operator<<(std::ostream &os, const DiagVertex &v) {
  os << v.str();
  return os;
}

int vertices_rank(const std::vector<DiagVertex> &vertices) {
  int r = 0;
  for (const auto &vertex : vertices) {
    r += vertex.rank();
  }
  return r;
}

int vertices_space(const std::vector<DiagVertex> &vertices) {
  std::vector<int> count(osi->num_spaces());
  for (const auto &vertex : vertices) {
    for (int s = 0; s < osi->num_spaces(); ++s) {
      count[s] += vertex.ann(s) + vertex.cre(s);
    }
  }
  for (int s = 0; s < osi->num_spaces(); ++s) {
    if (count[s] > 0) {
      return s;
    }
  }
  return -1;
}

std::string to_string(const std::vector<DiagVertex> &vertex_vec) {
  // print the creation operator above
  std::vector<std::string> cre_line, ann_line;
  for (const auto &vertex : vertex_vec) {
    for (int s = 0; s < osi->num_spaces(); ++s) {
      cre_line.push_back(to_string(vertex.cre(s)));
    }
    cre_line.push_back(" ");
  }
  // print the annihilation operator above
  for (const auto &vertex : vertex_vec) {
    for (int s = 0; s < osi->num_spaces(); ++s) {
      ann_line.push_back(to_string(vertex.ann(s)));
    }
    ann_line.push_back(" ");
  }
  return join(cre_line, " ") + "\n" + join(ann_line, " ");
}

std::string signature(const DiagVertex &vertex) {
  std::string str;
  for (int s = 0; s < osi->num_spaces(); ++s) {
    str += std::to_string(vertex.cre(s));
  }
  for (int s = 0; s < osi->num_spaces(); ++s) {
    str += to_string(vertex.ann(s));
  }
  return str;
}

std::string signature(const std::vector<DiagVertex> &vertex_vec) {
  std::string s;
  for (const auto &vertex : vertex_vec) {
    s += signature(vertex);
  }
  return s;
}
