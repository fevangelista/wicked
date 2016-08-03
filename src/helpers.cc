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

//std::string to_string(const rational r) {
//  if (r.numerator() == 0) {
//    return "0";
//  }
//  if (r.denominator() == 1) {
//    return to_string(r.numerator());
//  }
//  return to_string(r.numerator()) + "/" + to_string(r.denominator());
//}
