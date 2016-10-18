#ifndef _wicked_helpers_h_
#define _wicked_helpers_h_

#include <iostream>
#include <string>
#include <vector>

#include "wicked-def.h"

// Returns a comma separated list of the indices
std::string to_string(const std::vector<std::string> &svec,
                      const std::string &sep = ",");

std::string to_string(const scalar_t r);
std::string to_latex(const scalar_t r);

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
      : vec_(vec), last(vec.size()), iter(0) {}

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

//{
//  return enumerate2<T,A>(vec);
//}

#endif // _wicked_helpers_h_
