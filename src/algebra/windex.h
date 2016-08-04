#ifndef _wicked_windex_h_
#define _wicked_windex_h_

#include <string>

#include "orbital_space.h"

/// This class represents an orbital index that appears in a second quantized
/// expression.  It holds information about the space type (via
/// OrbitalSpaceType) and the index numerical label.
class WIndex {
public:
  // ==> Constructors <==
  WIndex();

  WIndex(int space, int p);

  // ==> Class public interface <==

  /// Return the orbital space type
  int space() const { return index_.first; }

  /// Return the label of this index
  int index() const { return index_.second; }

  /// Comparison operator
  bool operator==(WIndex const &other) const;

  /// Comparison operator (used for sorting)
  bool operator<(WIndex const &other) const;

  /// Return a string representation
  std::string str() const;

  /// Return a LaTeX representation
  std::string latex() const;

  /// Return an ambit representation
  std::string ambit() const;

private:
  // ==> Class private data <==

  /// Store the orbital space type and label of an index (type,label)
  std::pair<int, int> index_;
};

/// Helper function that converts a vector of space types (OrbitalSpaceType)
/// into a vector of indices (WIndex) labeled staring with zero.
std::vector<WIndex>
make_indices_from_space_labels(const std::vector<std::string> &labels);

/// Helper function that counts the number of spaces in a vector of indices
std::vector<int> num_indices_per_space(const std::vector<WIndex> &indices);

#endif // _wicked_windex_h_
