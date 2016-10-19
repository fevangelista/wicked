#ifndef _wicked_orbital_space_h_
#define _wicked_orbital_space_h_

#include <map>
#include <string>
#include <vector>
#include <memory>

/// Type of orbital space
// enum class OrbitalSpaceType { Core, Active, Virtual, RI, None };

/// Type of orbital space
enum class DMStructure {
  // Single creation/annihilation contractions that yields a Kronecker delta
  DoublyOccupied,
  // Single annihilation/creation contractions that yields a Kronecker delta
  Unoccupied,
  General // Multi-leg contractions
};

/// Spin types
enum class SpinType { SpinOrbital, SpinFree, Alpha, Beta };

class OrbitalSpaceInfo {

  /// This type holds infomation about a space
  using t_space_info =
      std::tuple<std::string, DMStructure, std::vector<std::string>>;

public:
  OrbitalSpaceInfo();

  /// Set default spaces
  void default_spaces();

  /// Set default spaces
  void reset();

  /// Add an elementary space
  void add_space(const std::string &label, DMStructure structure,
                 const std::vector<std::string> &indices);

  /// Return the number of elementary spaces
  int num_spaces() { return static_cast<int>(space_info_.size()); }

  /// The label of an orbital space
  const std::string &label(int pos) const;

  /// The structure of the density matrices for an orbital space
  DMStructure dmstructure(int pos) const;

  /// The indices of an orbital space
  const std::vector<std::string> &indices(int pos) const;

  /// Maps a label into an orbital space
  int label_to_space(const std::string &label) const;

private:
  /// Vector of spaces
  std::vector<t_space_info> space_info_;

  /// Maps a space label to its index
  std::map<std::string, int> label_to_pos_;

  /// Maps orbital indices to a composite space
  std::map<std::string, int> indices_to_pos_;
};

extern std::shared_ptr<OrbitalSpaceInfo> osi;

std::shared_ptr<OrbitalSpaceInfo> get_osi();

#endif // _wicked_orbital_space_h_
