#ifndef _wicked_rational_h_
#define _wicked_rational_h_

#include <ostream>

/// A class for rational numbers
class rational {
public:
  /// initialize with zero
  rational();
  /// initialize with rational (numerator/denominator)
  rational(int numerator, int denominator);
  /// initialize with integer (numerator)
  rational(int numerator);
  /// return the numerator
  int numerator() const;
  /// return the denominator
  int denominator() const;
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
  long long int numerator_;
  /// the denominator
  long long int denominator_;
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
