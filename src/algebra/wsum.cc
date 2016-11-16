#include <algorithm>
#include <iostream>

#include "helpers.h"
#include "wsum.h"
#include "walgebraicterm.h"
#include "wequationterm.h"
#include "wtensor.h"
#include "wsqoperator.h"

WSum::WSum() {}

void WSum::add(const WAlgebraicTerm &term, scalar_t factor) {
  auto search = terms_.find(term);

  if (search != terms_.end()) {
    /// Found, then just add the factor to the existing term
    search->second += factor;
    if (search->second == 0) {
      terms_.erase(search);
    }
  } else {
    terms_[term] = factor;
  }
}

void WSum::add(const std::pair<WAlgebraicTerm, scalar_t> &term_factor,
               scalar_t scale) {

  const WAlgebraicTerm &term = term_factor.first;
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

void WSum::add_sum(const WSum &sum, scalar_t scale) {
  for (const auto &kv : sum.terms()) {
    add(kv, scale);
  }
}

WSum &WSum::canonicalize() {
  std::map<WAlgebraicTerm, scalar_t> canonical_terms;
  for (auto &kv : terms_) {
    WAlgebraicTerm term = kv.first;
    scalar_t factor = term.canonicalize();
    factor *= kv.second;

    add_to_map(canonical_terms, term, factor);
  }
  terms_ = canonical_terms;
  return *this;
}

WSum &WSum::reindex(index_map_t &idx_map) {
  std::map<WAlgebraicTerm, scalar_t> reindexed_terms;
  for (auto &kv : terms_) {
    WAlgebraicTerm term = kv.first;
    term.reindex(idx_map);
    add_to_map(reindexed_terms, term, kv.second);
  }
  return *this;
}

bool WSum::operator==(const WSum &sum) {
  return terms_.size() == sum.terms_.size() &&
         std::equal(terms_.begin(), terms_.end(), sum.terms_.begin());
}

WSum &WSum::operator+=(const WSum &sum) {
  for (const auto &kv : sum.terms()) {
    add(kv);
  }
  return *this;
}

WSum &WSum::operator-=(const WSum &sum) {
  for (const auto &kv : sum.terms()) {
    add(kv, -1);
  }
  return *this;
}

std::string WSum::str() const {
  std::vector<std::string> str_vec;
  for (auto &kv : terms_) {
    str_vec.push_back(kv.second.str() + ' ' + kv.first.str());
  }
  return (to_string(str_vec, "\n"));
}

std::string WSum::latex() const {
  std::vector<std::string> str_vec;
  for (auto &kv : terms_) {
    str_vec.push_back(kv.second.latex() + ' ' + kv.first.latex());
  }
  return (to_string(str_vec, " \\\\ \n"));
}

std::vector<WEquationTerm>
WSum::to_manybody_equation(const std::string &label) {
  std::vector<WEquationTerm> result;

  for (const auto &term_factor : terms_) {
    std::vector<WIndex> lower;
    std::vector<WIndex> upper;
    const WAlgebraicTerm &term = term_factor.first;
    scalar_t factor = term_factor.second;
    for (const auto &op : term.ops()) {
      if (op.type() == Creation) {
        lower.push_back(op.index());
      } else {
        upper.push_back(op.index());
      }
    }
    WAlgebraicTerm rhs, lhs;
    WTensor lhs_tensor(label, lower, upper);
    lhs.add(lhs_tensor);
    factor *= lhs_tensor.symmetry_factor();

    for (const auto &tensor : term.tensors()) {
      rhs.add(tensor);
    }
    result.push_back(WEquationTerm(lhs, rhs, factor));
  }

  return result;
}

WSum operator+(WSum lhs, const WSum &rhs) {
  lhs += rhs;
  return lhs;
}

WSum operator-(WSum lhs, const WSum &rhs) {
  lhs -= rhs;
  return lhs;
}

std::ostream &operator<<(std::ostream &os, const WSum &sum) {
  os << sum.str();
  return os;
}

WSum string_to_sum(const std::string &s, TensorSyntax syntax) {
  WSum sum;

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

  WAlgebraicTerm term;
  for (size_t n = 0; n < tensors.size(); n += 3) {
    std::string label = tensors[n];

    // Proecess the upper indices
    auto upper_idx = split_indices(tensors[n + 1]);
    std::vector<WIndex> upper;
    for (const auto &idx : upper_idx) {
      upper.push_back(string_to_index(idx));
    }

    // Process the lower indices
    auto lower_idx = split_indices(tensors[n + 2]);
    std::vector<WIndex> lower;
    for (const auto &idx : lower_idx) {
      lower.push_back(string_to_index(idx));
    }
    term.add(WTensor(label, lower, upper));
  }

//  for (auto s : operators) {
//    std::cout << s << std::endl;
//  }

  for (size_t n = 0; n < operators.size(); n += 2) {
    SQOperatorType type = operators[n] == "+" ? Creation : Annihilation;
    WIndex index = string_to_index(operators[n + 1]);
    term.add(WSQOperator(type, index));
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
