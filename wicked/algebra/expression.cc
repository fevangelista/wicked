#include <algorithm>
#include <iostream>

#include "expression.h"
#include "helpers.h"
#include "sqoperator.h"
#include "tensor.h"
#include "term.h"
#include "wequationterm.h"

Expression::Expression() {}

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

void Expression::add(const SymbolicTerm &symterm, scalar_t coefficient) {
  auto search = terms_.find(symterm);

  if (search != terms_.end()) {
    /// Found, then just add the factor to the existing term
    search->second += coefficient;
    if (search->second == 0) {
      terms_.erase(search);
    }
  } else {
    terms_[symterm] = coefficient;
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
  for (auto &kv : terms_) {
    SymbolicTerm term = kv.first;
    scalar_t factor = term.canonicalize();
    factor *= kv.second;

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

bool Expression::operator==(const Expression &sum) {
  return terms_.size() == sum.terms_.size() &&
         std::equal(terms_.begin(), terms_.end(), sum.terms_.begin());
}

Expression &Expression::operator+=(const Expression &sum) {
  for (const auto &kv : sum.terms()) {
    add(kv);
  }
  return *this;
}

Expression &Expression::operator-=(const Expression &sum) {
  for (const auto &kv : sum.terms()) {
    add(kv, -1);
  }
  return *this;
}

std::string Expression::str() const {
  std::vector<std::string> str_vec;
  int n = 0;
  for (auto &kv : terms_) {
    std::string term_str;
    if (n == 0) {
      term_str += kv.second.str(false);
    } else {
      term_str += kv.second.str(true);
    }
    if ((term_str.size() > 1) and (term_str != "-")) {
      term_str += " ";
    }
    str_vec.push_back(term_str + kv.first.str());
    n++;
  }
  return (join(str_vec, "\n"));
}

std::string Expression::latex(const std::string &sep) const {
  std::vector<std::string> str_vec;
  for (auto &kv : terms_) {
    str_vec.push_back(kv.second.latex() + ' ' + kv.first.latex());
  }
  return (join(str_vec, sep));
}

// std::vector<WEquationSymbolicTerm>
// Expression::to_manybody_equation(const std::string &label) {
//   std::vector<WEquationSymbolicTerm> result;

//   for (const auto &term_factor : terms_) {
//     std::vector<Index> lower;
//     std::vector<Index> upper;
//     const SymbolicTerm &term = term_factor.first;
//     scalar_t factor = term_factor.second;
//     for (const auto &op : term.ops()) {
//       if (op.type() == SQOperatorType::Creation) {
//         lower.push_back(op.index());
//       } else {
//         upper.push_back(op.index());
//       }
//     }
//     SymbolicTerm rhs, lhs;
//     Tensor lhs_tensor(label, lower, upper);
//     lhs.add(lhs_tensor);
//     factor *= lhs_tensor.symmetry_factor();

//     for (const auto &tensor : term.tensors()) {
//       rhs.add(tensor);
//     }
//     result.push_back(WEquationSymbolicTerm(lhs, rhs, factor));
//   }

//   return result;
// }

Expression operator+(Expression lhs, const Expression &rhs) {
  lhs += rhs;
  return lhs;
}

Expression operator-(Expression lhs, const Expression &rhs) {
  lhs -= rhs;
  return lhs;
}

std::ostream &operator<<(std::ostream &os, const Expression &sum) {
  os << sum.str();
  return os;
}

// Expression make_operator_expr(const std::string &label,
//                               const std::vector<std::string> &components) {
//   Expression result;
//   for (const std::string &s : components) {
//     auto s_vec = split(s, std::regex("[->]+"));

//     std::vector<std::string> ann_labels;
//     for (char c : s_vec[0]) {
//       ann_labels.push_back(std::string(1, c));
//     }
//     std::vector<std::string> cre_labels;
//     for (char c : s_vec[1]) {
//       cre_labels.push_back(std::string(1, c));
//     }
//     std::vector<int> cre(osi->num_spaces());
//     std::vector<int> ann(osi->num_spaces());
//     for (const auto &label : cre_labels) {
//       int space = osi->label_to_space(label);
//       cre[space] += 1;
//     }
//     for (const auto &label : ann_labels) {
//       int space = osi->label_to_space(label);
//       ann[space] += 1;
//     }

//     result.add({WDiagOperator(label, cre, ann)});
//   }
//   return result;
// }

Expression string_to_sum(const std::string &s, TensorSyntax syntax) {
  Expression sum;

  //  std::cout << "\n  Parsing: \"" << s << "\"" << std::endl;

  //"f^{v0}_{o0} t^{o0}_{v0}"
  std::string factor_re;
  std::string tensor_re;
  std::string operator_re;
  if (syntax == TensorSyntax::Wicked) {
    tensor_re = "([a-zA-Z0-9]+)\\^\\{([\\w,\\s]*)\\}_\\{([\\w,\\s]*)\\}";
    operator_re = "a([+-]{1,1})\\(([\\w,\\d]*)\\)";
    factor_re = "^\\s*([+-]?\\d*)?/?(\\d*)?\\s*";
  }

  //  std::cout << "Parsing tensors: " << std::endl;
  auto tensors = findall(s, tensor_re);
  auto operators = findall(s, operator_re);

  SymbolicTerm term;
  for (size_t n = 0; n < tensors.size(); n += 3) {
    std::string label = tensors[n];

    // Proecess the upper indices
    auto upper_idx = split_indices(tensors[n + 1]);
    std::vector<Index> upper;
    for (const auto &idx : upper_idx) {
      upper.push_back(make_index(idx));
    }

    // Process the lower indices
    auto lower_idx = split_indices(tensors[n + 2]);
    std::vector<Index> lower;
    for (const auto &idx : lower_idx) {
      lower.push_back(make_index(idx));
    }
    term.add(Tensor(label, lower, upper));
  }

  //  for (auto s : operators) {
  //    std::cout << s << std::endl;
  //  }

  for (size_t n = 0; n < operators.size(); n += 2) {
    SQOperatorType type = operators[n] == "+" ? SQOperatorType::Creation
                                              : SQOperatorType::Annihilation;
    Index index = make_index(operators[n + 1]);
    term.add(SQOperator(type, index));
  }

  //  std::cout << "Parsing factor: " << std::endl;

  auto factor_vec = findall(s, factor_re);
  //  for (auto f : factor_vec) {
  //    std::cout << "Factor: " << f << std::endl;
  //  }
  int numerator = 1;
  int denominator = 1;
  if (factor_vec.size() == 2) {
    if (factor_vec[0] == "") {
    } else if (factor_vec[0] == "-") {
      numerator = -1;
    } else {
      numerator = std::stoi(factor_vec[0]);
    }
    if (factor_vec[1] != "") {
      denominator = std::stoi(factor_vec[1]);
    }
  }
  scalar_t factor(numerator, denominator);
  //  std::cout << term << std::endl;

  sum.add(term, factor);
  return sum;
}
