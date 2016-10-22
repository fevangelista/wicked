#ifndef _wicked_wtensor_h_
#define _wicked_wtensor_h_

#include <string>
#include <vector>

#include "wicked-def.h"
#include "windex.h"

/// This class represents a tensor labeled with orbital indices.
/// It holds information about the label and the indices of the tensor.
class WTensor {

public:
  // ==> Constructors <==
  explicit WTensor() {}

  WTensor(std::string label, const std::vector<WIndex> &lower,
                   const std::vector<WIndex> &upper,
                   SymmetryType symmetry = Antisymmetric);

  // ==> Class public interface <==

  /// Return a reference to the label
  const std::string &label() const { return label_; }

  /// Return a reference to the lower indices
  const std::vector<WIndex> &lower() const { return lower_; }

  /// Return a reference to the upper indices
  const std::vector<WIndex> &upper() const { return upper_; }

  /// Set the lower indices
  void set_lower(const std::vector<WIndex> &indices) { lower_ = indices; }

  /// Set the upper indices
  void set_upper(const std::vector<WIndex> &indices) { upper_ = indices; }

  /// Return a vector containing all indices
  std::vector<WIndex> indices() const;

  /// Reindex this tensor
  void reindex(index_map_t &idx_map);

  /// Return the rank of the tensor
  int rank() const { return lower_.size() + upper_.size(); }

  /// Comparison operator used for sorting
  bool operator<(WTensor const &other) const;

  /// Comparison operator used for sorting
  bool operator==(WTensor const &other) const;

  /// Return a string representation
  std::string str() const;

  /// Return a LaTeX representation
  std::string latex() const;

  /// Return an ambit representation
  std::string ambit() const;

private:
  // ==> Class private data <==

  std::string label_;
  std::vector<WIndex> lower_;
  std::vector<WIndex> upper_;
  SymmetryType symmetry_;
};


/// Print to an output stream
std::ostream &operator<<(std::ostream &os, const WTensor &tensor);

#endif // _wicked_wtensor_h_
