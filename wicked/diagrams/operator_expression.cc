#include "operator_expression.h"
#include "helpers.h"
#include "operator.h"
#include "orbital_space.h"

OperatorExpression::OperatorExpression() {}

OperatorExpression::OperatorExpression(
    const std::vector<OperatorProduct> &vec_vec_dop, scalar_t factor) {
  for (const auto &vec_dop : vec_vec_dop) {
    add(vec_dop, factor);
  }
}

void OperatorExpression::add(const OperatorProduct &vec_dop, scalar_t factor) {
  auto search = terms_.find(vec_dop);

  if (search != terms_.end()) {
    /// Found, then just add the factor to the existing term
    search->second += factor;
    if (search->second == 0) {
      terms_.erase(search);
    }
  } else {
    terms_[vec_dop] = factor;
  }
}

void OperatorExpression::add2(const OperatorExpression &expr, scalar_t factor) {
  for (const auto &vec_dop_factor : expr.terms()) {
    add(vec_dop_factor.first, factor * vec_dop_factor.second);
  }
}

size_t OperatorExpression::size() const { return terms_.size(); }
const dop_expr_t &OperatorExpression::terms() const { return terms_; }

void OperatorExpression::canonicalize() {
  dop_expr_t canonical;
  for (auto [prod, scalar] : terms_) {
    auto newprod = prod;
    const auto sign = newprod.canonicalize();
    add_to_map(canonical, newprod, sign * scalar);
  }
  terms_ = canonical;
}

OperatorExpression &
OperatorExpression::operator+=(const OperatorExpression &rhs) {
  for (const auto &vec_dop_factor : rhs.terms()) {
    add(vec_dop_factor.first, vec_dop_factor.second);
  }
  return *this;
}

OperatorExpression &
OperatorExpression::operator-=(const OperatorExpression &rhs) {
  for (const auto &vec_dop_factor : rhs.terms()) {
    add(vec_dop_factor.first, -vec_dop_factor.second);
  }
  return *this;
}

OperatorExpression &
OperatorExpression::operator*=(const OperatorExpression &rhs) {
  OperatorExpression result;
  for (const auto &r_vec_dop_factor : terms()) {
    for (const auto &l_vec_dop_factor : rhs.terms()) {
      std::vector<Operator> prod;
      prod.insert(prod.end(), r_vec_dop_factor.first.begin(),
                  r_vec_dop_factor.first.end());
      prod.insert(prod.end(), l_vec_dop_factor.first.begin(),
                  l_vec_dop_factor.first.end());
      result.add(prod, r_vec_dop_factor.second * l_vec_dop_factor.second);
    }
  }
  terms_ = result.terms_;
  return *this;
}

OperatorExpression &OperatorExpression::operator*=(scalar_t factor) {
  for (auto &vec_dop_factor : terms_) {
    vec_dop_factor.second *= factor;
  }
  return *this;
}

OperatorExpression &OperatorExpression::operator/=(scalar_t factor) {
  for (auto &vec_dop_factor : terms_) {
    vec_dop_factor.second /= factor;
  }
  return *this;
}

std::string OperatorExpression::str() const {
  std::vector<std::string> str_vec;
  for (auto &vec_dop_factor : terms_) {
    std::string s;
    s += vec_dop_factor.second.str(true);
    for (auto &dop : vec_dop_factor.first) {
      s += ' ' + dop.str();
    }
    str_vec.push_back(s);
  }
  return join(str_vec, "\n");
}

OperatorExpression operator*(OperatorExpression lhs,
                             const OperatorExpression &rhs) {
  lhs *= rhs;
  return lhs;
}

std::ostream &operator<<(std::ostream &os, const OperatorExpression &opsum) {
  os << opsum.str();
  return os;
}

OperatorExpression
make_diag_operator_expression(const std::string &label,
                              const std::vector<std::string> &components) {
  OperatorExpression result;
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
    for (const auto &l : cre_labels) {
      int space = osi->label_to_space(l);
      cre[space] += 1;
    }
    for (const auto &l : ann_labels) {
      int space = osi->label_to_space(l);
      ann[space] += 1;
    }

    result.add({Operator(label, cre, ann)});
  }
  return result;
}

OperatorExpression
make_diag_operator_expression2(const std::string &label,
                               const std::vector<std::string> &components) {
  OperatorExpression result;
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
    result.add({Operator(label, cre, ann)});
  }
  return result;
}

OperatorExpression commutator(const OperatorExpression &A,
                              const OperatorExpression &B) {
  OperatorExpression result;
  for (const auto &vec_factor_A : A.terms()) {
    for (const auto &vec_factor_B : B.terms()) {
      auto &vec_A = vec_factor_A.first;
      auto &vec_B = vec_factor_B.first;

      std::vector<Operator> vec_AB;
      vec_AB.insert(vec_AB.end(), vec_A.begin(), vec_A.end());
      vec_AB.insert(vec_AB.end(), vec_B.begin(), vec_B.end());

      std::vector<Operator> vec_BA;
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

// OperatorExpression exp(const OperatorExpression &A, int order) {
//   OperatorExpression result;
//   result.add({});
//   //  OperatorExpression temp1;
//   //  temp1.add({});
//   //  for (int k = 1; k <= order; k++){
//   //      temp1 *= A;
//   //      result.add(temp);
//   //  }
//   return result;
// }

OperatorExpression bch_series(const OperatorExpression &A,
                              const OperatorExpression &B, int n) {
  OperatorExpression result;
  result += A;
  OperatorExpression temp(A);

  for (int k = 1; k <= n; k++) {
    OperatorExpression comm = commutator(temp, B);
    comm *= scalar_t(1, k);
    result += comm;
    temp = comm;
  }

  return result;
}
