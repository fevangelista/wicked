#include <format>

#include "helpers/combinatorics.h"
#include "helpers/helpers.h"
#include "helpers/stl_utils.hpp"

#include "contraction.h"
#include "operator.h"
#include "operator_product.h"

#include "wick_theorem.h"

#define PRINT(detail, code)                                                    \
  if (print_ >= detail) {                                                      \
    code                                                                       \
  }

using namespace std;

std::vector<ElementaryContraction>
WickTheorem::generate_elementary_contractions(const OperatorProduct &ops) {
  PRINT(PrintLevel::Summary,
        std::cout << "\n- Step 1. Generating elementary contractions"
                  << std::endl;)

  int nops = ops.size();

  // a vector that will hold all the contractions
  std::vector<ElementaryContraction> contr_vec;

  PRINT(
      PrintLevel::Summary, cout << "\n  Operator   Space   Cre.   Ann.";
      cout << "\n  ------------------------------";
      for (int op = 0; op < nops; ++op) {
        for (int s = 0; s < orbital_subspaces->num_spaces(); s++) {
          cout << "\n      " << op << "        " << orbital_subspaces->label(s)
               << "      " << ops[op].cre(s) << "      " << ops[op].ann(s);
        }
      };
      cout << "\n";)

  // loop over orbital spaces
  for (int s = 0; s < orbital_subspaces->num_spaces(); s++) {
    PRINT(PrintLevel::Summary, std::cout
                                   << "\n  Elementary contractions for space "
                                   << orbital_subspaces->label(s) << ": ";)

    // differentiate between various types of spaces
    SpaceType space_type = orbital_subspaces->space_type(s);

    // 1. Pairwise contractions 1 cre + 1 ann operator:
    // ┌───┐
    // a^+ a
    if (space_type == SpaceType::Occupied) {
      elementary_contractions_occupied(ops, s, contr_vec);
    }

    // 2. Pairwise contractions 1 ann + 1 cre operator:
    // ┌───┐
    // a   a^+
    if (space_type == SpaceType::Unoccupied) {
      elementary_contractions_unoccupied(ops, s, contr_vec);
    }

    // 3. 2k-legged contractions (k >= 1) of k cre + k ann operators:
    // ┌───┬───┬───┐
    // a^+ a   a   a^+
    if (space_type == SpaceType::General) {
      elementary_contractions_general(ops, s, contr_vec);
    }
  }
  return contr_vec;
}

void WickTheorem::elementary_contractions_occupied(
    const OperatorProduct &ops, int s,
    std::vector<ElementaryContraction> &contr_vec) {
  int nops = ops.size();
  for (int l = 0; l < nops; l++) {             // loop over creation (left)
    for (int r = l + 1; r < nops; r++) {       // loop over annihilation (right)
      if (ops[l].cre(s) * ops[r].ann(s) > 0) { // is contraction viable?
        std::vector<GraphMatrix> new_contr(nops);
        new_contr[l].set_cre(s, 1);
        new_contr[r].set_ann(s, 1);
        contr_vec.push_back(new_contr);
        PRINT(PrintLevel::Summary,
              cout << std::format("\n    {:5d}:", contr_vec.size());
              PRINT_ELEMENTS(new_contr, " "););
      }
    }
  }
}

void WickTheorem::elementary_contractions_unoccupied(
    const OperatorProduct &ops, int s,
    std::vector<ElementaryContraction> &contr_vec) {
  int nops = ops.size();
  for (int l = 0; l < nops; l++) {             // loop over annihilation (left)
    for (int r = l + 1; r < nops; r++) {       // loop over creation (right)
      if (ops[l].ann(s) * ops[r].cre(s) > 0) { // is contraction viable?
        std::vector<GraphMatrix> new_contr(nops);
        new_contr[l].set_ann(s, 1);
        new_contr[r].set_cre(s, 1);
        contr_vec.push_back(new_contr);
        PRINT(PrintLevel::Summary,
              cout << std::format("\n    {:5d}:", contr_vec.size());
              PRINT_ELEMENTS(new_contr, " "););
      }
    }
  }
}

void WickTheorem::elementary_contractions_general(
    const OperatorProduct &ops, int s,
    std::vector<ElementaryContraction> &contr_vec) {
  int nops = ops.size();
  // compute the largest possible cumulant for this space
  int sumcre = 0;
  int sumann = 0;
  for (int A = 0; A < nops; A++) {
    sumcre += ops[A].cre(s);
    sumann += ops[A].ann(s);
  }
  // the number of legs is limited by the smallest of number of cre/ann
  // operators and the maximum cumulant level allowed
  int max_half_legs = std::min(std::min(sumcre, sumann), maxcumulant_);

  // in this algorithm we loop over all possible lengths of half-leg
  // contractions, partition this number into integers, permute these integers
  // to generate elementary contractions, and test if these are valid
  for (int half_legs = 1; half_legs <= max_half_legs; half_legs++) {
    PRINT(PrintLevel::Summary,
          cout << "\n    " << 2 * half_legs << "-legs contractions";)
    // create partitions of the number of half legs into at most nops numbers.
    // For half_legs = 2 and nops = 2, half_legs_part = [[2],[1,1]]
    auto half_legs_part = integer_partitions(half_legs, nops);
    // create lists of leg partitionings among all operators that are
    // compatible with the number of creation and annihilation operators
    //
    // these vectors store the number of cre/ann operators contracted per
    // operator
    std::vector<std::vector<int>> cre_legs_vec, ann_legs_vec;
    for (const auto part : half_legs_part) {
      // here we copy the partition and permute it (with added zeros, which
      // signify no contraction)
      std::vector<int> perm(nops, 0);
      std::copy(part.begin(), part.end(), perm.begin());
      std::sort(perm.begin(), perm.end());
      do {
        // check if compatible with creation/annihilation operators
        bool cre_compatible = true;
        bool ann_compatible = true;
        for (int A = 0; A < nops; A++) {
          if (ops[A].cre(s) < perm[A]) {
            cre_compatible = false;
          }
          if (ops[A].ann(s) < perm[A]) {
            ann_compatible = false;
          }
        }
        if (cre_compatible) {
          cre_legs_vec.push_back(perm);
        }
        if (ann_compatible) {
          ann_legs_vec.push_back(perm);
        }
      } while (std::next_permutation(perm.begin(), perm.end()));
    }

    // combine the creation and annihilation operators
    for (const auto cre_legs : cre_legs_vec) {
      for (const auto ann_legs : ann_legs_vec) {
        // count number of operators contracted
        int nops_contracted = 0;
        for (int A = 0; A < nops; A++) {
          nops_contracted += (cre_legs[A] + ann_legs[A] > 0);
        }
        // exclude operators that have legs only on one operator
        if (nops_contracted < 2)
          continue;
        // for a vector of GraphMatrix objects that represent this
        // contraction
        std::vector<GraphMatrix> new_contr(nops);
        for (int A = 0; A < nops; A++) {
          new_contr[A].set_cre(s, cre_legs[A]);
          new_contr[A].set_ann(s, ann_legs[A]);
        }
        contr_vec.push_back(new_contr);

        PRINT(PrintLevel::Summary,
              cout << std::format("\n    {:5d}:", contr_vec.size());
              PRINT_ELEMENTS(new_contr, " "););
      }
    }
  }
}
