#include "windex.h"

WIndex::WIndex() : index_(std::make_pair(OrbitalSpaceType::None, -1)) {}

WIndex::WIndex(OrbitalSpaceType space, int p)
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
make_indices_from_space_type(const std::vector<OrbitalSpaceType> &space) {
  std::vector<WIndex> indices;
  std::map<OrbitalSpaceType, int> space_count;
  for (const OrbitalSpaceType space : space) {
    space_count[space] += 1;
    int p = space_count[space];
    indices.push_back(WIndex(space, p));
  }
  return indices;
}
