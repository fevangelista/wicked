#ifndef _wicked_sqoperator_h_
#define _wicked_sqoperator_h_

#include <string>

#include "index.h"

enum class SQOperatorType {
  Creation,
  Annihilation,
};

enum class SQOperatorStatistics {
  Fermion,
  Boson,
};

class SQOperator {

  using index_map_t = std::map<Index, Index>;

public:
  SQOperator(SQOperatorType type, Index index,
             SQOperatorStatistics statistics = SQOperatorStatistics::Fermion);
  ~SQOperator();

  bool operator<(SQOperator const &other) const;
  bool operator==(SQOperator const &other) const;

  /// Return the type of this operator
  SQOperatorType type() const;

  /// @return the symbol of the operator
  std::string op_symbol() const;

  /// @return is this a creation operator
  bool is_creation() const;

  /// Return the type of this operator
  SQOperatorStatistics statistics() const;

  /// Return the index of this operator
  Index index() const;

  /// Reindex this operator
  void reindex(index_map_t &idx_map);

  /// Return a string representation
  std::string str() const;

  /// Return a LaTeX representation
  std::string latex() const;

  /// Return an ambit (C++ code) representation
  std::string ambit() const;

private:
  std::pair<SQOperatorType, Index> operator_;
  SQOperatorStatistics statistics_;
};

// Helper functions

/// Helper function to make an Index object from a space label and position
SQOperator make_sqoperator(const std::string &space, int p, SQOperatorType type,
                           SQOperatorStatistics statistics);

/// Print to an output stream
std::ostream &operator<<(std::ostream &os, const SQOperator &op);

#endif // _wicked_sqoperator_h_
