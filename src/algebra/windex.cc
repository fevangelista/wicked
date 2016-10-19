#include <algorithm>

#include "windex.h"

WIndex::WIndex() : index_(std::make_pair(-1, -1)) {}

WIndex::WIndex(int space, int p) : index_(std::make_pair(space, p)) {}

bool WIndex::operator==(WIndex const &other) const {
  return index_ == other.index_;
}

bool WIndex::operator<(WIndex const &other) const {
  return other.index_ < index_;
}

std::string WIndex::str() const {
  return osi->label(space()) + std::to_string(index());
}

std::string WIndex::latex() const {
  return osi->label(space()) + "_{" + std::to_string(index()) + "}";
}

std::ostream &operator<<(std::ostream &os, const WIndex &idx) {
  os << idx.str();
  return os;
}

// std::string WIndex::ambit() const {
//  std::string s;
//  s += osi->label(space_);
//  if (spin_ == Beta) {
//    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
//  }
//  s += '_';
//  s += boost::lexical_cast<std::string>(p);
//  return (s);
//}

std::vector<std::vector<WIndex>> make_indices_from_space_labels(
    const std::vector<std::vector<std::string>> &labels_space) {
  std::vector<std::vector<WIndex>> indices;
  std::vector<int> space_count(osi->num_spaces());
  for (const auto &labels : labels_space) {
    std::vector<WIndex> indices_space;
    for (const auto &label : labels) {
      int space = osi->label_to_space(label);
      space_count[space] += 1;
      int p = space_count[space];
      indices_space.push_back(WIndex(space, p));
    }
    indices.push_back(indices_space);
  }
  return indices;
}

index_map_t remap(const std::vector<WIndex> &idx_vec1,
                  const std::vector<WIndex> &idx_vec2) {
  index_map_t result;
  std::map<int, std::vector<int>> space_count;
  for (const WIndex &idx : idx_vec1) {
    int space = idx.space();
    int index = idx.index();
    space_count[space].push_back(index);
  }

  for (auto &kv : space_count) {
    std::sort(kv.second.begin(), kv.second.end());
  }

  for (const WIndex &idx : idx_vec2) {
    int space = idx.space();
    int index = idx.index();
    if (std::find(space_count[space].begin(), space_count[space].end(),
                  index) != space_count[space].end()) {
      // if the index is already in A create a new index
      int last_index = space_count[space].back();
      result[idx] = WIndex(space, last_index + 1);
      space_count[space].push_back(last_index + 1);
    }
  }
  return result;
}

std::vector<int> num_indices_per_space(const std::vector<WIndex> &indices) {
  std::vector<int> counter(osi->num_spaces());
  for (const auto &index : indices) {
    counter[index.space()] += 1;
  }
  return counter;
}
