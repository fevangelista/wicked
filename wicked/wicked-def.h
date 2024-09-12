#pragma once

#include <vector>

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
#include "helpers/rational.h"
using scalar_t = rational;
