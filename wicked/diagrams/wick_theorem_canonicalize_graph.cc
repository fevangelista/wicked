#include <algorithm>
#include <iomanip>
#include <iostream>

#include "fmt/format.h"

#include "helpers/combinatorics.h"
#include "helpers/helpers.h"
#include "helpers/orbital_space.h"
#include "helpers/stl_utils.hpp"

#include "contraction.h"
#include "operator.h"
#include "operator_expression.h"

#include "../algebra/expression.h"
#include "../algebra/sqoperator.h"
#include "../algebra/tensor.h"
#include "../algebra/term.h"

#include "wick_theorem.h"

#define PRINT(detail, code)                                                    \
  if (print_ >= detail) {                                                      \
    code                                                                       \
  }

void print_key(std::tuple<int, int, bool, int> key, int n);
void print_contraction(const OperatorProduct &ops,
                       const std::vector<Tensor> &tensors,
                       const std::vector<std::vector<bool>> &bit_map_vec,
                       const std::vector<SQOperator> &sqops,
                       const std::vector<int> sign_order);

void print_contraction_graph(const OperatorProduct &ops,
                             const CompositeContraction &contractions,
                             const std::vector<int> ops_perm,
                             const std::vector<int> contr_perm);

std::string contraction_signature(const OperatorProduct &ops,
                                  const CompositeContraction &contractions,
                                  const std::vector<int> &ops_perm,
                                  const std::vector<int> &contr_perm);

using namespace std;

// elements_canonicalization_less(OperatorProduct &ops,
//                              CompositeContraction &contractions) {}

bool is_ops_permutation_valid(
    std::vector<int> ops_perm,
    const std::vector<std::vector<bool>> &permutable) {
  // ok, so we are given a permutation of some operators and a matrix that tell
  // us which operators/contractions commute. Let's find out if this permutation
  // is inconsistent with the permutations

  // for each operator, let's ask if we can move it to its final position
  // without breaking commutativity
  int n = ops_perm.size();
  for (int i = 0; i < n - 1; i++) {
    // Last i elements are already in place
    for (int j = 0; j < n - i - 1; j++) {
      if (ops_perm[j + 1] < ops_perm[j]) {
        // permutable
        if (permutable[ops_perm[j + 1]][ops_perm[j]]) {
          swap(ops_perm[j], ops_perm[j + 1]);
        } else {
          return false;
        }
      }
    }
  }
  return true;
}

bool do_contractions_commute(int i, int j, const OperatorProduct &ops,
                             const CompositeContraction &contractions) {
  // contractions commute if rearranging two operators does not change the final
  // result
  bool do_commute = true;
  // loop over all elementary contractions
  for (const auto &el_contr : contractions) {
    // loop over all orbital spaces
    for (int s = 0; s < osi->num_spaces(); ++s) {
      // check if this is a single contraction
      if (el_contr.num_ops() == 2) {
        if (el_contr[i].cre(s) * el_contr[j].ann(s) > 0) {
          do_commute = false;
        }
        if (el_contr[i].ann(s) * el_contr[j].cre(s) > 0) {
          do_commute = false;
        }
      }
    }
  }
  return do_commute;
}

bool graph_less(const std::pair<int, int> &l, const std::pair<int, int> &r,
                const std::vector<std::vector<int>> &ops_perms,
                std::vector<std::vector<int>> &con_perms,
                const OperatorProduct &ops,
                const CompositeContraction &contractions) {
  // here we are given the indices of the permutations of the operators and we
  // have to determine if permutation l is less than r

  const auto &l_ops_perm = ops_perms[l.first];
  const auto &r_ops_perm = ops_perms[r.first];
  const auto &l_con_perm = con_perms[l.second];
  const auto &r_con_perm = con_perms[r.second];

  // first compare the operators
  // consider three cases:
  // 1.  lops <  rops (return true)
  // 2.  lops == rops (must check the contractions)
  // 3.  lops >  rops (return false)
  const int no = l_ops_perm.size();
  for (int i = 0; i < no; i++) {
    if (ops[l_ops_perm[i]] < ops[r_ops_perm[i]]) {
      return true;
    }
    if (ops[r_ops_perm[i]] < ops[l_ops_perm[i]]) {
      return false;
    }
  }

  // if we made it here lops == rops
  const int nc = l_con_perm.size();
  // loop over all the contractions permuted
  for (int j = 0; j < nc; j++) {
    const auto &l_con = contractions[l_con_perm[j]];
    const auto &r_con = contractions[r_con_perm[j]];
    for (int i = 0; i < no; i++) {
      if (l_con[l_ops_perm[i]] < r_con[r_ops_perm[i]]) {
        return false;
      }
      if (r_con[r_ops_perm[i]] < l_con[l_ops_perm[i]]) {
        return true;
      }
    }
  }
  return false;
}

