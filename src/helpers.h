#ifndef _wicked_helpers_h_
#define _wicked_helpers_h_

#include <string>
#include <vector>

// Returns a comma separated list of the indices
std::string to_string(const std::vector<std::string> &svec, const std::string &sep = ",");

#endif // _wicked_helpers_h_
