#include "orbital_space.h"

#include "woperator.h"

using namespace std;

WOperator::WOperator(const std::string& label,
                     const std::vector<int>& cre,
                     const std::vector<int>& ann)
    : label_(label), cre_(cre), ann_(ann)
{
}

std::string& WOperator::label()
{
    return label_;
}

int WOperator::num_indices(int space, bool creation)
{
    return creation ? cre_[space] : ann_[space];
}

std::string WOperator::str()
{
    std::string s = label_;
    s += "[";
    for (size_t i = 0; i < osi->num_elementary_spaces(); ++i){
        s += " " + osi->el_space_label(i) + "=" + to_string(num_indices(i,true));
    }
    s += "|";
    for (size_t i = 0; i < osi->num_elementary_spaces(); ++i){
        s += " " + osi->el_space_label(i) + "=" + to_string(num_indices(i,false));
    }
    s += "]";
    return s;
}

std::vector<WOperator> make_operator(const std::string& label,const std::string& indices)
{
    std::vector<WOperator> vec;

    return vec;
}
