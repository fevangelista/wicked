#include <algorithm>
#include <iostream>
#include <regex>

#include "helpers/helpers.h"
#include "helpers/orbital_space.h"

#include "tensor.h"
#include "wicked-def.h"

Tensor::Tensor(const std::string &label, const std::vector<Index> &lower,
               const std::vector<Index> &upper, SymmetryType symmetry)
    : label_(label), lower_(lower), upper_(upper), symmetry_(symmetry) {}

std::vector<std::pair<int, int>> Tensor::signature() const {
  std::vector<std::pair<int, int>> result(orbital_subspaces->num_spaces(),
                                          std::pair(0, 0));
  for (const Index &idx : upper_) {
    result[idx.space()].first += 1;
  }
  for (const Index &idx : lower_) {
    result[idx.space()].second += 1;
  }
  return result;
}

int Tensor::symmetry_factor() const {
  return ::symmetry_factor(upper_) * ::symmetry_factor(lower_);
}

bool Tensor::operator<(Tensor const &other) const {
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

bool Tensor::operator==(Tensor const &other) const {
  return (label_ == other.label_) and (lower_ == other.lower_) and
         (upper_ == other.upper_);
}

std::vector<Index> Tensor::indices() const {
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

void Tensor::reindex(index_map_t &idx_map) {
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

scalar_t Tensor::canonicalize() {
  if (symmetry_ == SymmetryType::Nonsymmetric) {
    throw std::runtime_error(
        "Tensor::canonicalize cannot canonicalize a nonsymmetric tensor");
  }
  scalar_t sign = 1;
  auto upper_indices = this->upper();
  sign *= canonicalize_indices(upper_indices, false);
  auto lower_indices = this->lower();
  sign *= canonicalize_indices(lower_indices, false);
  this->set_upper(upper_indices);
  this->set_lower(lower_indices);
  return (symmetry_ == SymmetryType::Antisymmetric) ? sign : scalar_t(1);
}

std::string Tensor::str() const {
  std::vector<std::string> str_vec_upper;
  std::vector<std::string> str_vec_lower;
  for (const Index &index : upper_) {
    str_vec_upper.push_back(index.str());
  }
  for (const Index &index : lower_) {
    str_vec_lower.push_back(index.str());
  }
  return (label_ + "^{" + join(str_vec_upper, ",") + "}_{" +
          join(str_vec_lower, ",") + "}");
}

std::string Tensor::latex() const {
  std::vector<std::string> str_vec_upper;
  std::vector<std::string> str_vec_lower;
  for (const Index &index : upper_) {
    str_vec_upper.push_back(index.latex());
  }
  for (const Index &index : lower_) {
    str_vec_lower.push_back(index.latex());
  }

  // read the label. Here we try to separate the name (e.g., lambda) from the
  // subscript (eg. 1). For greek letters we omit the subscript.
  std::smatch sm;
  auto m = std::regex_match(label_, sm, std::regex("([a-zA-Z]+)[_]?(\\d+)?"));
  if (not m) {
    throw std::runtime_error("\nCould not parse tensor label " + label_);
  }
  std::string symbol = sm[1];
  std::string raw_subscript = sm[2];
  std::vector<std::string> greek{"alpha",  "beta", "gamma", "delta", "epsilon",
                                 "zeta",   "eta",  "theta", "iota",  "kappa",
                                 "lambda", "mu",   "nu",    "xi",    "omicron",
                                 "pi",     "rho",  "sigma", "tau",   "upsilon",
                                 "phi",    "chi",  "psi",   "omega"};
  bool is_greek_symbol =
      (std::find(greek.begin(), greek.end(), symbol) != greek.end());
  std::string prefix = is_greek_symbol ? "{\\" : "{";
  std::string subscript =
      is_greek_symbol
          ? ""
          : (raw_subscript.size() == 0 ? "" : "_{" + raw_subscript + "}");
  return prefix + symbol + subscript + "}^{" + join(str_vec_upper, " ") +
         "}_{" + join(str_vec_lower, " ") + "}";
}

std::string Tensor::compile(const std::string &format) const {
  std::vector<std::string> str_vec;
  for (const Index &index : upper_) {
    str_vec.push_back(index.compile(format));
  }
  for (const Index &index : lower_) {
    str_vec.push_back(index.compile(format));
  }

  return (str_vec.size() > 0 ? (label_ + "[" + join(str_vec, ",") + "]")
                             : label_);
}

std::ostream &operator<<(std::ostream &os, const Tensor &tensor) {
  os << tensor.str();
  return os;
}

Tensor make_tensor(const std::string &label,
                   const std::vector<std::string> &lower,
                   const std::vector<std::string> &upper,
                   SymmetryType symmetry) {
  std::vector<Index> lower_indices;
  for (const auto &l : lower) {
    lower_indices.emplace_back(make_index_from_str(l));
  }
  std::vector<Index> upper_indices;
  for (const auto &u : upper) {
    upper_indices.emplace_back(make_index_from_str(u));
  }
  return Tensor(label, lower_indices, upper_indices, symmetry);
}

Tensor make_tensor_from_str(const std::string &s, SymmetryType symmetry) {
  std::smatch sm;
  auto tensor_re =
      std::regex("([a-zA-Z0-9]+)\\^\\{([\\w,\\s]*)\\}_\\{([\\w,\\s]*)\\}");
  auto m = std::regex_match(s, sm, tensor_re);
  if (not m) {
    throw std::runtime_error("\nCould not convert the string " + s +
                             " to a Tensor object");
  }
  std::string label = sm[1];
  auto upper = make_indices_from_str(sm[2]);
  auto lower = make_indices_from_str(sm[3]);
  return Tensor(label, lower, upper, symmetry);
}

// std::string Tensor::compile() {
//  std::vector<std::string> str_vec;
//  for (Index &index : upper_) {
//    str_vec.push_back(index.str());
//  }
//  for (Index &index : lower_) {
//    str_vec.push_back(index.str());
//  }
//  std::string compile_label = label_;
//  compile_label[0] = std::toupper(compile_label[0]);
//  if (str_vec.size() == 0)
//    return (compile_label);
//  return (compile_label + "[\"" + join(str_vec, ",") + "\"]");
//}
