#ifndef _wicked_index_h_
#define _wicked_index_h_

#include <map>
#include <string>

/// This class represents an orbital index that appears in a second quantized
/// expression.  It holds information about the space type (via
/// OrbitalSpaceType) and the index numerical label.
class Index {
public:
  // ==> Constructors <==
  Index();

  Index(int space, int p);

  // ==> Class public interface <==

  /// Return the orbital space type
  int space() const { return index_.first; }

  /// Return the position within a space
  int pos() const { return index_.second; }

  /// Comparison operator
  bool operator==(Index const &other) const;

  /// Comparison operator (used for sorting)
  bool operator<(Index const &other) const;

  /// @return a string representation
  /// (e.g., for index 1 of space 'o' returns 'o1')
  std::string str() const;

  /// @return a LaTeX representation
  /// This function either returns a pretty index (e.g., 'i')
  /// or a generic index (e.g., 'o1')
  std::string latex() const;

  /// Return an ambit (C++ code) representation (e.g., 'o1')
  std::string ambit() const;

private:
  // ==> Class private data <==

  /// Store the orbital space type and position in the space (space,p)
  std::pair<int, int> index_;
};

// A Index -> Index map used for reindexing
using index_map_t = std::map<Index, Index>;

// Helper functions

// /// Helper function to make an Index object from a space label and position
// Index make_index(const std::string &space, int p);

/// Helper function to make an Index object from a space label and position
Index make_index(const std::string &index);

/// Print to an output stream
std::ostream &operator<<(std::ostream &os, const Index &idx);

/// Canonicalize a set of indices
int canonicalize_indices(std::vector<Index> &indices);

/// Helper function that converts multiple vectors of space types
/// (OrbitalSpaceType) to vectors of indices (Index) labeled staring with zero.
/// Indices are assigned in an incremental way with no duplicates
std::vector<std::vector<Index>> make_indices_from_space_labels(
    const std::vector<std::vector<std::string>> &labels_space);

/// Helper function to convert a string to a Index object
Index string_to_index(const std::string &s);

/// A function that takes two lists of indices and creates a index map for the
/// second list that voids duplicates
index_map_t remap(const std::vector<Index> &idx_vec1,
                  const std::vector<Index> &idx_vec2);

/// Helper function that counts the number of spaces in a vector of indices
std::vector<int> num_indices_per_space(const std::vector<Index> &indices);

/// Return the symmetry factor of a product of indices
/// This is the product n1! x n2! x n3! x ... where ni is the number of indices
/// that belong to orbital space i
int symmetry_factor(const std::vector<Index> &indices);

#endif // _wicked_index_h_
