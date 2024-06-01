#ifndef _wicked_orbital_space_h_
#define _wicked_orbital_space_h_

#include <map>
#include <memory>
#include <string>
#include <vector>

/// Type of orbital space
enum class SpaceType {
  // Single creation/annihilation contractions that yields a Kronecker delta
  Occupied,
  // Single annihilation/creation contractions that yields a Kronecker delta
  Unoccupied,
  // Multi-leg contractions
  General,
};

/// The type of field
enum class FieldType { Fermion, Boson };

class OrbitalSpace {
private:
  char label_;
  SpaceType space_type_;
  FieldType field_type_;
  std::vector<std::string> indices_;
  std::vector<int> elementary_spaces_;

public:
  OrbitalSpace(char label, FieldType field_type, SpaceType space_type,
               const std::vector<std::string> &indices,
               const std::vector<int> &elementary_spaces);
  char label() const;
  FieldType field_type() const;
  SpaceType space_type() const;
  const std::vector<std::string> &indices() const;
  const std::vector<int> &elementary_spaces() const;
};

class OrbitalSpaceInfo {
public:
  OrbitalSpaceInfo();

  /// Set default spaces
  void reset();

  /// Add an elementary space
  void add_space(char label, FieldType field_type, SpaceType space_type,
                 const std::vector<std::string> &indices,
                 const std::vector<char> &elementary_spaces = {});

  // /// Add a composite space
  // void add_composite_space(char label,
  //                          const std::vector<char> &elementary_spaces);

  /// Return the number of elementary spaces
  int num_spaces() { return static_cast<int>(space_info_.size()); }

  /// The label of an orbital space
  char label(int pos) const;

  /// The label of an index that belongs to a given orbital space
  const std::string index_label(int pos, int idx) const;

  /// @return the space type, which carries information about
  /// the structure of the density matrices
  SpaceType space_type(int pos) const;

  /// @return the field type
  FieldType field_type(int pos) const;

  /// @return the symbol of an operator corresponding to this space
  const std::string &op_symbol(int pos) const;

  /// The indices of an orbital space
  const std::vector<std::string> &indices(int pos) const;

  /// Maps a label into an orbital space
  int label_to_space(char label) const;

  /// return a string representation
  std::string str() const;

  /// Return a dictionary representation
  std::map<std::string, std::vector<std::string>> to_dict() const;

private:
  /// Vector of spaces
  std::vector<OrbitalSpace> space_info_;

  /// Maps a space label to its index
  std::map<char, int> label_to_pos_;

  /// Maps orbital indices to a composite space
  std::map<std::string, int> indices_to_pos_;
};

extern std::shared_ptr<OrbitalSpaceInfo> orbital_subspaces;

std::shared_ptr<OrbitalSpaceInfo> get_osi();

/// Used to convert a string (e.g., "unoccupied") to a SpaceType
SpaceType string_to_space_type(const std::string &str);

/// Used to convert a string (e.g., "fermion") to a FieldType
FieldType string_to_field_type(const std::string &str);

#endif // _wicked_orbital_space_h_
