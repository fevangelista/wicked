// #include <algorithm>

// #include "helpers/combinatorics.h"
// #include "helpers/helpers.h"
// #include "helpers/orbital_space.h"
// #include "helpers/stl_utils.hpp"

// #include "symbolic_term.h"

// using namespace std;

// /// @brief Return a normal ordered version of this term
// Expression SymbolicTerm::normal_ordered() const {
//   Expression no_expr;
//   // loop over all terms in this expression and normal order them
//   for (const auto &[symterm, c] : terms_) {
//     auto no_term = symterm.normal_ordered();
//     no_expr.add(no_term, c);
//   }
//   return no_expr;
// }