#include "helpers.h"
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

    std::vector<std::string> cv,av;
    for (int i = 0; i < osi->num_spaces(); ++i){
        cv.push_back(to_string(num_indices(i,true)));
    }
    for (int i = 0; i < osi->num_spaces(); ++i){
        av.push_back(to_string(num_indices(i,false)));
    }

    s += " [" + to_string(cv," ") + "|" + to_string(av," ") + "] (";
    for (int i = 0; i < osi->num_spaces(); ++i){
        for (int j = 0; j < num_indices(i,false); j++) s += osi->label(i);
    }
    s += " -> ";
    for (int i = 0; i < osi->num_spaces(); ++i){
        for (int j = 0; j < num_indices(i,true); j++) s += osi->label(i);
    }
    s += ")";
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
