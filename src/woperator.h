#ifndef _wicked_operator_h_
#define _wicked_operator_h_

#include <vector>
#include <string>

/// A class to represent operators
class WOperator{

public:
    /// Constructor. Set number of
    WOperator(const std::string& label, const std::vector<int>& cre, const std::vector<int>& ann);

    /// Return the label of the operator
    std::string& label();

    /// Return the number of indices for a given space and type (cre/ann)
    int num_indices(int space,bool creation);

    /// Return a string representation of the operator
    std::string str();

private:
    /// The label of the operator
    std::string label_;

    /// The number of creation operator for each space
    std::vector<int> cre_;

    /// The number of annihilation operator for each space
    std::vector<int> ann_;
};

/// A helper function to construct all the components of an operator
/// Example: make_operator("T1","ia") will construct the following components
///          T1[0,1,0|1,0,0] -> T1(active|core) = T1[u][m]
///          T1[0,1,0|1,0,0] -> T1(active|core) = T1[u][m]
std::vector<WOperator> make_operator(const std::string& label,const std::string& indices);

#endif // _wicked_operator_h_
