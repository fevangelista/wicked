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

  std::cout << "\nContracting the following operators:" << std::endl;
  for (auto &op : ops) {
    std::cout << "    " << op;
  }

  auto elementary_contractions = generate_elementary_contractions(ops);

  bt_finished_ = false;

  std::vector<int> a(50, -1);
  std::vector<WDiagVertex> free_vertices;
  for (const auto &op : ops) {
    free_vertices.push_back(op.vertex());
  }

  std::cout << "\nPossible contractions:" << std::endl;

  generate_contractions(a, 0, elementary_contractions, free_vertices);
}

void WDiagTheorem::generate_contractions(
    std::vector<int> a, int k,
    const std::vector<std::vector<WDiagVertex>> &el_contr_vec,
    std::vector<WDiagVertex> &free_vertices) {

  cout << " ";
  for (int i = 0; i < k; ++i) {
    cout << " " << a[i];
  }
  cout << endl;

  k = k + 1;
  std::vector<int> candidates =
      construct_candidates(a, k, el_contr_vec, free_vertices);

//  PRINT_ELEMENTS(candidates, "\n  candidates:"); cout << endl;

  for (const auto &c : candidates) {
    a[k - 1] = c;
    make_move(a, k, el_contr_vec, free_vertices);
    generate_contractions(a, k, el_contr_vec, free_vertices);
    unmake_move(a, k, el_contr_vec, free_vertices);
  }
}

std::vector<int> WDiagTheorem::construct_candidates(
    std::vector<int> &a, int k,
    const std::vector<std::vector<WDiagVertex>> &el_contr_vec,
    const std::vector<WDiagVertex> &free_vertices) {

  std::vector<int> candidates;

  int nops = free_vertices.size();

  // determine the last elementary contraction used
  int minc = (k > 1) ? a[k - 2] : 0;
  int maxc = el_contr_vec.size();

//  cout << "\n  Min C: " << minc << "  Max C: " << maxc;

  // loop over all potentially viable contractions
  for (int c = minc; c < maxc; c++) {
    const auto &el_contr = el_contr_vec[c];

//    PRINT_ELEMENTS(free_vertices, "\n");
//    PRINT_ELEMENTS(el_contr, "\n");

    // test if this contraction is compatible
    bool compatible = true;
    for (int A = 0; A < nops; A++) {
      for (int s = 0; s < osi->num_spaces(); s++) {
        if (free_vertices[A].cre(s) < el_contr[A].cre(s))
          compatible = false;
        if (free_vertices[A].ann(s) < el_contr[A].ann(s))
          compatible = false;
      }
    }
    if (compatible) {
      candidates.push_back(c);
    }
  }
  return candidates;
}

void WDiagTheorem::make_move(
    const std::vector<int> &a, int k,
    const std::vector<std::vector<WDiagVertex>> &el_contr_vec,
    std::vector<WDiagVertex> &free_vertices) {
  int nops = free_vertices.size();

  // remove the current elementary contraction
  int c = a[k - 1];
  const auto &el_contr = el_contr_vec[c];

  for (int A = 0; A < nops; A++) {
    for (int s = 0; s < osi->num_spaces(); s++) {
      int ncre = free_vertices[A].cre(s) - el_contr[A].cre(s);
      free_vertices[A].cre(s, ncre);
      int nann = free_vertices[A].ann(s) - el_contr[A].ann(s);
      free_vertices[A].ann(s, nann);
    }
  }
}

void WDiagTheorem::unmake_move(
    const std::vector<int> &a, int k,
    const std::vector<std::vector<WDiagVertex>> &el_contr_vec,
    std::vector<WDiagVertex> &free_vertices) {
  int nops = free_vertices.size();

  // remove the current elementary contraction
  int c = a[k - 1];
  const auto &el_contr = el_contr_vec[c];

  for (int A = 0; A < nops; A++) {
    for (int s = 0; s < osi->num_spaces(); s++) {
      int ncre = free_vertices[A].cre(s) + el_contr[A].cre(s);
      free_vertices[A].cre(s, ncre);
      int nann = free_vertices[A].ann(s) + el_contr[A].ann(s);
      free_vertices[A].ann(s, nann);
    }
  }
}

std::vector<std::vector<WDiagVertex>>
WDiagTheorem::generate_elementary_contractions(
    const std::vector<WDiagOperator> &ops) {

  int nops = ops.size();

  // holds all the contractions
  std::vector<std::vector<WDiagVertex>> contr_vec;

  // loop over orbital spaces
  for (int s = 0; s < osi->num_spaces(); s++) {
    std::cout << "\n  => Basic contractions for space " << s << std::endl;

    // differentiate between various types of spaces
    DMStructure dmstruc = osi->dmstructure(s);

    // Pairwise contractions creation-annihilation:
    // _____
    // |   |
    // a^+ a

    cout << "\n    * c/a pairwise contractions" << endl;
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

          PRINT_ELEMENTS(new_contr, "      ");
          cout << endl;

          contr_vec.push_back(new_contr);
        }
      }
    }

    // Pairwise contractions creation-annihilation:
    // _____
    // |   |
    // a   a^+

    cout << "\n    * a/c pairwise contractions" << endl;
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

          PRINT_ELEMENTS(new_contr, "      ");
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

//      cout << "\n  Maximum cumulant considered has " << max_legs
//           << " legs (max = " << 2 * std::min(sumcre, sumann) << ")" << endl;

      // loop over all possible contractions from 4 to max_legs
      for (int half_legs = 2; half_legs <= max_half_legs; half_legs++) {
        cout << "\n    * " << 2 * half_legs
             << "-legs cumulant contractions" << endl;
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
            PRINT_ELEMENTS(new_contr, "      ");
            cout << endl;

            contr_vec.push_back(new_contr);
          }
        }
      }
    }
  }
  return contr_vec;
}
