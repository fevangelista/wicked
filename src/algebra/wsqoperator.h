#ifndef _wicked_wsqoperator_h_
#define _wicked_wsqoperator_h_

#include "wicked-def.h"
#include "orbital_space.h"
#include "windex.h"

#include <string>

class WSQOperator {

public:
  WSQOperator(SQOperatorType type, WIndex index);
  ~WSQOperator();

  bool operator<(WSQOperator const &other) const;
  bool operator==(WSQOperator const &other) const;

  SQOperatorType type() const;
  WIndex index() const;

  /// Return a string representation
  std::string str() const;

  /// Return a LaTeX representation
  std::string latex() const;

  /// Return an ambit representation
  std::string ambit() const;

private:
  std::pair<SQOperatorType, WIndex> operator_;
};

#endif // _wicked_wsqoperator_h_