std::tuple<OperatorProduct, CompositeContraction, scalar_t>
WickTheorem::canonicalize_contraction_graph(
    const OperatorProduct &ops, const CompositeContraction &contractions) {
  for (const auto &op : ops) {
    if (op.num_ops() % 2 != 0) {
      auto msg =
          "\n\n  WickTheorem::canonicalize_contraction cannot yet handle "
          "operators with an even number of sqops.\n";
      throw std::runtime_error(msg);
    }
  }

  scalar_t sign(1);

  const int nops = ops.size();

  // create a matrix that tells us if two operators commute
  std::vector<std::vector<bool>> commutable(nops,
                                            std::vector<bool>(nops, false));
  for (int i = 0; i < nops; i++) {
    for (int j = 0; j < nops; j++) {
      // check if a commutation changes the contraction
      if (do_contractions_commute(i, j, ops, contractions)) {
        commutable[i][j] = true;
      }
    }
  }

  PRINT(
      PrintLevel::Detailed, cout << "\n  Commutable operator matrix:" << endl;
      for (const auto row
           : commutable) {
        PRINT_ELEMENTS(row);
        cout << endl;
      });

  std::vector<std::vector<int>> ops_perms;
  std::vector<std::vector<int>> con_perms;
  {
    // Loop over all permutations of operators
    std::vector<int> ops_perm(ops.size());
    std::iota(ops_perm.begin(), ops_perm.end(), 0);
    do {
      if (is_ops_permutation_valid(ops_perm, commutable)) {
        ops_perms.push_back(ops_perm);
        PRINT(PrintLevel::Detailed, cout << "  Operator permutation: ";
              PRINT_ELEMENTS(ops_perm); cout << " +" << endl;);
      } else {
        PRINT(PrintLevel::Detailed, cout << "  Operator permutation: ";
              PRINT_ELEMENTS(ops_perm); cout << endl;);
      }
    } while (std::next_permutation(ops_perm.begin(), ops_perm.end()));

    std::vector<int> con_perm(contractions.size());
    std::iota(con_perm.begin(), con_perm.end(), 0);
    do {
      PRINT(PrintLevel::Detailed, cout << "    Contraction permutation: ";
            PRINT_ELEMENTS(con_perm); cout << endl;);
      con_perms.push_back(con_perm);
    } while (std::next_permutation(con_perm.begin(), con_perm.end()));
  }

  // store all the possible graphs
  std::vector<std::pair<int, int>> graphs;
  for (const auto &[o, ops_perm] : enumerate(ops_perms)) {
    for (const auto &[c, con_perm] : enumerate(con_perms)) {
      graphs.push_back(std::pair(o, c));
    }
  }

  // sort all the graphs
  std::sort(graphs.begin(), graphs.end(),
            [&](const std::pair<int, int> &l, const std::pair<int, int> &r) {
              return graph_less(l, r, ops_perms, con_perms, ops, contractions);
            });

  // setup vectors that will store the best permutation of
  // operators and contractions
  std::vector<int> best_ops_perm(ops.size());
  std::vector<int> best_contr_perm(contractions.size());
  std::iota(best_ops_perm.begin(), best_ops_perm.end(), 0);
  std::iota(best_contr_perm.begin(), best_contr_perm.end(), 0);

  PRINT(PrintLevel::Detailed, cout << "Contraction to canonicalize:" << endl;
        print_contraction_graph(ops, contractions, best_ops_perm,
                                best_contr_perm););

  const auto [best_o_perm, best_c_perm] = graphs[0];

  OperatorProduct best_ops;
  for (int o : ops_perms[best_o_perm]) {
    best_ops.push_back(ops[o]);
  }

  // permute the order and operator upon a contraction acts
  CompositeContraction best_contractions;
  for (int c : con_perms[best_c_perm]) {
    std::vector<GraphMatrix> permuted_contr;
    for (int o : ops_perms[best_o_perm]) {
      permuted_contr.push_back(contractions[c][o]);
    }
    // for (int j = 0; j < nops; j++) {
    //   permuted_contr.push_back(contractions[c][best_ops_perm[j]]);
    // }
    best_contractions.push_back(permuted_contr);
  }

  PRINT(PrintLevel::Detailed, cout << "Contraction to canonicalize:" << endl;
        print_contraction_graph(ops, contractions, ops_perms[best_o_perm],
                                con_perms[best_c_perm]););

  return std::make_tuple(best_ops, best_contractions, sign);
}

