#include <iostream>

#include "orbital_space.h"

std::shared_ptr<OrbitalSpaceInfo> osi;

std::shared_ptr<OrbitalSpaceInfo> get_osi() { return osi; }

OrbitalSpaceInfo::OrbitalSpaceInfo() {}

void OrbitalSpaceInfo::add_space(const std::string &label,
                                 DMStructure structure,
                                 const std::vector<std::string> &indices) {
  size_t pos = space_info_.size();
  label_to_pos_[label] = pos;
  for (auto &index : indices) {
    if (indices_to_pos_.count(index)) {
      std::cout << "Index: " << index << " is already assigned!" << std::endl;
      exit(1);
    } else {
      indices_to_pos_[index] = pos;
    }
  }
  space_info_.push_back(make_tuple(label, structure, indices));
}

const std::string &OrbitalSpaceInfo::label(int pos) const {
  return std::get<0>(space_info_[pos]);
}

DMStructure OrbitalSpaceInfo::dmstructure(int pos) const {
  return std::get<1>(space_info_[pos]);
}

const std::vector<std::string> &OrbitalSpaceInfo::indices(int pos) const {
  return std::get<2>(space_info_[pos]);
}

int OrbitalSpaceInfo::label_to_space(const std::string &label) const
{
  return label_to_pos_.at(label);
}

void OrbitalSpaceInfo::reset() {
  space_info_.clear();
  label_to_pos_.clear();
  indices_to_pos_.clear();
}

void OrbitalSpaceInfo::default_spaces() {
  add_space("c", DMStructure::DoublyOccupied,
            {"m", "n", "o", "p"});
  add_space("a", DMStructure::General,
            {"u", "v", "w", "x", "y", "z"});
  add_space("v", DMStructure::Unoccupied,
            {"e", "f", "g", "h"});
}
