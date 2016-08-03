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

  WIndex(OrbitalSpaceType space, int p);

  // ==> Class public interface <==

  /// Return the orbital space type
  OrbitalSpaceType space() const { return index_.first; }

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

  std::pair<OrbitalSpaceType, int> index_;
};

/// Helper function that converts a vector of space types (OrbitalSpaceType)
/// into a vector of indices (WIndex) labeled staring with zero.
std::vector<WIndex>
make_indices_from_space_type(const std::vector<OrbitalSpaceType> &space);

#endif // _wicked_windex_h_