// bool op_less(const size_t &l, const size_t &r,
//              const std::vector<std::vector<int>> &ops_perms,
//              const OperatorProduct &ops) {
//   // here we are given the indices of the permutations of the operators and
//   we
//   // have to determine if permutation l is less than r

//   const auto &l_ops_perm = ops_perms[l];
//   const auto &r_ops_perm = ops_perms[r];

//   // first compare the operators
//   // consider three cases:
//   // 1.  lops <  rops (return true)
//   // 2.  lops == rops (must check the contractions)
//   // 3.  lops >  rops (return false)
//   const int no = l_ops_perm.size();
//   for (int i = 0; i < no; i++) {
//     if (ops[l_ops_perm[i]] < ops[r_ops_perm[i]]) {
//       return true;
//     }
//     if (ops[r_ops_perm[i]] < ops[l_ops_perm[i]]) {
//       return false;
//     }
//   }
//   return false;
// }

// bool op_eq(const size_t &l, const size_t &r,
//            const std::vector<std::vector<int>> &ops_perms,
//            const OperatorProduct &ops) {
//   // here we are given the indices of the permutations of the operators and
//   we
//   // have to determine if permutation l is equal to r

//   const auto &l_ops_perm = ops_perms[l];
//   const auto &r_ops_perm = ops_perms[r];

//   const int no = l_ops_perm.size();
//   for (int i = 0; i < no; i++) {
//     if (ops[l_ops_perm[i]] != ops[r_ops_perm[i]]) {
//       return false;
//     }
//   }
//   return true;
// }

// std::tuple<OperatorProduct, CompositeContraction, scalar_t>
// WickTheorem::canonicalize_contraction_graph_experimental(
//     const OperatorProduct &ops, const CompositeContraction &contractions) {
//   for (const auto &op : ops) {
//     if (op.num_ops() % 2 != 0) {
//       auto msg =
//           "\n\n  WickTheorem::canonicalize_contraction cannot yet handle "
//           "operators with an even number of sqops.\n";
//       throw std::runtime_error(msg);
//     }
//   }

//   scalar_t sign(1);

//   const int nops = ops.size();

//   // create a matrix that tells us if two operators commute
//   std::vector<std::vector<bool>> commutable(nops,
//                                             std::vector<bool>(nops, false));
//   for (int i = 0; i < nops; i++) {
//     for (int j = 0; j < nops; j++) {
//       // check if a commutation changes the contraction
//       if (do_contractions_commute(i, j, ops, contractions)) {
//         commutable[i][j] = true;
//       }
//     }
//   }

//   PRINT(
//       PrintLevel::Detailed, cout << "\n  Commutable operator matrix:" <<
//       endl; for (const auto row
//            : commutable) {
//         PRINT_ELEMENTS(row);
//         cout << endl;
//       });

