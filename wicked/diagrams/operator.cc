#include "helpers/combinatorics.h"
#include "helpers/helpers.h"
#include "helpers/orbital_space.h"

#include "operator.h"

using namespace std;

Operator::Operator(const std::string &label, const std::vector<int> &cre,
                   const std::vector<int> &ann)
    : label_(label), graph_matrix_(cre, ann) {}

Operator::Operator(const std::string &label, const GraphMatrix &graph_matrix)
    : label_(label), graph_matrix_(graph_matrix) {}

const std::string &Operator::label() const { return label_; }

GraphMatrix Operator::graph_matrix() const { return graph_matrix_; }

scalar_t Operator::factor() const {
  scalar_t result = 1;
  for (int s = 0; s < orbital_subspaces->num_spaces(); ++s) {
    result /= static_cast<scalar_t>(factorial(cre(s)));
  }
  for (int s = 0; s < orbital_subspaces->num_spaces(); ++s) {
    result /= static_cast<scalar_t>(factorial(ann(s)));
  }
  return result;
}

Operator Operator::adjoint() const {
  return Operator(label(), graph_matrix().adjoint());
}

int Operator::cre(int space) const { return graph_matrix_.cre(space); }

int Operator::ann(int space) const { return graph_matrix_.ann(space); }

int Operator::num_ops() const { return graph_matrix_.num_ops(); }

bool Operator::operator<(Operator const &other) const {
  // Compare the labels
  if (label_ < other.label_)
    return true;
  if (label_ > other.label_)
    return false;
  // Compare the graph matrices
  return graph_matrix_ < other.graph_matrix_;
}

bool Operator::operator==(Operator const &other) const {
  return ((label_ == other.label_) and (graph_matrix_ == other.graph_matrix_));
}

bool Operator::operator!=(Operator const &other) const {
  return ((label_ != other.label_) or (graph_matrix_ != other.graph_matrix_));
}

std::string Operator::str() const {
  std::vector<std::string> s;
  if (factor() != scalar_t(1)) {
    s.push_back(factor().str(false));
  }
  s.push_back(label_);
  s.push_back("{");
  for (int i = 0; i < orbital_subspaces->num_spaces(); ++i) {
    for (int j = 0; j < cre(i); j++) {
      std::string op_s(1, orbital_subspaces->label(i));
      s.push_back(op_s + "+");
    }
  }

  for (int i = orbital_subspaces->num_spaces() - 1; i >= 0; --i) {
    for (int j = 0; j < ann(i); j++)
      s.push_back(std::string(1, orbital_subspaces->label(i)));
  }

  s.push_back("}");

  return join(s, " ");
}

std::ostream &operator<<(std::ostream &os, const Operator &op) {
  os << op.str();
  return os;
}

bool do_operators_commute(const Operator &a, const Operator &b) {
  int noncommuting = 0;
  for (int s = 0; s < orbital_subspaces->num_spaces(); s++) {
    noncommuting += a.ann(s) * b.cre(s) + a.cre(s) * b.ann(s);
  }
  return noncommuting == 0;
}

bool operator_noncommuting_less(const Operator &a, const Operator &b) {
  // if the operators commute return normal less
  if (do_operators_commute(a, b)) {
    return (a < b);
  }
  return false;
}

Operator make_diag_operator(const std::string &label,
                            const std::vector<char> &cre_labels,
                            const std::vector<char> &ann_labels) {
  // count the number of creation and annihilation operators in each space
  std::vector<int> cre(orbital_subspaces->num_spaces());
  std::vector<int> ann(orbital_subspaces->num_spaces());
  for (const auto &l : cre_labels) {
    int space = orbital_subspaces->label_to_space(l);
    cre[space] += 1;
  }
  for (const auto &l : ann_labels) {
    int space = orbital_subspaces->label_to_space(l);
    ann[space] += 1;
  }

  return Operator(label, cre, ann);
}

int sum_num_ops(const std::vector<Operator> &ops) {
  int r = 0;
  for (const auto &op : ops) {
    r += op.num_ops();
  }
  return r;
}
