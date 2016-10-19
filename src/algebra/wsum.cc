#include <algorithm>
#include <iostream>

#include "helpers.h"
#include "wsum.h"

WSum::WSum() {}

void WSum::add(const std::pair<WAlgebraicTerm, scalar_t> &term_factor,
               scalar_t scale) {

  const WAlgebraicTerm &term = term_factor.first;
  scalar_t factor = term_factor.second;

  auto search = sum_.find(term);

  if (search != sum_.end()) {
    /// Found, then just add the factor to the existing term
    search->second += scale * factor;
    if (search->second == 0) {
      std::cout << "\n Term cancellation!" << std::endl;
      sum_.erase(search);
    }
  } else {
    sum_[term] = scale * factor;
  }
}

void WSum::add_sum(WSum&& sum, scalar_t scale) {
  for (const auto &kv : sum.sum()) {
    add(kv, scale);
  }
}

std::string WSum::str() const {
  std::vector<std::string> str_vec;
  for (auto &kv : sum_) {
    str_vec.push_back(kv.second.str() + ' ' + kv.first.str());
  }
  return (to_string(str_vec, "\n"));
}

std::ostream &operator<<(std::ostream &os, const WSum &sum) {
  os << sum.str();
  return os;
}
