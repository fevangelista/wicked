#include "helpers.h"
#include "orbital_space.h"
#include "wdiag_operator.h"

using namespace std;

WDiagOperator::WDiagOperator(const std::string &label, const std::vector<int> &ann,
                     const std::vector<int> &cre)
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
