#include <algorithm>
#include <iostream>

#include "helpers/helpers.h"
#include "helpers/orbital_space.h"
#include "helpers/stl_utils.hpp"

#include "equation.h"
#include "expression.h"
#include "sqoperator.h"
#include "tensor.h"
#include "term.h"

Expression::Expression() : Algebra<SymbolicTerm, scalar_t>() {}

void Expression::add(const Term &sterm) {
  SymbolicTerm symterm = sterm.symterm();
  auto search = terms_.find(symterm);

  if (search != terms_.end()) {
    /// Found, then just add the factor to the existing term
    search->second += sterm.coefficient();
    if (search->second == 0) {
      terms_.erase(search);
    }
  } else {
    terms_[symterm] = sterm.coefficient();
  }
}

void Expression::add(const std::pair<SymbolicTerm, scalar_t> &term_factor,
                     scalar_t scale) {

  const SymbolicTerm &term = term_factor.first;
  scalar_t factor = term_factor.second;

  auto search = terms_.find(term);

  if (search != terms_.end()) {
    /// Found, then just add the factor to the existing term
    search->second += scale * factor;
    if (search->second == 0) {
      terms_.erase(search);
    }
  } else {
    terms_[term] = scale * factor;
  }
}

void Expression::add(const Expression &expr, scalar_t scale) {
  for (const auto &kv : expr.terms()) {
    add(kv, scale);
  }
}

Expression &Expression::canonicalize() {
  std::map<SymbolicTerm, scalar_t> canonical_terms;
  for (const auto &[k, v] : terms_) {
    SymbolicTerm term = k;
    scalar_t factor = term.canonicalize();
    factor *= v;
    add_to_map(canonical_terms, term, factor);
  }
  terms_ = canonical_terms;
  return *this;
}

Expression &Expression::reindex(index_map_t &idx_map) {
  std::map<SymbolicTerm, scalar_t> reindexed_terms;
  for (auto &kv : terms_) {
    SymbolicTerm term = kv.first;
    term.reindex(idx_map);
    add_to_map(reindexed_terms, term, kv.second);
  }
  return *this;
}

bool Expression::operator==(const Expression &other) {
  return (terms_.size() == other.terms_.size()) and
         std::equal(terms_.begin(), terms_.end(), other.terms_.begin());
}

std::string Expression::str() const {
  std::vector<std::string> str_vec;
  int n = 0;
  for (auto &kv : terms_) {
    std::string symterm_str = kv.first.str();
    std::string factor_str;
    if (n == 0) { // don't show the first element sign unless it's negative
      factor_str += kv.second.str(false);
    } else {
      factor_str += kv.second.str(true);
    }
    // rational(1,1).str() returns "", so we need to handle the case of
    // a pure scalar term with no operator
    if ((factor_str.size() > 1) and (factor_str != "-")) {
      factor_str += " ";
    }
    if (factor_str.size() + symterm_str.size() == 0) {
      factor_str = (n == 0) ? "1" : "+1";
    }
    str_vec.push_back(factor_str + symterm_str);
    n++;
  }
  return join(str_vec, "\n");
}

std::string Expression::latex(const std::string &sep) const {
  std::vector<std::string> str_vec;
  for (auto &kv : terms_) {
    str_vec.push_back(kv.second.latex() + ' ' + kv.first.latex());
  }
  return join(str_vec, sep);
}

