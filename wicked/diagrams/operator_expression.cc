#include "helpers/helpers.h"
#include "helpers/orbital_space.h"

#include "operator.h"
#include "operator_expression.h"

OperatorExpression::OperatorExpression()
    : Algebra<OperatorExpression, OperatorProduct, scalar_t>() {}

OperatorExpression::OperatorExpression(
    const std::vector<OperatorProduct> &vec_vec_dop, scalar_t factor) {
  for (const auto &vec_dop : vec_vec_dop) {
    add(vec_dop, factor);
  }
}

void OperatorExpression::add2(const OperatorExpression &expr, scalar_t factor) {
  for (const auto &vec_dop_factor : expr.terms()) {
    add(vec_dop_factor.first, factor * vec_dop_factor.second);
  }
}

void OperatorExpression::canonicalize() {
  opexpr_t canonical;
  for (auto [prod, scalar] : terms_) {
    auto newprod = prod;
    const auto sign = newprod.canonicalize();
    add_to_map(canonical, newprod, sign * scalar);
  }
  terms_ = canonical;
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

OperatorExpression operator+(OperatorExpression lhs,
                             const OperatorExpression &rhs) {
  lhs += rhs;
  return lhs;
}

OperatorExpression operator-(OperatorExpression lhs,
                             const OperatorExpression &rhs) {
  lhs -= rhs;
  return lhs;
}

std::ostream &operator<<(std::ostream &os, const OperatorExpression &opsum) {
  os << opsum.str();
  return os;
}

OperatorExpression
make_diag_operator_expression(const std::string &label,
                              const std::vector<std::string> &components,
                              bool unique) {
  OperatorExpression result;

  for (const std::string &s : components) {
    auto s_vec = findall(s, std::regex(R"(([a-zA-Z][+^]?))"));
    std::vector<int> cre(orbital_subspaces->num_spaces());
    std::vector<int> ann(orbital_subspaces->num_spaces());

    for (auto const &el : s_vec) {
      int space = orbital_subspaces->label_to_space(el[0]);
      if (el.size() > 1) {
        cre[space] += 1;
      } else {
        ann[space] += 1;
      }
    }
    Operator op(label, cre, ann);
    // if we want unique terms, we check if the term is already in the result
    if (unique and result.contains({op})) {
      continue;
    }
    result.add({op}, scalar_t(1, 1));
  }
  return result;
}

OperatorExpression commutator(const OperatorExpression &A,
                              const OperatorExpression &B) {
  return A * B - B * A;
}

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
