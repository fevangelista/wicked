#pragma once

#include <ostream>

/// USE_BOOST_1024_INT is set to true by CMake if boost is found
/// otherwise, we use long long int.
/// Note that long long int is not sufficient for all cases.
/// For example, the CC equations with up to 8-body excitation operators cannot
/// be handled with long long int.
#if USE_BOOST_1024_INT
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/rational.hpp>
using rational_t = boost::multiprecision::int1024_t;
#else
using rational_t = long long int;
#endif

/// A class for rational numbers
class rational {

public:
  /// initialize with zero
  rational();
  /// initialize with rational (numerator/denominator)
  rational(rational_t numerator, rational_t denominator);
  /// initialize with integer (numerator)
  rational(rational_t numerator);
  /// initialize with integer (numerator)
  rational(int numerator);
  /// return the numerator
  rational_t numerator() const;
  /// return the denominator
  rational_t denominator() const;
  /// return this converted to a double
  double to_double() const;
  /// return a (nice) string representation, and optionally show the sign
  std::string str(bool sign = false) const;
  /// return a string representation
  std::string repr() const;
  /// return a LaTeX representation
  std::string latex(bool first) const;
  /// return a compilable representation
  std::string compile(const std::string &format) const;

  /// addition assignment
  rational &operator+=(const rational &rhs);
  /// subtraction assignment
  rational &operator-=(const rational &rhs);
  /// multiplication assignment
  rational &operator*=(const rational &rhs);
  /// division assignment
  rational &operator/=(const rational &rhs);

private:
  /// the numerator
  rational_t numerator_;
  /// the denominator
  rational_t denominator_;
  /// reduce the rational number
  void reduce();
};

/// equal to
bool operator==(const rational &lhs, const rational &rhs);
/// not equal to
bool operator!=(const rational &lhs, const rational &rhs);
/// unary plus
rational operator+(rational rhs);
/// unary minus
rational operator-(rational rhs);
/// addition
rational operator+(rational lhs, const rational &rhs);
/// subtraction
rational operator-(rational lhs, const rational &rhs);
/// multiplication
rational operator*(rational lhs, const rational &rhs);
/// division
rational operator/(rational lhs, const rational &rhs);
/// make a rational from a string
rational make_rational_from_str(const std::string &s);
/// output a rational to a stream
std::ostream &operator<<(std::ostream &os, const rational &rhs);
/// return true if boost rational is used
bool use_boost_1024_int();
