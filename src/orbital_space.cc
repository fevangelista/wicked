#include <iostream>

#include "orbital_space.h"

std::shared_ptr<OrbitalSpaceInfo> osi;

std::shared_ptr<OrbitalSpaceInfo> get_osi() {return osi;}

OrbitalSpaceInfo::OrbitalSpaceInfo()
{
}

OrbitalSpaceInfo::~OrbitalSpaceInfo()
{
}

void OrbitalSpaceInfo::add_elementary_space(const std::string& label,
                                            OrbitalSpaceType type,
                                            const std::vector<std::string>& indices)
{
    elementary_spaces_.push_back(type);
    composite_spaces_[type] = {type};
    for (auto& index : indices){
        if (index_to_space_.count(index)){
            std::cout << "Index: " << index << " is already assigned!" << std::endl;
            exit(1);
        }else{
            index_to_space_[index] = type;
        }
    }
    orbital_space_label_[type] = label;
    orbital_space_indices_[type] = indices;
}

void OrbitalSpaceInfo::add_composite_space(const std::string& label,
                                           OrbitalSpaceType type,
                                           const std::vector<std::string>& indices,
                                           std::vector<OrbitalSpaceType> subspaces)
{
    composite_spaces_[type] = subspaces;
    for (auto& index : indices){
        if (index_to_space_.count(index)){
            std::cout << "Index: " << index << " is already assigned!" << std::endl;
            exit(1);
        }else{
            index_to_space_[index] = type;
        }
    }
    orbital_space_label_[type] = label;
    orbital_space_indices_[type] = indices;
}

bool OrbitalSpaceInfo::is_space_elementary(OrbitalSpaceType type)
{
    return composite_spaces_[type].size() == 1;
}

size_t OrbitalSpaceInfo::space_size(OrbitalSpaceType type)
{
    return composite_spaces_[type].size();
}

std::string& OrbitalSpaceInfo::label(OrbitalSpaceType type)
{
    return orbital_space_label_[type];
}

OrbitalSpaceType OrbitalSpaceInfo::index_to_space(const std::string &idx)
{
    return index_to_space_[idx];
}

std::string& OrbitalSpaceInfo::index(OrbitalSpaceType type, int n)
{
    return orbital_space_indices_[type][n];
}

void OrbitalSpaceInfo::reset()
{
    elementary_spaces_.clear();
    composite_spaces_.clear();
    index_to_space_.clear();
    orbital_space_indices_.clear();
}

void OrbitalSpaceInfo::default_spaces()
{
    std::cout << "Setting up default spaces" << std::endl;
    add_elementary_space("c",Core,{"m","n","o"});
    add_elementary_space("a",Active,{"u","v","w","x","y","z"});
    add_elementary_space("v",Virtual,{"e","f","g","h"});
    add_composite_space("h",Hole,{"i","j","k","l"},{Core,Active});
    add_composite_space("p",Particle,{"a","b","c","d"},{Active,Virtual});
    add_composite_space("g",All,{"p","q","r","s","t"},{Core,Active,Virtual});
}
