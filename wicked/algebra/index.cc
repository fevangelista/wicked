#include <algorithm>

#include "combinatorics.h"
#include "helpers.h"
#include "index.h"
#include "orbital_space.h"

Index::Index() : index_(std::make_pair(-1, -1)) {}

Index::Index(int space, int p) : index_(std::make_pair(space, p)) {}

bool Index::operator==(Index const &other) const {
  return index_ == other.index_;
}

bool Index::operator<(Index const &other) const {
  return index_ < other.index_;
}

std::string Index::str() const {
  return osi->label(space()) + std::to_string(pos());
}

std::string Index::latex() const { return osi->index_label(space(), pos()); }

std::string Index::compile(const std::string &format) const { return str(); }

std::ostream &operator<<(std::ostream &os, const Index &idx) {
  os << idx.str();
  return os;
}

scalar_t canonicalize_indices(std::vector<Index> &indices, bool reversed) {
  std::vector<std::pair<Index, int>> idx_pos;
  for (const auto &[n, index] : enumerate(indices)) {
    idx_pos.push_back(std::make_pair(index, n));
  }
  // when reversed == false, we sort in increasing order
  // when reversed == true, we sort in decreasing order
  if (reversed) {
    std::sort(idx_pos.rbegin(), idx_pos.rend());
  } else {
    std::sort(idx_pos.begin(), idx_pos.end());
  }
  std::vector<int> perm;
  indices.clear();
  for (const auto &[idx, pos] : idx_pos) {
    indices.push_back(idx);
    perm.push_back(pos);
  }
  return permutation_sign(perm);
}

std::vector<std::vector<Index>> make_indices_from_space_labels(
    const std::vector<std::vector<char>> &labels_space) {
  std::vector<std::vector<Index>> indices;
  std::vector<int> space_count(osi->num_spaces());
  for (const auto &labels : labels_space) {
    std::vector<Index> indices_space;
    for (const auto &label : labels) {
      int space = osi->label_to_space(label);
      space_count[space] += 1;
      int p = space_count[space];
      indices_space.push_back(Index(space, p));
    }
    indices.push_back(indices_space);
  }
  return indices;
}

Index make_index_from_str(const std::string &s) {
  std::smatch sm;
  auto m = std::regex_match(s, sm, std::regex("([a-zA-Z])[_]?(\\d+)"));
  if (not m) {
    throw std::runtime_error("\nCould not convert the string " + s +
                             " to an Index object");
  }
  std::string label = sm[1];
  auto space = osi->label_to_space(label[0]);
  size_t p = stoi(sm[2]);
  return Index(space, p);
}

std::vector<Index> make_indices_from_str(const std::string &s) {
  std::vector<Index> res;
  auto s_split = split_indices(s);
  for (const auto &subs : s_split) {
    res.push_back(make_index_from_str(subs));
  }
  return res;
}

index_map_t remap(const std::vector<Index> &idx_vec1,
                  const std::vector<Index> &idx_vec2) {
  index_map_t result;
  std::map<int, std::vector<int>> space_count;
  for (const Index &idx : idx_vec1) {
    int space = idx.space();
    int index = idx.pos();
    space_count[space].push_back(index);
  }

  for (auto &kv : space_count) {
    std::sort(kv.second.begin(), kv.second.end());
  }

  for (const Index &idx : idx_vec2) {
    int space = idx.space();
    int index = idx.pos();
    if (std::find(space_count[space].begin(), space_count[space].end(),
                  index) != space_count[space].end()) {
      // if the index is already in A create a new index
      int last_index = space_count[space].back();
      result[idx] = Index(space, last_index + 1);
      space_count[space].push_back(last_index + 1);
    }
  }
  return result;
}

std::vector<int> num_indices_per_space(const std::vector<Index> &indices) {
  std::vector<int> counter(osi->num_spaces());
  for (const auto &index : indices) {
    counter[index.space()] += 1;
  }
  return counter;
}

int symmetry_factor(const std::vector<Index> &indices) {
  int result = 1;
  std::vector<int> idx_per_space = num_indices_per_space(indices);
  for (int s = 0; s < osi->num_spaces(); ++s) {
    result *= factorial(idx_per_space[s]);
  }
  return result;
}
