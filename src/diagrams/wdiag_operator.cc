#include "helpers.h"
#include "orbital_space.h"
#include "wdiag_operator.h"

using namespace std;

WDiagOperator::WDiagOperator(const std::string &label,
                             const std::vector<int> &cre,
                             const std::vector<int> &ann)
    : label_(label), vertex_(cre, ann) {}

const std::string &WDiagOperator::label() const { return label_; }

WDiagVertex WDiagOperator::vertex() const {return vertex_;}

int WDiagOperator::num_indices(int space, bool creation) const {
  return creation ? vertex_.cre(space) : vertex_.ann(space);
}

int WDiagOperator::num_cre(int space) const { return vertex_.cre(space); }

int WDiagOperator::num_ann(int space) const { return vertex_.ann(space); }

int WDiagOperator::rank() const {
  return vertex_.rank();
}

std::string WDiagOperator::str() const {
  std::string s = label_;

  std::vector<std::string> cv, av;
  for (int s = 0; s < osi->num_spaces(); ++s) {
    cv.push_back(to_string(num_cre(s)));
  }
  for (int s = 0; s < osi->num_spaces(); ++s) {
    av.push_back(to_string(num_ann(s)));
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

int operators_rank(const std::vector<WDiagOperator> &ops)
{
    int r = 0;
    for (const auto& op : ops){
        r += op.rank();
    }
    return r;
}
