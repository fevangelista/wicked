#ifndef _wicked_wsqoperator_h_
#define _wicked_wsqoperator_h_

#include <string>

//#include "wicked-def.h"
#include "orbital_space.h"
#include "windex.h"

enum SQOperatorType { Creation, Annihilation };

class WSQOperator {

public:
  WSQOperator(SQOperatorType type, WIndex index);
  ~WSQOperator();

  bool operator<(WSQOperator const &other) const;
  bool operator==(WSQOperator const &other) const;

  /// Return the type of this operator
  SQOperatorType type() const;

  /// Return the index of this operator
  WIndex index() const;

  /// Reindex this operator
  void reindex(index_map_t &idx_map);

  /// Return a string representation
  std::string str() const;

  /// Return a LaTeX representation
  std::string latex() const;

  /// Return an ambit representation
  std::string ambit() const;

  friend std::ostream &operator<<(std::ostream &os, const WSQOperator &op);

private:
  std::pair<SQOperatorType, WIndex> operator_;
};

#endif // _wicked_wsqoperator_h_
