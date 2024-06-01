#include <iostream>

#include "helpers.h"
#include "orbital_space.h"

std::shared_ptr<OrbitalSpaceInfo> orbital_subspaces =
    std::make_shared<OrbitalSpaceInfo>();

std::shared_ptr<OrbitalSpaceInfo> get_osi() { return orbital_subspaces; }

std::map<FieldType, std::string> FieldType_to_str{
    {FieldType::Fermion, "fermion"}, {FieldType::Boson, "boson"}};

std::map<FieldType, std::string> FieldType_to_op_symbol{
    {FieldType::Fermion, "a"}, {FieldType::Boson, "b"}};

std::map<SpaceType, std::string> SpaceType_to_str{
    {SpaceType::Occupied, "occupied"},
    {SpaceType::Unoccupied, "unoccupied"},
    {SpaceType::General, "general"},
};

OrbitalSpace::OrbitalSpace(char label, FieldType field_type,
                           SpaceType space_type,
                           const std::vector<std::string> &indices,
                           const std::vector<int> &elementary_spaces)
    : label_(label), space_type_(space_type), field_type_(field_type),
      indices_(indices), elementary_spaces_(elementary_spaces) {}

char OrbitalSpace::label() const { return label_; }

FieldType OrbitalSpace::field_type() const { return field_type_; }

SpaceType OrbitalSpace::space_type() const { return space_type_; }

const std::vector<std::string> &OrbitalSpace::indices() const {
  return indices_;
}

const std::vector<int> &OrbitalSpace::elementary_spaces() const {
  return elementary_spaces_;
}

SpaceType string_to_space_type(const std::string &str) {
  for (const auto &[space_type, s] : SpaceType_to_str) {
    if (str == s) {
      return space_type;
    }
  }
  std::vector<std::string> options;
  for (const auto &[space_type, s] : SpaceType_to_str) {
    options.push_back(s);
  }
  throw std::runtime_error(
      "\nstring_to_space_type() - called with an invalid string (" + str + ")" +
      "\nValid options are: [" + join(options) + "]");
  return SpaceType::General;
}

FieldType string_to_field_type(const std::string &str) {
  for (const auto &[field_type, s] : FieldType_to_str) {
    if (str == s) {
      return field_type;
    }
  }
  throw std::runtime_error(
      "\nstring_to_field_type() - called with an invalid string (" + str + ")" +
      "\nValid options are: [fermion]");
  return FieldType::Fermion;
}

OrbitalSpaceInfo::OrbitalSpaceInfo() {}

void OrbitalSpaceInfo::add_space(char label, FieldType field_type,
                                 SpaceType space_type,
                                 const std::vector<std::string> &indices,
                                 const std::vector<char> &elementary_spaces) {
  if (label_to_pos_.count(label) != 0) {
    throw std::runtime_error("add_space: Orbitals space label \"" +
                             std::string(1, label) +
                             "\" is already defined. Use another label.");
  }

  size_t pos = space_info_.size();
  label_to_pos_[label] = pos;
  for (auto &index : indices) {
    if (indices_to_pos_.count(index)) {
      throw std::runtime_error(
          "add_space: Index \"" + index +
          "\" is already assigned to another space. Use another index.");
    } else {
      indices_to_pos_[index] = pos;
    }
  }
  std::vector<int> elementary_spaces_int;
  for (char e : elementary_spaces) {
    elementary_spaces_int.push_back(label_to_space(e));
  }
  space_info_.push_back(OrbitalSpace(label, field_type, space_type, indices,
                                     elementary_spaces_int));
}

std::string OrbitalSpaceInfo::str() const {
  std::vector<std::string> s;
  for (const auto &info : space_info_) {
    s.push_back("space label: " + std::string(1, info.label()) +
                "\nfield type: " + FieldType_to_str[info.field_type()] +
                "\nspace type: " + SpaceType_to_str[info.space_type()] +
                "\nindices: [" + join(info.indices(), ",") + "]");
  }
  return join(s, "\n\n");
}

std::map<std::string, std::vector<std::string>> OrbitalSpaceInfo::to_dict() const {
  std::map<std::string, std::vector<std::string>> d;
  for (const auto &info : space_info_) {
    d[std::string(1, info.label())] = info.indices();
  }
  return d;
}

char OrbitalSpaceInfo::label(int pos) const { return space_info_[pos].label(); }

const std::string OrbitalSpaceInfo::index_label(int pos, int idx) const {
  const std::vector<std::string> &index_labels = indices(pos);
  if (idx < index_labels.size()) {
    return indices(pos)[idx];
  }
  return std::string(1, label(pos)) + "_{" + std::to_string(idx) + "}";
}

SpaceType OrbitalSpaceInfo::space_type(int pos) const {
  return space_info_[pos].space_type();
}

FieldType OrbitalSpaceInfo::field_type(int pos) const {
  return space_info_[pos].field_type();
}

const std::string &OrbitalSpaceInfo::op_symbol(int pos) const {
  return FieldType_to_op_symbol[field_type(pos)];
}

const std::vector<std::string> &OrbitalSpaceInfo::indices(int pos) const {
  return space_info_[pos].indices();
}

int OrbitalSpaceInfo::label_to_space(char label) const {
  auto search = label_to_pos_.find(label);
  if (search == label_to_pos_.end()) {
    throw std::runtime_error("\n  Could not find label :'" +
                             std::string(1, label) +
                             "' in OrbitalSpaceInfo.\n  Use the add_space() "
                             "function to define orbital spaces.");
  }
  return search->second;
}

void OrbitalSpaceInfo::reset() {
  space_info_.clear();
  label_to_pos_.clear();
  indices_to_pos_.clear();
}

// void OrbitalSpaceInfo::default_spaces() {
//   // add_space('o', FieldType::Fermion, SpaceType::Occupied,
//   //           {"i", "j", "k", "l", "m", "n"});
//   // add_space('v', FieldType::Fermion, SpaceType::Unoccupied,
//   //           {"a", "b", "c", "d", "e", "f"});
//   // add_space('c', FieldType::Fermion, SpaceType::Occupied, {"m", "n", "o",
//   // "p"}); add_space('a', FieldType::Fermion, SpaceType::General,
//   //           {"u", "v", "w", "x", "y", "z"});
//   // add_space('v', FieldType::Fermion, SpaceType::Unoccupied,
//   //           {"e", "f", "g", "h"});
// }