std::map<std::string, std::vector<Equation>>
Expression::to_manybody_equation(const std::string &label) const {
  std::map<std::string, std::vector<Equation>> result;
  for (const auto &term_factor : terms_) {
    std::vector<Index> lower;
    std::vector<Index> upper;
    const SymbolicTerm &term = term_factor.first;
    scalar_t factor = term_factor.second;
    for (const auto &op : term.ops()) {
      if (op.type() == SQOperatorType::Creation) {
        lower.push_back(op.index());
      } else {
        // upper indices are read in reverse order
        upper.insert(upper.begin(), op.index());
      }
    }
    SymbolicTerm lhs;
    Tensor lhs_tensor(label, lower, upper, SymmetryType::Antisymmetric);
    auto signature = lhs_tensor.signature();
    // convert the signature to a string (to bypass limitations of pybind11)
    std::string signature_str_upper;
    std::string signature_str_lower;
    int pos = 0;
    for (const auto &[u, l] : signature) {
      signature_str_upper += std::string(u, orbital_subspaces->label(pos));
      signature_str_lower += std::string(l, orbital_subspaces->label(pos));
      pos += 1;
    }
    reverse(signature_str_lower.begin(), signature_str_lower.end());
    auto signature_str = signature_str_upper + "|" + signature_str_lower;
    lhs.add(lhs_tensor);

    SymbolicTerm rhs;
    for (const auto &tensor : term.tensors()) {
      rhs.add(tensor);
    }
    result[signature_str].push_back(Equation(lhs, rhs, factor));
  }
  return result;
}

std::ostream &operator<<(std::ostream &os, const Expression &sum) {
  os << sum.str();
  return os;
}

Expression make_operator_expr(const std::string &label,
                              const std::vector<std::string> &components,
                              bool normal_ordered, SymmetryType symmetry,
                              scalar_t coefficient) {
  Expression result;
  for (const std::string &s : components) {
    auto s_vec = split(s);

    std::vector<int> cre_count(orbital_subspaces->num_spaces(), 0);
    std::vector<int> ann_count(orbital_subspaces->num_spaces(), 0);
    for (const auto &s : s_vec) {
      int space = orbital_subspaces->label_to_space(s[0]);
      ann_count[space] += 1;
    }

    std::vector<Index> cre;
    std::vector<Index> ann;

    // parse "v+ o"
    for (const auto &s : s_vec) {
      int space = orbital_subspaces->label_to_space(s[0]);
      if (s.size() == 2) {
        auto idx = Index(space, cre_count[space]);
        cre.push_back(idx);
        cre_count[space] += 1;
      } else {
        ann_count[space] -= 1;
        auto idx = Index(space, ann_count[space]);
        ann.push_back(idx);
      }
    }
    Term term;
    term.set_normal_ordered(normal_ordered);
    term.set(coefficient);
    for (const auto &c : cre) {
      term.add(SQOperator(SQOperatorType::Creation, c));
    }
    for (const auto &a : ann) {
      term.add(SQOperator(SQOperatorType::Annihilation, a));
    }
    std::reverse(std::begin(ann), std::end(ann));
    Tensor tensor(label, cre, ann, symmetry);
    term.add(tensor);
    result.add(term);
  }
  return result;
}

Expression string_to_expr(const std::string &s, SymmetryType symmetry) {

  TensorSyntax syntax = TensorSyntax::Wicked;
  Expression sum;

  // if we have an empty string, do not parse it (the code below would interpret
  // it as the term 1)
  if (s.size() == 0)
    return sum;

  //"f^{v0}_{o0} t^{o0}_{v0}"
  std::string factor_re;
  std::string tensor_re;
  std::string operator_re;
  if (syntax == TensorSyntax::Wicked) {
    tensor_re = "([a-zA-Z0-9]+\\^\\{[\\w,\\s]*\\}_\\{[\\w,\\s]*\\})";
    operator_re = "a([+-]{1,1})\\(([\\w,\\d]*)\\)";
    factor_re = "^\\s*([+-]?\\d*\\/?\\d*)\\s*";
  }

  auto tensors = findall(s, tensor_re);

  SymbolicTerm term;
  for (const auto &t : tensors) {
    term.add(make_tensor_from_str(t, symmetry));
  }

  auto operators = findall(s, operator_re);

  for (size_t n = 0; n < operators.size(); n += 2) {
    SQOperatorType type = operators[n] == "+" ? SQOperatorType::Creation
                                              : SQOperatorType::Annihilation;
    Index index = make_index_from_str(operators[n + 1]);
    term.add(SQOperator(type, index));
  }

  auto factor_vec = findall(s, factor_re);
  scalar_t factor = make_rational_from_str(factor_vec[0]);

  sum.add(term, factor);

  return sum;
}
