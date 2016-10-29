#include "helpers.h"
#include "orbital_space.h"
#include "wdiag_operator_sum.h"

WDiagOperatorSum::WDiagOperatorSum() {}

// WDiagOperatorSum::WDiagOperatorSum(const std::vector<WDiagOperator> &vec_dop,
//                                   scalar_t factor) {
//  add(vec_dop, factor);
//}

WDiagOperatorSum::WDiagOperatorSum(
    const std::vector<std::vector<WDiagOperator>> &vec_vec_dop,
    scalar_t factor) {
  for (const auto &vec_dop : vec_vec_dop) {
    add(vec_dop, factor);
  }
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

const dop_sum_t &WDiagOperatorSum::sum() const { return sum_; }

WDiagOperatorSum &WDiagOperatorSum::operator+=(const WDiagOperatorSum &rhs) {
  for (const auto &vec_dop_factor : rhs.sum()) {
    add(vec_dop_factor.first, vec_dop_factor.second);
  }
  return *this;
}

WDiagOperatorSum &WDiagOperatorSum::operator-=(const WDiagOperatorSum &rhs) {
  for (const auto &vec_dop_factor : rhs.sum()) {
    add(vec_dop_factor.first, -vec_dop_factor.second);
  }
  return *this;
}

WDiagOperatorSum &WDiagOperatorSum::operator*=(scalar_t factor) {
  for (auto &vec_dop_factor : sum_) {
    vec_dop_factor.second *= factor;
  }
  return *this;
}

WDiagOperatorSum &WDiagOperatorSum::operator/=(scalar_t factor) {
  for (auto &vec_dop_factor : sum_) {
    vec_dop_factor.second /= factor;
  }
  return *this;
}

std::string WDiagOperatorSum::str() const {
  std::vector<std::string> str_vec;
  for (auto &vec_dop_factor : sum_) {
    std::string s;
    s += vec_dop_factor.second.str(true);
    for (auto &dop : vec_dop_factor.first) {
      s += ' ' + dop.str();
    }
    str_vec.push_back(s);
  }
  return to_string(str_vec, "\n");
}

std::ostream &operator<<(std::ostream &os, const WDiagOperatorSum &opsum) {
  os << opsum.str();
  return os;
}

WDiagOperatorSum make_operator(const std::string &label,
                               const std::vector<std::string> &components) {
  WDiagOperatorSum result;
  for (const std::string &s : components) {
    auto s_vec = split(s, std::regex("[->]+"));

    std::vector<std::string> ann_labels;
    for (char c : s_vec[0]) {
      ann_labels.push_back(std::string(1, c));
    }
    std::vector<std::string> cre_labels;
    for (char c : s_vec[1]) {
      cre_labels.push_back(std::string(1, c));
    }
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

    result.add({WDiagOperator(label, cre, ann)});
  }
  return result;
}

WDiagOperatorSum commutator(const WDiagOperatorSum &A,
                            const WDiagOperatorSum &B) {
  WDiagOperatorSum result;
  for (const auto &vec_factor_A : A.sum()) {
    for (const auto &vec_factor_B : B.sum()) {
      auto &vec_A = vec_factor_A.first;
      auto &vec_B = vec_factor_B.first;

      std::vector<WDiagOperator> vec_AB;
      vec_AB.insert(vec_AB.end(), vec_A.begin(), vec_A.end());
      vec_AB.insert(vec_AB.end(), vec_B.begin(), vec_B.end());

      std::vector<WDiagOperator> vec_BA;
      vec_BA.insert(vec_BA.end(), vec_B.begin(), vec_B.end());
      vec_BA.insert(vec_BA.end(), vec_A.begin(), vec_A.end());

      scalar_t factor_A = vec_factor_A.second;
      scalar_t factor_B = vec_factor_B.second;

      result.add(vec_AB, factor_A * factor_B);
      result.add(vec_BA, -factor_A * factor_B);
    }
  }
  return result;
}

WDiagOperatorSum exp(const WDiagOperatorSum &A, int order) {
  WDiagOperatorSum result;
  result.add({});
  //  WDiagOperatorSum temp1;
  //  temp1.add({});
  //  for (int k = 1; k <= order; k++){
  //      temp1 *= A;
  //      result.add(temp);
  //  }
  return result;
}

WDiagOperatorSum bch_series(const WDiagOperatorSum &A,
                            const WDiagOperatorSum &B, int n) {
  WDiagOperatorSum result;
  result += A;
  WDiagOperatorSum temp(A);
  for (int k = 1; k <= n; k++) {
    WDiagOperatorSum comm = commutator(temp, B);
    comm *= scalar_t(1, k);
    result += comm;
    temp = comm;
  }

  return result;
}
