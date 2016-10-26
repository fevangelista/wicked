#ifndef _wicked_wicked_def_h_
#define _wicked_wicked_def_h_

#include <vector>

#include "rational.h"

/// Enums
enum TensorType { Amplitude, Integral, OneDensity, OneHole, Cumulant };

enum SymmetryType { Symmetric, Antisymmetric };

/// Rational numbers
using scalar_t = rational;

#endif // #ifndef _wicked_wicked_def_h_
