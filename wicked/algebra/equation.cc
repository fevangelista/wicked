#include <algorithm>
#include <format>
#include <iostream>

#include "equation.h"
#include "expression.h"
#include "helpers/helpers.h"
#include "sqoperator.h"
#include "tensor.h"
#include "wicked-def.h"

Equation::Equation(const SymbolicTerm &lhs, const SymbolicTerm &rhs,
                   scalar_t factor)
    : lhs_(lhs), rhs_(rhs), factor_(factor) {}

const SymbolicTerm &Equation::lhs() const { return lhs_; }

const SymbolicTerm &Equation::rhs() const { return rhs_; }

scalar_t Equation::rhs_factor() const { return factor_; }

Expression Equation::rhs_expression() const {
  Expression expr;
  expr.add(rhs(), rhs_factor());
  return expr;
}

bool Equation::operator==(Equation const &other) const {
  return ((lhs() == other.lhs()) and (rhs() == other.rhs()) and
          (rhs_factor() == other.rhs_factor()));
}

std::string Equation::str() const {
  std::vector<std::string> str_vec;
  str_vec.push_back(lhs_.str());
  str_vec.push_back("+=");
  str_vec.push_back(factor_.str());
  str_vec.push_back(rhs_.str());
  return (join(str_vec, " "));
}

std::string Equation::latex() const { return str(); }

std::string get_unique_index(const std::string &s,
                             std::map<std::string, std::string> &index_map,
                             std::vector<char> &unused_indices);

std::string
get_unique_tensor_indices(const Tensor &t,
                          std::map<std::string, std::string> &index_map,
                          std::vector<char> &unused_indices);

std::string get_unique_index(const std::string &s,
                             std::map<std::string, std::string> &index_map,
                             std::vector<std::string> &unused_indices) {
  // is this index (something like "i" or "o2") in the map? If not, figure out
  // what it corresponds to.
  if (index_map.count(s) == 0) {
    // a character
    if (s.size() == 1) {
      if (std::find(unused_indices.begin(), unused_indices.end(), s) !=
          unused_indices.end()) {
        // if this character is unused, use it
        index_map[s] = s;
      } else {
        // if it is used, grab the first available index
        index_map[s] = unused_indices.back();
      }
    } else {
      index_map[s] = unused_indices.back();
    }
    // erase this character from the available characters to avoid reusing
    unused_indices.erase(
        std::remove(unused_indices.begin(), unused_indices.end(), index_map[s]),
        unused_indices.end());
  }
  return index_map[s];
}

std::string
get_unique_tensor_indices(const Tensor &t,
                          std::map<std::string, std::string> &index_map,
                          std::vector<std::string> &unused_indices) {
  std::string indices;
  for (const auto &l : t.upper()) {
    indices += get_unique_index(l.latex(), index_map, unused_indices);
  }
  for (const auto &l : t.lower()) {
    indices += get_unique_index(l.latex(), index_map, unused_indices);
  }
  return indices;
}

std::string Equation::compile(const std::string &format, const std::string &optimize) const {
  if (format == "ambit") {
    std::vector<std::string> str_vec;
    str_vec.push_back(lhs_.compile(format) + " += " + factor_.compile(format));
    str_vec.push_back(rhs_.compile(format));
    return (join(str_vec, " * ") + ";");
  }

  if (format == "einsum") {
    std::vector<std::string> str_vec;
    const auto &lhs_tensor = lhs().tensors()[0];

    std::string lhs_tensor_label = lhs_tensor.label();
    for (const auto &l : lhs_tensor.upper()) {
      lhs_tensor_label += orbital_subspaces->label(l.space());
    }
    for (const auto &l : lhs_tensor.lower()) {
      lhs_tensor_label += orbital_subspaces->label(l.space());
    }

    str_vec.push_back(lhs_tensor_label +
                      " += " + std::format("{:.9f}", rhs_factor().to_double()) +
                      " * np.einsum(");

    std::map<std::string, std::string> index_map;
    std::vector<std::string> unused_indices = {
        "Z", "Y", "X", "W", "V", "U", "T", "S", "R", "Q", "P", "O", "N",
        "M", "L", "K", "J", "I", "H", "G", "F", "E", "D", "C", "B", "A",
        "z", "y", "x", "w", "v", "u", "t", "s", "r", "q", "p", "o", "n",
        "m", "l", "k", "j", "i", "h", "g", "f", "e", "d", "c", "b", "a"};

    std::vector<std::string> indices_vec;
    for (const auto &t : rhs().tensors()) {
      std::string tensor_indices =
          get_unique_tensor_indices(t, index_map, unused_indices);
      indices_vec.push_back(tensor_indices);
    }

    std::vector<std::string> args_vec;
    args_vec.push_back(
        "\"" + join(indices_vec, ",") + "->" +
        get_unique_tensor_indices(lhs_tensor, index_map, unused_indices) +
        "\"");
    for (const auto &t : rhs().tensors()) {
      std::string t_label = t.label() + "[\"";
      for (const auto &l : t.upper()) {
        t_label += orbital_subspaces->label(l.space());
      }
      for (const auto &l : t.lower()) {
        t_label += orbital_subspaces->label(l.space());
      }
      t_label += "\"]";
      args_vec.push_back(t_label);
    }
    str_vec.push_back(join(args_vec, ","));
    str_vec.push_back(",optimize="+optimize+")");
    return join(str_vec, "");
  }
  std::string msg = "Equation::compile() - the argument '" + format +
                    "' is not valid. Choices are 'ambit' or 'einsum'";
  throw std::runtime_error(msg);
  return "";
}

std::ostream &operator<<(std::ostream &os, const Equation &eterm) {
  os << eterm.str();
  return os;
}

void Equation::reindex(index_map_t &idx_map) {
  lhs_.reindex(idx_map);
  rhs_.reindex(idx_map);
}

void Equation::canonicalize() {
  auto [rhs_factor, idx_map] = rhs_.canonicalize();
  factor_ *= rhs_factor;
  lhs_.reindex(idx_map);
}