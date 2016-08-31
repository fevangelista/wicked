#include <iostream>

#include "stl_utils.hpp"
#include "orbital_space.h"
#include "combinatorics.h"
#include "wdiag_operator.h"
#include "wdiag_theorem.h"
#include "wcontractiongraph.h"

using namespace std;

WDiagTheorem::WDiagTheorem() {}

void WDiagTheorem::contract(double factor,
                            const std::vector<WDiagOperator> &ops) {

  std::cout << "\nContracting the following operators" << std::endl;
  for (auto &op : ops) {
    std::cout << op << std::endl;
  }

  auto basic_contractions = generate_basic_contractions(ops);

  //  /// Create the contraction graph
  //  WContractionGraph cg(factor,ops);
}

std::vector<std::vector<WDiagVertex>> WDiagTheorem::generate_basic_contractions(
    const std::vector<WDiagOperator> &ops) {

  int nops = ops.size();

  // holds all the contractions
  std::vector<std::vector<WDiagVertex>> contr_vec;

  // loop over orbital spaces
  for (int s = 0; s < osi->num_spaces(); s++) {
    std::cout << "\nBasic contractions for space " << s << std::endl;

    // differentiate between various types of spaces
    DMStructure dmstruc = osi->dmstructure(s);

    // Pairwise contractions creation-annihilation:
    // _____
    // |   |
    // a^+ a

    cout << "\n  ==> c/a pairwise contractions <==" << endl;
    // loop over the creation operators of each operator
    for (int c = 0; c < nops; c++) {
      // loop over the annihilation operators of each operator (right to the
      // creation operators)
      for (int a = c + 1; a < nops; a++) {
        // check if contraction is viable
        if ((ops[c].num_cre(s) > 0) and (ops[a].num_ann(s) > 0)) {
          std::vector<WDiagVertex> new_contr(nops);
          new_contr[c].cre(s, 1);
          new_contr[a].ann(s, 1);

          PRINT_ELEMENTS(new_contr, "      ** C: ");
          cout << endl;

          contr_vec.push_back(new_contr);
        }
      }
    }

    // Pairwise contractions creation-annihilation:
    // _____
    // |   |
    // a   a^+

    cout << "\n  ==> a/c pairwise contractions <==" << endl;
    // loop over the creation operators of each operator
    for (int a = 0; a < nops; a++) {
      // loop over the annihilation operators of each operator (right to the
      // creation operators)
      for (int c = a + 1; c < nops; c++) {
        // check if contraction is viable
        if ((ops[c].num_cre(s) > 0) and (ops[a].num_ann(s) > 0)) {
          std::vector<WDiagVertex> new_contr(nops);
          new_contr[c].cre(s, 1);
          new_contr[a].ann(s, 1);

          PRINT_ELEMENTS(new_contr, "      ** C: ");
          cout << endl;

          contr_vec.push_back(new_contr);
        }
      }
    }

    // 2k-legged contractions (k >= 2) of k creation and k annihilation
    // operators:
    // _____________
    // |   |   |   |
    // a^+ a   a   a^+

    if (dmstruc == DMStructure::General) {

      // compute the largest possible cumulant
      int sumcre = 0;
      int sumann = 0;
      for (int A = 0; A < nops; A++) {
        sumcre += ops[A].num_cre(s);
        sumann += ops[A].num_ann(s);
      }
      int max_half_legs = std::min(std::min(sumcre, sumann), maxcumulant_);
      int max_legs = 2 * max_half_legs;

      cout << "\n  Maximum cumulant considered has " << max_legs
           << " legs (max = " << 2 * std::min(sumcre, sumann) << ")" << endl;

      // loop over all possible contractions from 4 to max_legs
      for (int half_legs = 2; half_legs <= max_half_legs; half_legs++) {
        cout << "\n  ==> " << 2 * half_legs
             << "-legs cumulant contractions <==" << endl;
        auto half_legs_part = integer_partitions(half_legs);

        // create lists of leg partitionings compatible with the number of
        // creation and annihilation operators
        std::vector<std::vector<int>> cre_legs_vec, ann_legs_vec;
        for (const auto part : half_legs_part) {
          //          cout << "\n  Partition:";
          //          PRINT_ELEMENTS(part);
          if ((part.size() >= 2) and (part.size() <= nops)) {
            //            cout << " <- OK";
            std::vector<int> perm(nops, 0);
            std::copy(part.begin(), part.end(), perm.begin());
            std::sort(perm.begin(), perm.end());
            //            cout << "\n    Permutations:";
            do {
              //              PRINT_ELEMENTS(perm, "\n    ");
              // check if compatible with creation/annihilation operators
              bool cre_compatible = true;
              bool ann_compatible = true;
              for (int A = 0; A < nops; A++) {
                if (ops[A].num_cre(s) < perm[A]) {
                  cre_compatible = false;
                }
                if (ops[A].num_ann(s) < perm[A]) {
                  ann_compatible = false;
                }
              }
              if (cre_compatible) {
                //                cout << " <- c-compatible";
                cre_legs_vec.push_back(perm);
              }
              if (ann_compatible) {
                //                cout << " <- a-compatible";
                ann_legs_vec.push_back(perm);
              }
            } while (std::next_permutation(perm.begin(), perm.end()));
          }
          //          cout << endl;
        }

        // combine the creation and annihilation operators
        for (const auto cre_legs : cre_legs_vec) {
          for (const auto ann_legs : ann_legs_vec) {
            std::vector<WDiagVertex> new_contr(nops);
            for (int c = 0; c < nops; c++) {
              new_contr[c].cre(s, cre_legs[c]);
            }
            for (int a = 0; a < nops; a++) {
              new_contr[a].ann(s, ann_legs[a]);
            }
            PRINT_ELEMENTS(new_contr, "      ** C: ");
            cout << endl;

            contr_vec.push_back(new_contr);
          }
        }
      }
    }
  }
  return contr_vec;
}
