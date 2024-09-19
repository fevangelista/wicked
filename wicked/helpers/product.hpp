#pragma once

#include <algorithm>
#include <vector>

template <class T> class Product {
  using prod_t = std::vector<T>;

public:
  Product() {}
  Product(const prod_t &elements) : elements_(elements) {}
  Product(const Product &prod) : elements_(prod.elements()) {}
  Product(std::initializer_list<T> elements) : elements_(elements) {}

  const prod_t &elements() const { return elements_; }

  void clear() { elements_.clear(); }

  size_t size() const { return elements_.size(); }

  void push_back(const T &e) { elements_.push_back(e); }

  bool operator<(const Product<T> &other) const {
    return elements_ < other.elements_;
  }

  bool is_equal(const Product<T> &other) const {
    return elements_ == other.elements_;
  }

  Product adjoint() const {
    Product d;
    for (const auto &e : elements()) {
      d.push_back(e.adjoint());
    }
    std::reverse(d.begin(), d.end());
    return d;
  }

  T &operator[](size_t n) { return elements_[n]; }

  const T &operator[](size_t n) const { return elements_[n]; }

  typename std::vector<T>::iterator begin() { return elements_.begin(); }
  typename std::vector<T>::const_iterator begin() const {
    return elements_.begin();
  }
  typename std::vector<T>::iterator end() { return elements_.end(); }
  typename std::vector<T>::const_iterator end() const {
    return elements_.end();
  }

  const std::vector<T> &vec() const { return elements_; }
  std::vector<T> &vec() { return elements_; }

  /// @brief Multiplication assignment operator
  /// @param rhs the term to multiply by
  Product &operator*=(const Product &rhs) {
    // Add the elements of the second product
    elements_.insert(elements_.end(), rhs.elements_.begin(),
                     rhs.elements_.end());
    return *this;
  }

protected:
  prod_t elements_;
};

template <class T> Product<T> operator*(Product<T> rhs, const Product<T> &lhs) {
  rhs *= lhs;
  return rhs;
}

template <class T>
bool operator==(const Product<T> &rhs, const Product<T> &lhs) {
  return rhs.is_equal(lhs);
}
