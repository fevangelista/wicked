#ifndef _wicked_vector_space_h_
#define _wicked_vector_space_h_

#include <vector>

#include "helpers/helpers.h"

/// Represents a vector space of objects of type T over the field F
template <class T, class F> class Algebra {

public:
  using vecspace_t = std::map<T, F>;
  Algebra() {}
  Algebra(const vecspace_t &v) : terms_(v) {}

  /// size of
  size_t size() const { return terms_.size(); }
  const vecspace_t &terms() const { return terms_; }
  vecspace_t &terms() { return terms_; }

  /// add an element
  void add(const T &e, F c = scalar_t(1, 1), bool unique = false) { add_to_map(terms_, e, c, unique); }

  /// comparison
  bool is_equal(const Algebra &rhs) const { return terms_ == rhs.terms_; }

  /// addition assignment
  Algebra &operator+=(const Algebra &rhs) {
    for (const auto &[e, c] : rhs.terms()) {
      add(e, c);
    }
    return *this;
  }
  /// subtraction assignment
  Algebra &operator-=(const Algebra &rhs) {
    for (const auto &[e, c] : rhs.terms()) {
      add(e, -c);
    }
    return *this;
  }
  /// multiplication assignment (scalar)
  Algebra &operator*=(const Algebra &rhs) {
    Algebra result;
    for (const auto &[e, c] : terms()) {
      for (const auto &[er, cr] : rhs.terms()) {
        result.add(e * er, c * cr);
      }
    }
    terms_ = result.terms_;
    return *this;
  }
  /// multiplication assignment (scalar)
  Algebra &operator*=(scalar_t factor) {
    for (auto &[e, c] : terms()) {
      c *= factor;
    }
    return *this;
  }
  /// division assignment (scalar)
  Algebra &operator/=(scalar_t factor) {
    for (auto &[e, c] : terms()) {
      c /= factor;
    }
    return *this;
  }

  typename vecspace_t::iterator begin() { return terms_.begin(); }
  typename vecspace_t::const_iterator begin() const { return terms_.begin(); }
  typename vecspace_t::iterator end() { return terms_.end(); }
  typename vecspace_t::const_iterator end() const { return terms_.end(); }

protected:
  vecspace_t terms_;
};

#endif // _wicked_vector_space_h_