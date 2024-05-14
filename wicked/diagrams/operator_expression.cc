#include "helpers/helpers.h"
#include "helpers/orbital_space.h"

#include "operator.h"
#include "operator_expression.h"

OperatorExpression::OperatorExpression()
    : Algebra<OperatorProduct, scalar_t>() {}

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
  if (!unique) {std::cout << "Warning: combinatorically equivalent operators are not "
               "removed from the expression\n" << std::endl;}
  for (const std::string &s : components) {
    auto s_vec = findall(s, "([a-zA-Z][+^]?)");
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
    result.add({Operator(label, cre, ann)}, scalar_t(1, 1), unique);
  }
  return result;
}

OperatorExpression commutator(const OperatorExpression &A,
                              const OperatorExpression &B) {
  return A * B - B * A;
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
