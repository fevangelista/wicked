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
