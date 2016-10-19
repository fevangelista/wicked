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
  /// set the numerator
  void numerator(int value);
  /// set the denominator
  void denominator(int value);
  /// return a string representation
  std::string str() const;
  /// return a LaTeX representation
  std::string latex() const;

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
  int numerator_;
  /// the denominator
  int denominator_;
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

std::ostream &operator<<(std::ostream &os, const rational &rhs);

#endif // _wicked_rational_h_
