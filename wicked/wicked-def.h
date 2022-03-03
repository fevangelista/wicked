#ifndef _wicked_def_h_
#define _wicked_def_h_

#include <vector>

#include "rational.h"
#include <bitset>

#define DEBUG_PRINT 0

#if DEBUG_PRINT == 1
#define WPRINT(code)                                                           \
  { code }

#else
#define WPRINT(code)                                                           \
  {}
#endif

/// Rational numbers
using scalar_t = rational;

/// Bit array
using bitarray = std::bitset<64>;

#endif // #ifndef _wicked_def_h_
