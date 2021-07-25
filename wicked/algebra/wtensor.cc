#include <algorithm>
#include <iostream>
#include <regex>

#include "wicked-def.h"
#include "helpers.h"
#include "wtensor.h"

WTensor::WTensor(std::string label, const std::vector<Index> &lower,
                 const std::vector<Index> &upper, SymmetryType symmetry)
    : label_(label), lower_(lower), upper_(upper), symmetry_(symmetry) {}

int WTensor::symmetry_factor() const {
  return ::symmetry_factor(upper_) * ::symmetry_factor(lower_);
}

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

std::vector<Index> WTensor::indices() const {
  std::vector<Index> vec;
  for (const Index &idx : upper_) {
    vec.push_back(idx);
  }
  for (const Index &idx : lower_) {
    vec.push_back(idx);
  }
  // Remove repeated indices
  std::sort(vec.begin(), vec.end());
  vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
  return vec;
}

void WTensor::reindex(index_map_t &idx_map) {
  for (Index &idx : upper_) {
    if (idx_map.count(idx) > 0) {
      idx = idx_map[idx];
    }
  }
  for (Index &idx : lower_) {
    if (idx_map.count(idx) > 0) {
      idx = idx_map[idx];
    }
  }
}

std::string WTensor::str() const {
  std::vector<std::string> str_vec_upper;
  std::vector<std::string> str_vec_lower;
  for (const Index &index : upper_) {
    str_vec_upper.push_back(index.str());
  }
  for (const Index &index : lower_) {
    str_vec_lower.push_back(index.str());
  }
  return (label_ + "^{" + to_string(str_vec_upper, ",") + "}_{" +
          to_string(str_vec_lower, ",") + "}");
}

std::string WTensor::latex() const {
  std::vector<std::string> str_vec_upper;
  std::vector<std::string> str_vec_lower;
  for (const Index &index : upper_) {
    str_vec_upper.push_back(index.latex());
  }
  for (const Index &index : lower_) {
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
  for (const Index &index : upper_) {
    str_vec.push_back(index.ambit());
  }
  for (const Index &index : lower_) {
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
//  for (Index &index : upper_) {
//    str_vec.push_back(index.str());
//  }
//  for (Index &index : lower_) {
//    str_vec.push_back(index.str());
//  }
//  std::string ambit_label = label_;
//  ambit_label[0] = std::toupper(ambit_label[0]);
//  if (str_vec.size() == 0)
//    return (ambit_label);
//  return (ambit_label + "[\"" + join(str_vec, ",") + "\"]");
//}
