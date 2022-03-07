#ifndef _wicked_rational_h_
#define _wicked_rational_h_

#include <ostream>

#define USE_BOOST_RATIONAL 1
#if USE_BOOST_RATIONAL
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/rational.hpp>
using rational_t = boost::multiprecision::int1024_t;
#else
using rational_t = long long int;
#endif

// class boost_rational
//     : public boost::rational<boost::multiprecision::int1024_t> {

// public:
//   boost_rational(int i)
//       : boost::rational<boost::multiprecision::int1024_t>(i) {}
//   /// return a (nice) string representation, and optionally show the sign
//   std::string str(bool sign = false) const;
//   double to_double() const;
//   std::string compile(const std::string &format) const;
// };

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
  std::string latex() const;
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

rational make_rational_from_str(const std::string &s);

std::ostream &operator<<(std::ostream &os, const rational &rhs);

#endif // _wicked_rational_h_
