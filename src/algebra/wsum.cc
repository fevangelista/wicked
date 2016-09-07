#include <algorithm>
#include <iostream>

#include "helpers.h"
#include "wsum.h"

WSum::WSum() {}

void WSum::add(const WAlgebraicTerm &term, scalar_t scale) {
  auto it = std::find(sum_.begin(), sum_.end(), term);
  if (it != sum_.end()) {
    /// Just add the factor to the existing term
    scalar_t factor = it->factor();
    factor += scale * term.factor();
    it->set_factor(factor);
    if (factor == 0){
        std::cout << "\n Term cancellation!" << std::endl;
    }
  } else {
    sum_.push_back(term);
    scalar_t scaled_factor = scale * term.factor();
    sum_.back().set_factor(scaled_factor);
  }
}

void WSum::add(const WSum& sum, scalar_t scale) {
  for (const WAlgebraicTerm &term : sum.sum_) {
    add(term, scale);
  }
}

std::string WSum::str() const {
  std::vector<std::string> str_vec;
  for (auto &term : sum_) {
    str_vec.push_back(term.str());
  }
  return (to_string(str_vec, "\n"));
}
