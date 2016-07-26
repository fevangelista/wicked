#ifndef _wicked_orbital_space_h_
#define _wicked_orbital_space_h_

#include <map>
#include <string>
#include <vector>

/// Type of orbital space
enum OrbitalSpaceType {Core,Active,Virtual,Hole,Particle,All,RI,None};

/// Spin types
enum SpinType {SpinOrbital,SpinFree,Alpha,Beta};

class OrbitalSpaceInfo {
public:
    OrbitalSpaceInfo();

    ~OrbitalSpaceInfo();

    /// Set default spaces
    void default_spaces();

    /// Set default spaces
    void reset();

    /// Add an elementary space
    void add_elementary_space(const std::string& label, OrbitalSpaceType type,
                              const std::vector<std::string>& indices);

    /// Add a composite space
    void add_composite_space(const std::string& label, OrbitalSpaceType type,
                             const std::vector<std::string>& indices,
                             std::vector<OrbitalSpaceType> subspaces);   

    /// Return the number of elementary spaces
    size_t num_elementary_spaces() {return elementary_spaces_.size();}

    std::string& el_space_label(int space) {return orbital_space_label_[elementary_spaces_[space]];}

    /// Is this orbital space elementary?
    bool is_space_elementary(OrbitalSpaceType type);

    /// Return the size of a space
    size_t space_size(OrbitalSpaceType type);

    /// Return the size of a space
    std::vector<OrbitalSpaceType>& subspaces(OrbitalSpaceType type) {return composite_spaces_[type];}

    /// The label of an orbital space
    std::string& label(OrbitalSpaceType type);

    /// Maps an index to a space
    OrbitalSpaceType index_to_space(const std::string& idx);

    /// Return the n-th index in a space
    std::string& index(OrbitalSpaceType type,int n);

private:
    /// List the elementary spaces
    std::vector<OrbitalSpaceType> elementary_spaces_;

    /// Maps the orbital spaces to elementary spaces
    std::map<OrbitalSpaceType,std::vector<OrbitalSpaceType>> composite_spaces_;

    /// Maps orbital indices to a composite space
    std::map<std::string,OrbitalSpaceType> index_to_space_;

    /// Maps a composite space to its label
    std::map<OrbitalSpaceType,std::string> orbital_space_label_;

    /// Maps a composite space to its orbital indices
    std::map<OrbitalSpaceType,std::vector<std::string>> orbital_space_indices_;
};

extern std::shared_ptr<OrbitalSpaceInfo> osi;

std::shared_ptr<OrbitalSpaceInfo> get_osi();

#endif // _wicked_orbital_space_h_
