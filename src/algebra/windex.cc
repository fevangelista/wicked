#include "windex.h"

WIndex::WIndex() : index_(std::make_pair(-1, -1)) {}

WIndex::WIndex(int space, int p)
    : index_(std::make_pair(space, p)) {}

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

//std::string WIndex::ambit() const {
//  std::string s;
//  s += osi->label(space_);
//  if (spin_ == Beta) {
//    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
//  }
//  s += '_';
//  s += boost::lexical_cast<std::string>(p);
//  return (s);
//}

std::vector<WIndex>
make_indices_from_space_labels(const std::vector<std::string>& labels) {
  std::vector<WIndex> indices;
  std::vector<int> space_count(osi->num_spaces());
  for (const auto& label : labels) {
    int space = osi->label_to_space(label);
    space_count[space] += 1;
    int p = space_count[space];
    indices.push_back(WIndex(space, p));
  }
  return indices;
}

std::vector<int> num_indices_per_space(const std::vector<WIndex> &indices)
{
    std::vector<int> counter(osi->num_spaces());
    for (const auto& index : indices){
        counter[index.space()] += 1;
    }
    return counter;
}
