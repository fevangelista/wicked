#pragma once

#include <cmath>
#include <vector>

#include "helpers/helpers.h"

/// Represents a vector space of objects of type T over the field F
template <typename Derived, class T, class F> class Algebra {

public:
  /// @brief A type used to store an element of the algebra. Uses a map from
  /// elements of type T to field elements of type F
  using vecspace_t = std::map<T, F>;

  /// @brief Default constructor
  Algebra() {}

  /// @brief Construct from a map
  /// @param v the element of the algebra
  Algebra(const vecspace_t &v) : terms_(v) {}

  /// @brief The number of elements in the term
  size_t size() const { return terms_.size(); }
  /// @brief Return the terms as a map
  const vecspace_t &terms() const { return terms_; }
  /// @brief Return the terms as a map
  vecspace_t &terms() { return terms_; }

  /// @brief Add a term
  /// @param e an element of the vector space
  /// @param c an element of the field
  void add(const T &e, F c = scalar_t(1, 1)) { add_to_map(terms_, e, c); }

  /// test if element is in the space
  bool contains(const T &e) const { return terms_.find(e) != terms_.end(); }

  /// @brief Is this term equal to another term?
  bool is_equal(const Algebra &rhs) const { return terms_ == rhs.terms_; }

  /// @brief Dot product of two terms
  F dot(const Derived &rhs) const {
    F result{0};
    for (const auto &[e, c] : terms()) {
      auto search = rhs.terms().find(e);
      if (search != rhs.terms().end()) {
        result += c * search->second;
      }
    }
    return result;
  }

  /// @brief Norm of the term
  double norm() const {
    double result = 0;
    for (const auto &[e, c] : terms()) {
      auto c2 = c * c;
      result += c2.to_double();
    }
    return std::sqrt(result);
  }

  /// @brief Unary minus operator
  Derived operator-() const {
    Derived result;
    for (const auto &[e, c] : terms()) {
      result.add(e, -c);
    }
    return result;
  }

  /// @brief Addition operator
  /// @param rhs the term to add
  Derived &operator+(const Algebra &rhs) {
    Derived result;
    for (const auto &[e, c] : terms()) {
      result.add(e, c);
    }
    for (const auto &[e, c] : rhs.terms()) {
      result.add(e, c);
    }
    return result;
  }

  /// @brief Addition assignment operator
  /// @param rhs the term to add
  Algebra &operator+=(const std::pair<T, F> &rhs) {
    const auto &[e, c] = rhs;
    add(e, c);
    return *this;
  }

  /// @brief Addition assignment operator
  /// @param rhs the term to add
  Algebra &operator+=(const Algebra &rhs) {
    for (const auto &[e, c] : rhs.terms()) {
      add(e, c);
    }
    return *this;
  }

  /// @brief Subtraction assignment operator
  /// @param rhs the term to subtract
  Algebra &operator-=(const Algebra &rhs) {
    for (const auto &[e, c] : rhs.terms()) {
      add(e, -c);
    }
    return *this;
  }

  /// @brief Multiplication assignment operator
  /// @param rhs the term to multiply by
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

  /// @brief Scalar multiplication assignment operator
  /// @param rhs the scalar to multiply by
  Algebra &operator*=(scalar_t factor) {
    for (auto &[e, c] : terms()) {
      c *= factor;
    }
    return *this;
  }

  /// @brief Scalar division assignment operator
  Algebra &operator/=(scalar_t factor) {
    if (factor == scalar_t(0, 1)) {
      throw std::runtime_error("division by zero");
    }
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
