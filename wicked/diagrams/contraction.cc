#import "contraction.h"
#import "../orbital_space.h"
#import "diag_vertex.h"

ElementaryContraction::ElementaryContraction() {}

ElementaryContraction::ElementaryContraction(
    const std::vector<DiagVertex> &vertices)
    : vertices_(vertices) {}

DiagVertex &ElementaryContraction::operator[](size_t n) { return vertices_[n]; }

const DiagVertex &ElementaryContraction::operator[](size_t n) const {
  return vertices_[n];
}

size_t ElementaryContraction::size() const { return vertices_.size(); }

int ElementaryContraction::num_ops() const {
  int n = 0;
  for (const auto &vertex : vertices_) {
    n += vertex.num_ops();
  }
  return n;
}

bool ElementaryContraction::operator<(
    ElementaryContraction const &other) const {
  return vertices_ < other.vertices_;
}

std::vector<int> ElementaryContraction::spaces_in_vertices() const {
  std::vector<int> vec;
  for (const auto &vertex : vertices_) {
    for (int s = 0; s < osi->num_spaces(); ++s) {
      if (vertex.ann(s) + vertex.cre(s) > 0) {
        vec.push_back(s);
      }
    }
  }
  return vec;
}

CompositeContraction::CompositeContraction() {}

size_t CompositeContraction::size() const { return contractions_.size(); }

ElementaryContraction &CompositeContraction::operator[](size_t n) {
  return contractions_[n];
}

const ElementaryContraction &CompositeContraction::operator[](size_t n) const {
  return contractions_[n];
}

void CompositeContraction::push_back(const ElementaryContraction &c) {
  contractions_.push_back(c);
}