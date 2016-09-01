#include "orbital_space.h"
#include "helpers.h"
#include "wdiag_vertex.h"

using namespace std;

WDiagVertex::WDiagVertex()
    : vertex_(std::vector<std::pair<int, int>>(osi->num_spaces())) {
  for (int i = 0; i < osi->num_spaces(); i++) {
    vertex_[i] = std::make_pair(0, 0);
  }
}

WDiagVertex::WDiagVertex(const std::vector<int> &cre,
                         const std::vector<int> &ann)
    : vertex_(std::vector<std::pair<int, int>>(osi->num_spaces())) {
  for (int i = 0; i < osi->num_spaces(); i++) {
    vertex_[i] = std::make_pair(cre[i], ann[i]);
  }
}

int WDiagVertex::cre(int space) const { return vertex_[space].first; }

int WDiagVertex::ann(int space) const { return vertex_[space].second; }

void WDiagVertex::cre(int space, int value) { vertex_[space].first = value; }

void WDiagVertex::ann(int space, int value) { vertex_[space].second = value; }

int WDiagVertex::rank() const {
  int r = 0;
  for (const auto &pair : vertex_) {
    r += pair.first + pair.second;
  }
  return r;
}

WDiagVertex &WDiagVertex::operator+=(const WDiagVertex &rhs) {
  for (int s = 0; s < osi->num_spaces(); ++s) {
    vertex_[s].first += rhs.vertex_[s].first;
    vertex_[s].second += rhs.vertex_[s].second;
  }
  return *this;
}

std::string WDiagVertex::str() const {
  std::vector<std::string> cv, av;
  for (int s = 0; s < osi->num_spaces(); ++s) {
    cv.push_back(to_string(cre(s)));
  }
  for (int s = 0; s < osi->num_spaces(); ++s) {
    av.push_back(to_string(ann(s)));
  }
  return "[" + to_string(cv, " ") + "|" + to_string(av, " ") + "]";
}

std::ostream &operator<<(std::ostream &os, const WDiagVertex &v) {
  os << v.str();
  return os;
}

int vertices_rank(const std::vector<WDiagVertex> &vertices)
{
    int r = 0;
    for (const auto& vertex : vertices){
        r += vertex.rank();
    }
    return r;
}

