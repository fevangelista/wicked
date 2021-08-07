#include "diag_operator_expression.h"
#include "helpers.h"
#include "orbital_space.h"

DiagOpExpression::DiagOpExpression() {}

// DiagOpExpression::DiagOpExpression(const std::vector<DiagOperator>
// &vec_dop,
//                                   scalar_t factor) {
//  add(vec_dop, factor);
//}

DiagOpExpression::DiagOpExpression(
    const std::vector<std::vector<DiagOperator>> &vec_vec_dop,
    scalar_t factor) {
  for (const auto &vec_dop : vec_vec_dop) {
    add(vec_dop, factor);
  }
}

void DiagOpExpression::add(const std::vector<DiagOperator> &vec_dop,
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

const dop_sum_t &DiagOpExpression::sum() const { return sum_; }

DiagOpExpression &DiagOpExpression::operator+=(const DiagOpExpression &rhs) {
  for (const auto &vec_dop_factor : rhs.sum()) {
    add(vec_dop_factor.first, vec_dop_factor.second);
  }
  return *this;
}

DiagOpExpression &DiagOpExpression::operator-=(const DiagOpExpression &rhs) {
  for (const auto &vec_dop_factor : rhs.sum()) {
    add(vec_dop_factor.first, -vec_dop_factor.second);
  }
  return *this;
}

DiagOpExpression &DiagOpExpression::operator*=(const DiagOpExpression &rhs) {
  DiagOpExpression result;
  for (const auto &r_vec_dop_factor : sum()) {
    for (const auto &l_vec_dop_factor : rhs.sum()) {
      std::vector<DiagOperator> prod;
      prod.insert(prod.end(), r_vec_dop_factor.first.begin(),
                  r_vec_dop_factor.first.end());
      prod.insert(prod.end(), l_vec_dop_factor.first.begin(),
                  l_vec_dop_factor.first.end());
      result.add(prod, r_vec_dop_factor.second * l_vec_dop_factor.second);
    }
  }
  sum_ = result.sum_;
  return *this;
}

DiagOpExpression &DiagOpExpression::operator*=(scalar_t factor) {
  for (auto &vec_dop_factor : sum_) {
    vec_dop_factor.second *= factor;
  }
  return *this;
}

DiagOpExpression &DiagOpExpression::operator/=(scalar_t factor) {
  for (auto &vec_dop_factor : sum_) {
    vec_dop_factor.second /= factor;
  }
  return *this;
}

std::string DiagOpExpression::str() const {
  std::vector<std::string> str_vec;
  for (auto &vec_dop_factor : sum_) {
    std::string s;
    s += vec_dop_factor.second.str(true);
    for (auto &dop : vec_dop_factor.first) {
      s += ' ' + dop.str();
    }
    str_vec.push_back(s);
  }
  return join(str_vec, "\n");
}

DiagOpExpression operator*(DiagOpExpression lhs, const DiagOpExpression &rhs) {
  lhs *= rhs;
  return lhs;
}

std::ostream &operator<<(std::ostream &os, const DiagOpExpression &opsum) {
  os << opsum.str();
  return os;
}

DiagOpExpression
make_diag_operator_expression(const std::string &label,
                              const std::vector<std::string> &components) {
  DiagOpExpression result;
  for (const std::string &s : components) {
    auto s_vec = split(s, std::regex("[->]+"));

    std::vector<char> ann_labels;
    for (char c : s_vec[0]) {
      ann_labels.push_back(c);
    }
    std::vector<char> cre_labels;
    for (char c : s_vec[1]) {
      cre_labels.push_back(c);
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

    result.add({DiagOperator(label, cre, ann)});
  }
  return result;
}

DiagOpExpression
make_diag_operator_expression2(const std::string &label,
                               const std::vector<std::string> &components) {
  DiagOpExpression result;
  for (const std::string &s : components) {
    auto s_vec = findall(s, "([a-zA-Z][+^]?)");
    std::vector<int> cre(osi->num_spaces());
    std::vector<int> ann(osi->num_spaces());

    for (auto const &el : s_vec) {
      int space = osi->label_to_space(el[0]);
      if (el.size() > 1) {
        cre[space] += 1;
      } else {
        ann[space] += 1;
      }
    }
    result.add({DiagOperator(label, cre, ann)});
  }
  return result;
}

DiagOpExpression commutator(const DiagOpExpression &A,
                            const DiagOpExpression &B) {
  DiagOpExpression result;
  for (const auto &vec_factor_A : A.sum()) {
    for (const auto &vec_factor_B : B.sum()) {
      auto &vec_A = vec_factor_A.first;
      auto &vec_B = vec_factor_B.first;

      std::vector<DiagOperator> vec_AB;
      vec_AB.insert(vec_AB.end(), vec_A.begin(), vec_A.end());
      vec_AB.insert(vec_AB.end(), vec_B.begin(), vec_B.end());

      std::vector<DiagOperator> vec_BA;
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

DiagOpExpression exp(const DiagOpExpression &A, int order) {
  DiagOpExpression result;
  result.add({});
  //  DiagOpExpression temp1;
  //  temp1.add({});
  //  for (int k = 1; k <= order; k++){
  //      temp1 *= A;
  //      result.add(temp);
  //  }
  return result;
}

DiagOpExpression bch_series(const DiagOpExpression &A,
                            const DiagOpExpression &B, int n) {
  DiagOpExpression result;
  result += A;
  DiagOpExpression temp(A);

  for (int k = 1; k <= n; k++) {
    DiagOpExpression comm = commutator(temp, B);
    comm *= scalar_t(1, k);
    result += comm;
    temp = comm;
  }

  return result;
}