//   std::vector<std::vector<int>> ops_perms;
//   std::vector<std::vector<int>> con_perms;
//   {
//     // Loop over all permutations of operators
//     std::vector<int> ops_perm(ops.size());
//     std::iota(ops_perm.begin(), ops_perm.end(), 0);
//     do {
//       if (is_ops_permutation_valid(ops_perm, commutable)) {
//         ops_perms.push_back(ops_perm);
//         PRINT(PrintLevel::Detailed, cout << "  Operator permutation: ";
//               PRINT_ELEMENTS(ops_perm); cout << " +" << endl;);
//       } else {
//         PRINT(PrintLevel::Detailed, cout << "  Operator permutation: ";
//               PRINT_ELEMENTS(ops_perm); cout << endl;);
//       }
//     } while (std::next_permutation(ops_perm.begin(), ops_perm.end()));

//     std::vector<int> con_perm(contractions.size());
//     std::iota(con_perm.begin(), con_perm.end(), 0);
//     do {
//       PRINT(PrintLevel::Detailed, cout << "    Contraction permutation: ";
//             PRINT_ELEMENTS(con_perm); cout << endl;);
//       con_perms.push_back(con_perm);
//     } while (std::next_permutation(con_perm.begin(), con_perm.end()));
//   }

//   std::vector<size_t> idx(ops_perms.size());
//   std::iota(idx.begin(), idx.end(), 0);
//   // sort all the operator permutations
//   std::sort(idx.begin(), idx.end(), [&](const size_t &l, const size_t &r) {
//     return op_less(l, r, ops_perms, ops);
//   });

//   // check for degenerate best opeartor permutations
//   std::vector<std::vector<int>> best_ops_perms;
//   for (const size_t i : idx) {
//     if (op_eq(i, idx[0], ops_perms, ops)) {
//       best_ops_perms.push_back(ops_perms[i]);
//     } else {
//       break;
//     }
//   }

//   // store all the possible graphs
//   std::vector<std::pair<int, int>> graphs;
//   for (const auto &[o, ops_perm] : enumerate(best_ops_perms)) {
//     for (const auto &[c, con_perm] : enumerate(con_perms)) {
//       graphs.push_back(std::pair(o, c));
//     }
//   }

//   // sort all the graphs
//   std::sort(graphs.begin(), graphs.end(),
//             [&](const std::pair<int, int> &l, const std::pair<int, int> &r) {
//               return graph_less(l, r, best_ops_perms, con_perms, ops,
//                                 contractions);
//             });

//   // setup vectors that will store the best permutation of
//   // operators and contractions
//   std::vector<int> best_ops_perm(ops.size());
//   std::vector<int> best_contr_perm(contractions.size());
//   std::iota(best_ops_perm.begin(), best_ops_perm.end(), 0);
//   std::iota(best_contr_perm.begin(), best_contr_perm.end(), 0);

//   PRINT(PrintLevel::Detailed, cout << "Contraction to canonicalize:" << endl;
//         print_contraction_graph(ops, contractions, best_ops_perm,
//                                 best_contr_perm););

//   const auto [best_o_perm, best_c_perm] = graphs[0];

//   OperatorProduct best_ops;
//   for (int o : best_ops_perms[best_o_perm]) {
//     best_ops.push_back(ops[o]);
//   }

//   // permute the order and operator upon a contraction acts
//   CompositeContraction best_contractions;
//   for (int c : con_perms[best_c_perm]) {
//     std::vector<GraphMatrix> permuted_contr;
//     for (int o : best_ops_perms[best_o_perm]) {
//       permuted_contr.push_back(contractions[c][o]);
//     }
//     // for (int j = 0; j < nops; j++) {
//     //   permuted_contr.push_back(contractions[c][best_ops_perm[j]]);
//     // }
//     best_contractions.push_back(permuted_contr);
//   }

//   PRINT(PrintLevel::Detailed, cout << "Contraction to canonicalize:" << endl;
//         print_contraction_graph(ops, contractions, ops_perms[best_o_perm],
//                                 con_perms[best_c_perm]););

//   return std::make_tuple(best_ops, best_contractions, sign);
// }
