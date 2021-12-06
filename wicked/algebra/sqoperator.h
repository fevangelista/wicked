#ifndef _wicked_sqoperator_h_
#define _wicked_sqoperator_h_

#include <string>

#include "index.h"
#include "orbital_space.h"

enum class SQOperatorType {
  Creation,
  Annihilation,
};

class SQOperator {

  using index_map_t = std::map<Index, Index>;

public:
  SQOperator(SQOperatorType type, Index index);
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
  FieldType field_type() const;

  /// Return the index of this operator
  Index index() const;

  /// Return the index of this operator
  int space() const;

  /// Reindex this operator
  void reindex(index_map_t &idx_map);

  /// Return a string representation
  std::string str() const;

  /// Return a LaTeX representation
  std::string latex() const;

  /// Return a compilable representation
  std::string compile(const std::string &format) const;

private:
  std::pair<SQOperatorType, Index> operator_;
};

// Helper functions

/// Helper function to make an SQOperator object from an index and the type
SQOperator make_sqoperator(const std::string &index, SQOperatorType type);

/// Print to an output stream
std::ostream &operator<<(std::ostream &os, const SQOperator &op);

#endif // _wicked_sqoperator_h_
