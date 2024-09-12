#ifndef _wicked_helpers_h_
#define _wicked_helpers_h_

#include <iostream>
#include <map>
#include <numeric>
#include <optional>
#include <regex>
#include <string>
#include <vector>

#include "wicked-def.h"

// Returns a comma separated list of the indices
std::string join(const std::vector<std::string> &svec,
                 const std::string &sep = ",");

/// Split a string.  Default delimiters are spaces (\s) and/or commas
std::vector<std::string> split(const std::string &s,
                               std::regex re = std::regex("[\\s,]+"));

/// Find all occurences of a pattern
std::vector<std::string> findall(const std::string &s, const std::regex &regex);

/// Split indices
std::vector<std::string> split_indices(const std::string &s);

template <class T> std::vector<T> iota_vector(size_t size, T value = T(0)) {
  std::vector<T> v(size);
  std::iota(v.begin(), v.end(), value);
  return v;
}

/// A range iterator class used to loop over
class int_matrix {
private:
  size_t nrows_;
  size_t ncols_;
  std::vector<int> m_;

public:
  int_matrix(size_t nrows, size_t ncols)
      : nrows_(nrows), ncols_(ncols), m_(nrows * ncols, 0) {}
  int &operator()(size_t i, size_t j) { return m_[i * ncols_ + j]; }
  int operator()(size_t i, size_t j) const { return m_[i * ncols_ + j]; }
  std::string str() {
    std::string s;
    for (int i = 0; i < nrows_; ++i) {
      for (int j = 0; j < ncols_; ++j) {
        s += " " + std::to_string(m_[i * ncols_ + j]);
      }
      s += '\n';
    }
    return s;
  }
};

template <class T, class F>
void add_to_map(std::map<T, F> &m, const T &key, const F &value) {
  // don't add a zero term
  if (value == 0)
    return;

  // find the key
  auto search = m.find(key);

  if (search != m.end()) {
    // found key: just add the factor to the existing term
    search->second += value;
    // if after addition the result is zero, eliminate from map
    if (search->second == 0) {
      m.erase(search);
    }
  } else {
    // key not found:
    m[key] = value;
  }
}

// A class to count indices
class index_counter {
private:
  std::vector<int> counter_;

public:
  index_counter(int n) : counter_(n, 0) {}
  // A next available value of index i
  int next_index(int i) {
    int index = counter_[i];
    counter_[i] += 1;
    return index;
  }
  // The value of index i
  int index(int i) { return counter_[i]; }
};

/// A range iterator class used to loop over
class range {
private:
  size_t iter;
  size_t last;

public:
  range(size_t end) : iter(0), last(end) {}
  range(size_t begin, size_t end) : iter(begin), last(end) {}

  // Iterable functions
  const range &begin() const { return *this; }
  const range &end() const { return *this; }

  // Iterator functions
  bool operator!=(const range &) const { return iter < last; }
  void operator++() { ++iter; }
  size_t operator*() const { return iter; }
};

/// Enumerator class used to loop over vector indices and elements
template <class T, class A = std::allocator<T>> class enumerate_class {
private:
  size_t last;
  size_t iter;
  std::vector<T, A> &vec_;

public:
  enumerate_class(std::vector<T, A> &vec)
      : last(vec.size()), iter(0), vec_(vec) {}

  // Iterable functions
  const enumerate_class &begin() const { return *this; }
  const enumerate_class &end() const { return *this; }

  // Iterator functions
  bool operator!=(const enumerate_class &) const { return iter < last; }
  void operator++() { ++iter; }
  std::pair<int, T &> operator*() const {
    return std::pair<size_t, T &>(iter, vec_[iter]);
  }
};

/// Create an enumerator object
template <class T, class A = std::allocator<T>>
enumerate_class<T, A> enumerate(std::vector<T, A> &vec) {
  return enumerate_class<T, A>(vec);
}

/// @brief A template function to find the first adjacent pair of elements in a
/// vector that satisfy a predicate p and swap them.
/// This function assumes that there is at least one pair of elements that
/// satisfy the predicate.
/// @param vec The vector to be modified
/// @param p A binary predicate that takes two elements of the vector and
/// compares them
template <class T, class BinaryPredicate>
std::optional<std::pair<T, T>> swap_first_unordered_pair(std::vector<T> &vec,
                                                         BinaryPredicate p) {
  auto it = std::adjacent_find(vec.begin(), vec.end(), p);
  if (it != vec.end() && std::next(it) != vec.end()) {
    T firstValue = *it;
    T secondValue = *std::next(it);
    std::swap(*it, *std::next(it));
    return std::make_pair(firstValue, secondValue);
  }
  return std::nullopt;
}

/// @brief A template function to find the first adjacent pair of elements in a
/// vector that satisfy a predicate p and remove them.
/// @param vec The vector to be modified
/// @param p A binary predicate that takes two elements of the vector and
/// compares them
/// @return An optional pair of elements that were removed from the vector in
/// their original order
template <class T, class BinaryPredicate>
std::optional<std::pair<T, T>> remove_first_unordered_pair(std::vector<T> &vec,
                                                           BinaryPredicate p) {
  auto it = std::adjacent_find(vec.begin(), vec.end(), p);
  if (it != vec.end() && std::next(it) != vec.end()) {
    std::pair<T, T> erased_pair = {*it, *std::next(it)};
    vec.erase(it, std::next(it, 2)); // Erase the pair of elements
    return erased_pair;
  }
  return std::nullopt;
}

/// @brief A template function to check if a vector has no adjacent identical
/// elements
/// @param vec The vector to be checked
/// @param p A binary predicate that takes two elements of the vector and checks
/// for an additional condition (e.g., that both elements are fermions)
template <typename T>
bool has_no_adjacent_identical_terms(
    const std::vector<T> &vec, std::function<bool(const T &, const T &)> p =
                                   [](const T &, const T &) { return true; }) {
  // return std::adjacent_find(vec.begin(), vec.end()) == vec.end();
  auto it =
      std::adjacent_find(vec.begin(), vec.end(), [&p](const T &a, const T &b) {
        return a == b and p(a, b);
      });
  return it == vec.end();
}

class MyInt {
private:
  int m_;

public:
  MyInt(int m) : m_(m) { std::cout << "created " << m_ << std::endl; }

  // Copy constructor
  MyInt(const MyInt &my) : m_(my.m_) {
    std::cout << "copied " << m_ << std::endl;
  }

  // Move constructor
  MyInt(MyInt &&other) : m_(other.m_) {
    std::cout << "moved " << m_ << std::endl;
  }

  // Copy assignment operator
  MyInt &operator=(const MyInt &other) {
    std::cout << "copy assigned " << m_ << std::endl;
    return *this;
  }

  // Move assignment operator
  MyInt &operator=(MyInt &&other) {
    std::cout << "move assigned " << m_ << std::endl;
    return *this;
  }

  int get() { return m_; }
};

#endif // _wicked_helpers_h_
