#include <regex>

#include "rational.h"

rational::rational() : numerator_(0), denominator_(1) {}

rational::rational(int numerator) {
  numerator_ = numerator;
  denominator_ = 1;
}

rational::rational(int numerator, int denominator) {
  numerator_ = denominator < 0 ? -numerator : numerator;
  denominator_ = denominator < 0 ? -denominator : denominator;
  reduce();
}

int rational::numerator() const { return numerator_; }

int rational::denominator() const { return denominator_; }

double rational::to_double() const {
  return static_cast<double>(numerator()) / static_cast<double>(denominator());
}

rational operator+(rational rhs) { return rhs; }

rational operator-(rational rhs) {
  return rational(-rhs.numerator(), rhs.denominator());
}

rational &rational::operator+=(const rational &rhs) {
  numerator_ = rhs.denominator_ * numerator_ + rhs.numerator_ * denominator_;
  denominator_ = rhs.denominator_ * denominator_;
  reduce();
  return *this;
}

rational &rational::operator-=(const rational &rhs) {
  numerator_ = rhs.denominator_ * numerator_ - rhs.numerator_ * denominator_;
  denominator_ = rhs.denominator_ * denominator_;
  reduce();
  return *this;
}

rational &rational::operator*=(const rational &rhs) {
  numerator_ *= rhs.numerator();
  denominator_ *= rhs.denominator();
  reduce();
  return *this;
}

rational &rational::operator/=(const rational &rhs) {
  numerator_ *= rhs.denominator();
  denominator_ *= rhs.numerator();
  reduce();
  return *this;
}

std::string rational::str(bool sign) const {
  std::string s;
  if (numerator_ == 0) {
    s = "0";
  } else {
    if (sign and (numerator_ > 0)) {
      s = '+';
    }
    if (denominator_ == 1) {
      if (numerator_ == 1) {
      } else if (numerator_ == -1) {
        s += "-";
      } else {
        s += std::to_string(numerator_);
      }
    } else {
      s += std::to_string(numerator_) + "/" + std::to_string(denominator_);
    }
  }
  return s;
}

std::string rational::repr() const {
  if (numerator_ == 0) {
    return "0";
  }
  std::string s = (numerator_ > 0) ? "+" : "-";
  if (denominator_ == 1) {
    s += std::to_string(std::abs(numerator_));
  } else {
    s += std::to_string(std::abs(numerator_)) + "/" +
         std::to_string(denominator_);
  }
  return s;
}

std::string rational::latex() const {
  std::string s;
  if (numerator_ == 0) {
    s = "0";
  } else {
    if (denominator_ == 1) {
      if (numerator_ == 1) {
        s += "+";
      } else if (numerator_ == -1) {
        s += "-";
      } else {
        s += std::to_string(numerator_);
      }
    } else {
      if (numerator_ > 0) {
        s += "+";
      } else {
        s += "-";
      }
      s += "\\frac{" + std::to_string(std::abs(numerator_)) + "}{" +
           std::to_string(denominator_) + "}";
    }
  }
  return s;
}

std::string rational::compile(const std::string &format) const {
  return std::to_string(static_cast<double>(numerator_) /
                        static_cast<double>(denominator_));
}

rational operator+(rational lhs, const rational &rhs) {
  lhs += rhs;
  return lhs;
}

rational operator-(rational lhs, const rational &rhs) {
  lhs -= rhs;
  return lhs;
}

rational operator*(rational lhs, const rational &rhs) {
  lhs *= rhs;
  return lhs;
}

rational operator/(rational lhs, const rational &rhs) {
  lhs /= rhs;
  return lhs;
}

bool operator==(const rational &lhs, const rational &rhs) {
  return ((lhs.numerator() == rhs.numerator()) and
          (lhs.denominator() == rhs.denominator()));
}

std::ostream &operator<<(std::ostream &os, const rational &rhs) {
  if (rhs.numerator() == 0) {
    os << '0';
  } else {
    if (rhs.denominator() == 1) {
      os << rhs.numerator();
    } else {
      os << rhs.numerator() << '/' << rhs.denominator();
    }
  }
  return os;
}

void rational::reduce() {
  int n = numerator_ < 0 ? -numerator_ : numerator_;
  int d = denominator_;
  int largest = n > d ? n : d;

  int gcd = 0; // greatest common divisor

  for (int loop = largest; loop >= 2; loop--)
    if (numerator_ % loop == 0 && denominator_ % loop == 0) {
      gcd = loop;
      break;
    }

  if (gcd != 0) {
    numerator_ /= gcd;
    denominator_ /= gcd;
  }
}

#include <iostream>

rational make_rational_from_str(const std::string &s) {
  std::smatch sm;
  auto factor_re = std::regex("^\\s*([+-])?(\\d*)?\\/?(\\d*)?\\s*");
  auto m = std::regex_match(s, sm, factor_re);
  if (not m) {
    throw std::runtime_error("\nCould not convert the string " + s +
                             " to a rational object");
  }
  std::string sign = sm[1];
  std::string numerator_str = sm[2];
  std::string denominator_str = sm[3];
  int numerator = 1, denominator = 1;
  if (sign == "-") {
    numerator *= -1;
  }
  if (numerator_str.size() > 0) {
    numerator *= std::stoi(numerator_str);
  }
  if (denominator_str.size() > 0) {
    denominator *= std::stoi(denominator_str);
  }
  return rational(numerator, denominator);
}
