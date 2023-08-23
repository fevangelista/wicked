#include <iostream>
#include <iterator>
#include <sstream>

#include "helpers.h"

using std::regex;
using std::sregex_token_iterator;
using std::string;

std::string join(const std::vector<std::string> &strvec,
                 const std::string &sep) {
  if (strvec.size() == 0)
    return std::string();

  std::ostringstream ss;

  std::copy(strvec.begin(), strvec.end() - 1,
            std::ostream_iterator<std::string>(ss, sep.c_str()));
  ss << strvec.back();

  return ss.str();
}

// trim from start
inline std::string &ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char c) {
            return not std::isspace(c);
          }));
  return s;
}

// trim from end
inline std::string &rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](unsigned char c) { return not std::isspace(c); })
              .base(),
          s.end());
  return s;
}

// trim from both ends
inline std::string &trim(std::string &s) { return ltrim(rtrim(s)); }

std::vector<std::string> split(const std::string &s, regex re) {
  sregex_token_iterator it(s.begin(), s.end(), re, -1);
  sregex_token_iterator reg_end;

  std::vector<std::string> result;
  for (; it != reg_end; ++it) {
    result.push_back(it->str());
  }
  return result;
}

std::vector<std::string> findall(const string &s, const std::regex &regex) {
  std::vector<std::string> result;
  try {
    std::sregex_iterator next(s.begin(), s.end(), regex);
    std::sregex_iterator end;
    std::vector<std::string> matches;
    for (; next != end; ++next) {
      std::smatch match = *next;
      for (size_t k = 1; k < match.size(); ++k) {
        result.push_back(match[k]);
      }
    }
  } catch (std::regex_error &e) {
    // Syntax error in the regular expression
  }
  return result;
}

std::vector<std::string> split_indices(const std::string &idx) {
  std::istringstream f(idx);
  std::string s;
  std::vector<std::string> v;

  while (std::getline(f, s, ',')) {
    std::string trimmed = trim(s);
    v.push_back(trimmed);
  }

  return v;
}
