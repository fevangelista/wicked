#include "orbital_space.h"

#include "woperator.h"

using namespace std;

WOperator::WOperator(const std::string& label,
                     const std::vector<int>& ann,
                     const std::vector<int>& cre)
    : label_(label), ann_(ann) , cre_(cre)
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
    for (size_t i = 0; i < osi->num_spaces(); ++i){
        s += " " + osi->label(i) + "=" + to_string(num_indices(i,true));
    }
    s += "|";
    for (size_t i = 0; i < osi->num_spaces(); ++i){
        s += " " + osi->label(i) + "=" + to_string(num_indices(i,false));
    }
    s += "]";
    return s;
}

std::vector<WOperator> make_operator(const std::string& label,
                                     const std::vector<std::string>& cre_indices,
                                     const std::vector<std::string>& ann_indices)
{
    std::vector<WOperator> vec;

    // Count the number of indices in each space
    std::map<std::string,int> cre_count,ann_count;

//    for (std::string& s : cre_indices){
//        cre_count[s] += 1;
//    }
//    for (std::string& s : ann_indices){
//        ann_count[s] += 1;
//    }



//    enumeration_elements(const std::vector<std::vector<T>>& sets);


    return vec;
}
