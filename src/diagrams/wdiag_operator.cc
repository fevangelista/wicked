#include "helpers.h"
#include "orbital_space.h"
#include "wdiag_operator.h"

using namespace std;

WDiagOperator::WDiagOperator(const std::string &label,
                             const std::vector<int> &cre,
                             const std::vector<int> &ann)
    : label_(label), ops_(std::vector<std::pair<int, int>>(osi->num_spaces())) {
  for (int i = 0; i < osi->num_spaces(); i++) {
    ops_[i] = std::make_pair(cre[i], ann[i]);
  }
}

std::string &WDiagOperator::label() { return label_; }

int WDiagOperator::num_indices(int space, bool creation) const {
  return creation ? ops_[space].first : ops_[space].second;
}

std::string WDiagOperator::str() const {
  std::string s = label_;

  std::vector<std::string> cv, av;
  for (int i = 0; i < osi->num_spaces(); ++i) {
    cv.push_back(to_string(num_indices(i, true)));
  }
  for (int i = 0; i < osi->num_spaces(); ++i) {
    av.push_back(to_string(num_indices(i, false)));
  }

  s += " [" + to_string(cv, " ") + "|" + to_string(av, " ") + "] (";
  for (int i = 0; i < osi->num_spaces(); ++i) {
    for (int j = 0; j < num_indices(i, false); j++)
      s += osi->label(i);
  }
  s += " -> ";
  for (int i = 0; i < osi->num_spaces(); ++i) {
    for (int j = 0; j < num_indices(i, true); j++)
      s += osi->label(i);
  }
  s += ")";
  return s;
}

std::ostream &operator<<(std::ostream &os, const WDiagOperator &op) {
  os << op.str();
  return os;
}

WDiagOperator make_diag_operator(const std::string &label,
                                 const std::vector<std::string> &cre_labels,
                                 const std::vector<std::string> &ann_labels) {
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

  return WDiagOperator(label, cre, ann);
}
