#include <algorithm>
#include <iostream>
#include <regex>

#include "wicked-def.h"
#include "helpers.h"
#include "wtensor.h"

WTensor::WTensor(std::string label, const std::vector<WIndex> &lower,
                 const std::vector<WIndex> &upper, SymmetryType symmetry)
    : label_(label), lower_(lower), upper_(upper), symmetry_(symmetry) {}

bool WTensor::operator<(WTensor const &other) const {
  // Compare the labels
  if (label_ < other.label_)
    return true;
  if (label_ > other.label_)
    return false;
  // Compare the lower indices
  if (lower_ < other.lower_)
    return true;
  if (lower_ > other.lower_)
    return false;
  return upper_ < other.upper_;
}

bool WTensor::operator==(WTensor const &other) const {
  return (label_ == other.label_) and (lower_ == other.lower_) and
         (upper_ == other.upper_);
}

std::vector<WIndex> WTensor::indices() const {
  std::vector<WIndex> vec;
  for (const WIndex &idx : upper_) {
    vec.push_back(idx);
  }
  for (const WIndex &idx : lower_) {
    vec.push_back(idx);
  }
  // Remove repeated indices
  std::sort(vec.begin(), vec.end());
  vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
  return vec;
}

void WTensor::reindex(index_map_t &idx_map) {
  for (WIndex &idx : upper_) {
    if (idx_map.count(idx) > 0) {
      idx = idx_map[idx];
    }
  }
  for (WIndex &idx : lower_) {
    if (idx_map.count(idx) > 0) {
      idx = idx_map[idx];
    }
  }
}

std::string WTensor::str() const {
  std::vector<std::string> str_vec_upper;
  std::vector<std::string> str_vec_lower;
  for (const WIndex &index : upper_) {
    str_vec_upper.push_back(index.str());
  }
  for (const WIndex &index : lower_) {
    str_vec_lower.push_back(index.str());
  }
  return (label_ + "^{" + to_string(str_vec_upper, ",") + "}_{" +
          to_string(str_vec_lower, ",") + "}");
}

std::string WTensor::latex() const {
  std::vector<std::string> str_vec_upper;
  std::vector<std::string> str_vec_lower;
  for (const WIndex &index : upper_) {
    str_vec_upper.push_back(index.latex());
  }
  for (const WIndex &index : lower_) {
    str_vec_lower.push_back(index.latex());
  }

  std::regex num_re("1|2|3|4|5|6|7|8|9|0");
  std::string label_wo_num = std::regex_replace(label_, num_re, "");

  std::vector<std::string> greek_letters{
      "alpha", "beta",    "gamma",   "delta", "epsilon", "zeta",
      "eta",   "theta",   "iota",    "kappa", "lambda",  "mu",
      "nu",    "xi",      "omicron", "pi",    "rho",     "sigma",
      "tau",   "upsilon", "phi",     "chi",   "psi",     "omega"};
  if (std::find(greek_letters.begin(), greek_letters.end(), label_wo_num) !=
      greek_letters.end()) {
    return ("\\" + label_wo_num + "^{" + to_string(str_vec_upper, " ") + "}_{" +
            to_string(str_vec_lower, " ") + "}");
  }
  return (label_wo_num + "^{" + to_string(str_vec_upper, " ") + "}_{" +
          to_string(str_vec_lower, " ") + "}");
}

std::string WTensor::ambit() const {
  std::vector<std::string> str_vec;
  for (const WIndex &index : upper_) {
    str_vec.push_back(index.ambit());
  }
  for (const WIndex &index : lower_) {
    str_vec.push_back(index.ambit());
  }

  return (str_vec.size() > 0 ? (label_ + "[" + to_string(str_vec, ",") + "]")
                             : label_);
}

std::ostream &operator<<(std::ostream &os, const WTensor &tensor) {
  os << tensor.str();
  return os;
}

// std::string WTensor::ambit() {
//  std::vector<std::string> str_vec;
//  for (WIndex &index : upper_) {
//    str_vec.push_back(index.str());
//  }
//  for (WIndex &index : lower_) {
//    str_vec.push_back(index.str());
//  }
//  std::string ambit_label = label_;
//  ambit_label[0] = std::toupper(ambit_label[0]);
//  if (str_vec.size() == 0)
//    return (ambit_label);
//  return (ambit_label + "[\"" + join(str_vec, ",") + "\"]");
//}
