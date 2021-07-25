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

void rational::numerator(int value) { numerator_ = value; }

void rational::denominator(int value) { denominator_ = value; }

rational operator+(rational rhs) { return rhs; }

rational operator-(rational rhs) {
  rhs.numerator(-rhs.numerator());
  return rhs;
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

std::string rational::ambit() const {
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
