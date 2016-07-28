#ifndef _wicked_orbital_space_h_
#define _wicked_orbital_space_h_

#include <map>
#include <string>
#include <vector>

/// Type of orbital space
enum class OrbitalSpaceType {Core,Active,Virtual,Hole,Particle,All,RI,None};

/// Spin types
enum class SpinType {SpinOrbital,SpinFree,Alpha,Beta};

class OrbitalSpaceInfo {

using t_space_info = std::vector<std::tuple<OrbitalSpaceType,std::string,std::vector<std::string>>>;
public:
    OrbitalSpaceInfo();

    ~OrbitalSpaceInfo();

    /// Set default spaces
    void default_spaces();

    /// Set default spaces
    void reset();

    /// Add an elementary space
    void add_space(const std::string& label, OrbitalSpaceType type,
                              const std::vector<std::string>& indices);

    /// Return the number of elementary spaces
    size_t num_spaces() {return space_info_.size();}

    /// The label of an orbital space
    std::string& label(int pos);
    std::string& label(OrbitalSpaceType type);

    /// The indices of an orbital space
    std::vector<std::string>& indices(int pos);
    std::vector<std::string>& indices(OrbitalSpaceType type);

private:
    /// Vector of spaces
    t_space_info space_info_;

    /// Maps a space to its index
    std::map<OrbitalSpaceType,int> type_to_pos_;

    /// Maps a space label to its index
    std::map<std::string,int> label_to_pos_;

    /// Maps orbital indices to a composite space
    std::map<std::string,int> indices_to_pos_;
};

extern std::shared_ptr<OrbitalSpaceInfo> osi;

std::shared_ptr<OrbitalSpaceInfo> get_osi();

#endif // _wicked_orbital_space_h_
