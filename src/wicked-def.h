#ifndef _wicked_wicked_def_h_
#define _wicked_wicked_def_h_

#include <vector>

/// Enums
enum TensorType { Amplitude, Integral, OneDensity, OneHole, Cumulant };

enum SymmetryType { Symmetric, Antisymmetric };

/// Rational numbers
using scalar_t = double;
//#include <boost/rational.hpp>
//typedef boost::rational<int> rational;

#endif // #ifndef _wicked_wicked_def_h_
