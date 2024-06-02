#include <numeric>
#include <regex>

#if USE_BOOST_1024_INT
#include "boost/lexical_cast.hpp"
#endif

#include "rational.h"

rational::rational() : numerator_(0), denominator_(1) {}

rational::rational(int numerator) {
  numerator_ = numerator;
  denominator_ = 1;
}

rational::rational(rational_t numerator) {
  numerator_ = numerator;
  denominator_ = 1;
}

rational::rational(rational_t numerator, rational_t denominator) {
  // enforce a positive denominator
  numerator_ = denominator < 0 ? -numerator : numerator;
  denominator_ = denominator < 0 ? -denominator : denominator;
  // bring to canonical form
  reduce();
}

rational_t rational::numerator() const { return numerator_; }

rational_t rational::denominator() const { return denominator_; }

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
      if (numerator_ == -1) {
        s += "-";
      } else {
        if (numerator_ != 1) {
#if USE_BOOST_1024_INT
          s += boost::lexical_cast<std::string>(numerator_);
#else
          s += std::to_string(numerator_);
#endif
        }
      }
    } else {
#if USE_BOOST_1024_INT
      s += boost::lexical_cast<std::string>(numerator_) + "/" +
           boost::lexical_cast<std::string>(denominator_);
#else
      s += std::to_string(numerator_) + "/" + std::to_string(denominator_);
#endif
    }
  }
  return s;
}

std::string rational::repr() const {
#if USE_BOOST_1024_INT
  return "rational(" + boost::lexical_cast<std::string>(numerator_) + "," +
         boost::lexical_cast<std::string>(denominator_) + ")";
#else
  return "rational(" + std::to_string(numerator_) + "," +
         std::to_string(denominator_) + ")";
#endif
}

std::string rational::latex(bool first) const {
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
#if USE_BOOST_1024_INT
        s += (first ? "" : (numerator_ > 0 ? "+" : "")) +
             boost::lexical_cast<std::string>(numerator_);
#else
        s += std::to_string(numerator_);
#endif
      }
    } else {
      if (numerator_ > 0) {
        s += "+";
      } else {
        s += "-";
      }
#if USE_BOOST_1024_INT
      s += "\\frac{" + boost::lexical_cast<std::string>(abs(numerator_)) +
           "}{" + boost::lexical_cast<std::string>(denominator_) + "}";
#else
      s += "\\frac{" + std::to_string(std::abs(numerator_)) + "}{" +
           std::to_string(denominator_) + "}";
#endif
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
  if ((lhs.numerator() == 0) and (rhs.numerator() == 0))
    return true;
  return ((lhs.numerator() == rhs.numerator()) and
          (lhs.denominator() == rhs.denominator()));
}

bool operator!=(const rational &lhs, const rational &rhs) {
  return (not(lhs == rhs));
}

std::ostream &operator<<(std::ostream &os, const rational &rhs) {
  os << rhs.str(false);
  return os;
}

void rational::reduce() {
// find the gcd
#if USE_BOOST_1024_INT
  rational_t gcd = boost::gcd(numerator_, denominator_);
#else
  rational_t gcd = std::gcd(numerator_, denominator_);
#endif
  // divide numerator and denominator by the gcd
  if (gcd > 1) {
    numerator_ /= gcd;
    denominator_ /= gcd;
  }
}

#include <iostream>

rational make_rational_from_str(const std::string &s) {
  std::smatch sm;
  auto factor_re = std::regex("^\\s*([+-])?(\\d*)?(\\/)?(\\d*)?\\s*");
  auto m = std::regex_match(s, sm, factor_re);
  if (not m) {
    throw std::runtime_error("\nCould not convert the string " + s +
                             " to a rational object");
  }
  std::string sign = sm[1];
  std::string numerator_str = sm[2];
  std::string division = sm[3];
  std::string denominator_str = sm[4];
  int numerator = 1, denominator = 1;
  if (sign == "-") {
    numerator *= -1;
  }
  // if we have a division sign
  if (division.size() != 0) {
    // make sure there is a numerator
    if ((numerator_str.size() == 0) or (denominator_str.size() == 0))
      throw std::runtime_error("\nCould not convert the string " + s +
                               " to a rational object");
  }
  if (numerator_str.size() > 0) {
    numerator *= std::stoi(numerator_str);
  }
  if ((division == "/") and (denominator_str.size() > 0)) {
    denominator *= std::stoi(denominator_str);
  }
  return rational(numerator, denominator);
}

/// return true if boost rational is used
#if USE_BOOST_1024_INT
bool use_boost_1024_int() { return true; }
#else
bool use_boost_1024_int() { return false; }
#endif