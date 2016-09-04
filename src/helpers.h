#ifndef _wicked_helpers_h_
#define _wicked_helpers_h_

#include <string>
#include <vector>

#include "wicked-def.h"

// Returns a comma separated list of the indices
std::string to_string(const std::vector<std::string> &svec,
                      const std::string &sep = ",");

class range {
private:
  int last;
  int iter;

public:
  range(int end) : last(end), iter(0) {}

  // Iterable functions
  const range &begin() const { return *this; }
  const range &end() const { return *this; }

  // Iterator functions
  bool operator!=(const range &) const { return iter < last; }
  void operator++() { ++iter; }
  int operator*() const { return iter; }
};

//template<std::vector<class T>>
//class enumerate {
//private:
//  int last;
//  int iter;

//public:
//  range(T& vec) : last(end), iter(0) {}

//  // Iterable functions
//  const range &begin() const { return *this; }
//  const range &end() const { return *this; }

//  // Iterator functions
//  bool operator!=(const range &) const { return iter < last; }
//  void operator++() { ++iter; }
//  int operator*() const { return iter; }
//};

#endif // _wicked_helpers_h_
