#include "wdiag_operator_set.h"

WDiagOperatorSum::WDiagOperatorSum() {}

WDiagOperatorSum::WDiagOperatorSum(WDiagOperator dop, scalar_t factor) {
  add({dop}, factor);
}

WDiagOperatorSum::WDiagOperatorSum(std::vector<WDiagOperator> vec_dop,
                                   scalar_t factor) {
  add(vec_dop, factor);
}

void WDiagOperatorSum::add(const WDiagOperator &dop, scalar_t factor) {
  add({dop}, factor);
}

void WDiagOperatorSum::add(const std::vector<WDiagOperator> &vec_dop,
                           scalar_t factor) {
  auto search = sum_.find(vec_dop);

  if (search != sum_.end()) {
    /// Found, then just add the factor to the existing term
    search->second += factor;
    if (search->second == 0) {
      sum_.erase(search);
    }
  } else {
    sum_[vec_dop] = factor;
  }
}

dop_sum_t &WDiagOperatorSum::sum() { return sum_; }

WDiagOperatorSum &WDiagOperatorSum::operator+=(WDiagOperatorSum &rhs) {
  for (auto &vec_dop_factor : rhs.sum()) {
    add(vec_dop_factor.first, vec_dop_factor.second);
  }
  return *this;
}
