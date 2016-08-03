#include <iostream>

#include "orbital_space.h"

std::shared_ptr<OrbitalSpaceInfo> osi;

std::shared_ptr<OrbitalSpaceInfo> get_osi() { return osi; }

OrbitalSpaceInfo::OrbitalSpaceInfo() {}

void OrbitalSpaceInfo::add_space(const std::string &label,
                                 OrbitalSpaceType type,
                                 const std::vector<std::string> &indices) {
  size_t pos = space_info_.size();
  type_to_pos_[type] = pos;
  label_to_pos_[label] = pos;
  for (auto &index : indices) {
    if (indices_to_pos_.count(index)) {
      std::cout << "Index: " << index << " is already assigned!" << std::endl;
      exit(1);
    } else {
      indices_to_pos_[index] = pos;
    }
  }
  space_info_.push_back(make_tuple(type, label, indices));
}

std::string &OrbitalSpaceInfo::label(int pos) {
  return std::get<1>(space_info_[pos]);
}

std::string &OrbitalSpaceInfo::label(OrbitalSpaceType type) {
  return label(type_to_pos_[type]);
}

std::vector<std::string> &OrbitalSpaceInfo::indices(int pos) {
  return std::get<2>(space_info_[pos]);
}

std::vector<std::string> &OrbitalSpaceInfo::indices(OrbitalSpaceType type) {
  return indices(type_to_pos_[type]);
}

void OrbitalSpaceInfo::reset() {
  space_info_.clear();
  type_to_pos_.clear();
  label_to_pos_.clear();
  indices_to_pos_.clear();
}

void OrbitalSpaceInfo::default_spaces() {
  std::cout << "Setting up default spaces" << std::endl;
  add_space("c", OrbitalSpaceType::Core, {"m", "n", "o", "p"});
  add_space("a", OrbitalSpaceType::Active, {"u", "v", "w", "x", "y", "z"});
  add_space("v", OrbitalSpaceType::Virtual, {"e", "f", "g", "h"});
}
