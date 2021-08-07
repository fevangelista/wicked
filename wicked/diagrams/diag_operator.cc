#include "diag_operator.h"
#include "combinatorics.h"
#include "helpers.h"
#include "orbital_space.h"

using namespace std;

DiagOperator::DiagOperator(const std::string &label,
                           const std::vector<int> &cre,
                           const std::vector<int> &ann)
    : label_(label), vertex_(cre, ann) {}

const std::string &DiagOperator::label() const { return label_; }

DiagVertex DiagOperator::vertex() const { return vertex_; }

scalar_t DiagOperator::factor() const {
  scalar_t result = 1;
  for (int s = 0; s < osi->num_spaces(); ++s) {
    result /= static_cast<scalar_t>(factorial(cre(s)));
  }
  for (int s = 0; s < osi->num_spaces(); ++s) {
    result /= static_cast<scalar_t>(factorial(ann(s)));
  }
  return result;
}

int DiagOperator::cre(int space) const { return vertex_.cre(space); }

int DiagOperator::ann(int space) const { return vertex_.ann(space); }

int DiagOperator::rank() const { return vertex_.rank(); }

bool DiagOperator::operator<(DiagOperator const &other) const {
  // Compare the labels
  if (label_ < other.label_)
    return true;
  if (label_ > other.label_)
    return false;
  // Compare the vertices
  return vertex_ < other.vertex_;
}

std::string DiagOperator::str() const {
  std::vector<std::string> s;
  s.push_back(label_);
  s.push_back("{");
  for (int i = 0; i < osi->num_spaces(); ++i) {
    for (int j = 0; j < cre(i); j++) {
      std::string op_s(1, osi->label(i));
      s.push_back(op_s + "+");
    }
  }

  for (int i = osi->num_spaces() - 1; i >= 0; --i) {
    for (int j = 0; j < ann(i); j++)
      s.push_back(std::string(1, osi->label(i)));
  }

  s.push_back("}");

  return join(s, " ");
}

std::ostream &operator<<(std::ostream &os, const DiagOperator &op) {
  os << op.str();
  return os;
}

DiagOperator make_diag_operator(const std::string &label,
                                const std::vector<char> &cre_labels,
                                const std::vector<char> &ann_labels) {
  // count the number of creation and annihilation operators in each space
  std::vector<int> cre(osi->num_spaces());
  std::vector<int> ann(osi->num_spaces());
  for (const auto &label : cre_labels) {
    int space = osi->label_to_space(label);
    cre[space] += 1;
  }
  for (const auto &label : ann_labels) {
    int space = osi->label_to_space(label);
    ann[space] += 1;
  }

  return DiagOperator(label, cre, ann);
}

int operators_rank(const std::vector<DiagOperator> &ops) {
  int r = 0;
  for (const auto &op : ops) {
    r += op.rank();
  }
  return r;
}
