#include <iostream>
#include <sstream>

#include "helpers.h"

std::string to_string(const std::vector<std::string> &strvec,
                      const std::string &sep) {
  if (strvec.size() == 0)
    return std::string();

  std::ostringstream ss;

  std::copy(strvec.begin(), strvec.end() - 1,
            std::ostream_iterator<std::string>(ss, sep.c_str()));
  ss << strvec.back();

  return ss.str();
}

std::string to_string(const scalar_t r) {
  if (r.numerator() == 0) {
    return "0";
  }
  if (r.denominator() == 1) {
    return std::to_string(r.numerator());
  }
  return std::to_string(r.numerator()) + "/" + std::to_string(r.denominator());
}

std::string to_latex(const scalar_t r) {
  if (r.numerator() == 0) {
    return "0";
  }
  if (r.denominator() == 1) {
    return std::to_string(r.numerator());
  }
  return "\frac{" + std::to_string(r.numerator()) + "}{" +
         std::to_string(r.denominator()) + "}";
}
