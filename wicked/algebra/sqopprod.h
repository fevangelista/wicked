#ifndef _wicked_sqopprod_h_
#define _wicked_sqopprod_h_

#include <array>
#include <string>

#include "index.h"
#include "wicked-def.h"

template <std::size_t N>
bool operator<(const std::bitset<N> &x, const std::bitset<N> &y) {
  for (int i = N - 1; i >= 0; i--) {
    if (x[i] ^ y[i])
      return y[i];
  }
  return false;
}

/// A class to represent a product of fermionic second quantized operators
class SQOpProd {

  using index_map_t = std::map<Index, Index>;

public:
  SQOpProd();
  SQOpProd(const std::vector<Index> &cre, const std::vector<Index> &ann);

  int nops() const;

  bool operator<(SQOpProd const &other) const;
  bool operator==(SQOpProd const &other) const;

  /// Return a string representation
  std::string str() const;

  /// Return a LaTeX representation
  std::string latex() const;

private:
  void citerate(const std::function<void(bool, const size_t &, const size_t &)>
                    &func) const;

  void citerate_spaces(const std::function<void(bool, const size_t &,
                                                const bitarray &)> &func) const;

  static constexpr size_t maxspaces = 3;

  std::array<bitarray, maxspaces> cre_{};
  std::array<bitarray, maxspaces> ann_{};
};

// Helper functions

/// Helper function to make an SQOpProd object from two lists of indices
SQOpProd make_sqopprod(const std::vector<Index> &cre,
                       const std::vector<Index> &ann);

/// Helper function to make an SQOpProd object from two lists of indices
SQOpProd make_sqopprod(const std::vector<std::string> &cre_str,
                       const std::vector<std::string> &ann_str);

/// Print to an output stream
std::ostream &operator<<(std::ostream &os, const SQOpProd &opprod);

#endif // _wicked_sqopprod_h_
