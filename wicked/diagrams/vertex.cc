#include <cassert>

#include "helpers/helpers.h"
#include "helpers/orbital_space.h"
#include "vertex.h"

using namespace std;

Vertex::Vertex() {}

Vertex::Vertex(const std::vector<int> &cre, const std::vector<int> &ann) {
  for (int i = 0; i < osi->num_spaces(); i++) {
    vertex_[i] = std::make_pair(cre[i], ann[i]);
  }
}

const Vertex::vertex_t &Vertex::vertex() const { return vertex_; }

const std::pair<int, int> &Vertex::vertex(int space) const {
  return vertex_[space];
}

int Vertex::cre(int space) const { return vertex_[space].first; }

int Vertex::ann(int space) const { return vertex_[space].second; }

void Vertex::set_cre(int space, int value) { vertex_[space].first = value; }

void Vertex::set_ann(int space, int value) { vertex_[space].second = value; }

int Vertex::num_ops(int space) const { return cre(space) + ann(space); }

int Vertex::num_ops() const {
  int r = 0;
  for (const auto &pair : vertex_) {
    r += pair.first + pair.second;
  }
  return r;
}

bool Vertex::operator<(Vertex const &other) const {
  return vertex_ < other.vertex_;
}

bool Vertex::operator==(Vertex const &other) const {
  return vertex_ == other.vertex_;
}

bool Vertex::operator!=(Vertex const &other) const {
  return vertex_ != other.vertex_;
}

Vertex &Vertex::operator+=(const Vertex &rhs) {
  for (int s = 0; s < osi->num_spaces(); ++s) {
    vertex_[s].first += rhs.vertex_[s].first;
    vertex_[s].second += rhs.vertex_[s].second;
  }
  return *this;
}

Vertex &Vertex::operator-=(const Vertex &rhs) {
  for (int s = 0; s < osi->num_spaces(); ++s) {
    vertex_[s].first -= rhs.vertex_[s].first;
    vertex_[s].second -= rhs.vertex_[s].second;
  }
  return *this;
}

Vertex Vertex::adjoint() const {
  std::vector<int> cre_v(osi->num_spaces(), 0);
  std::vector<int> ann_v(osi->num_spaces(), 0);
  for (int s = 0; s < osi->num_spaces(); ++s) {
    cre_v[s] = ann(s);
    ann_v[s] = cre(s);
  }
  return Vertex(cre_v, ann_v);
}

std::string Vertex::str() const {
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

std::ostream &operator<<(std::ostream &os, const Vertex &v) {
  os << v.str();
  return os;
}

int sum_num_ops(const std::vector<Vertex> &vertices) {
  int n = 0;
  for (const auto &vertex : vertices) {
    n += vertex.num_ops();
  }
  return n;
}

std::string to_string(const std::vector<Vertex> &vertex_vec) {
  // print the creation operator above
  std::vector<std::string> lines;
  for (int s = 0; s < osi->num_spaces(); ++s) {
    std::string line;
    for (const auto &vertex : vertex_vec) {
      line += std::to_string(vertex.cre(s)) + " " +
              std::to_string(vertex.ann(s)) + "   ";
    }
    lines.push_back(line);
  }
  return join(lines, "\n");
}

std::string signature(const Vertex &vertex) {
  std::string str;
  for (int s = 0; s < osi->num_spaces(); ++s) {
    str += std::to_string(vertex.cre(s));
  }
  for (int s = 0; s < osi->num_spaces(); ++s) {
    str += to_string(vertex.ann(s));
  }
  return str;
}

std::string signature(const std::vector<Vertex> &vertex_vec) {
  std::string s;
  for (const auto &vertex : vertex_vec) {
    s += signature(vertex);
  }
  return s;
}
