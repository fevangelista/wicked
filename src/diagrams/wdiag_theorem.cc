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

  ncontractions_ = 0;
  contractions_.clear();
  elementary_contractions_.clear();

  std::cout << "\nContracting the following operators:" << std::endl;
  for (auto &op : ops) {
    std::cout << "    " << op;
  }

  elementary_contractions_ = generate_elementary_contractions(ops);

  std::vector<int> a(50, -1);
  // create a vector of
  std::vector<WDiagVertex> free_vertex_vec;
  for (const auto &op : ops) {
    free_vertex_vec.push_back(op.vertex());
  }

  std::cout << "\nPossible contractions:" << std::endl;

  generate_contractions(a, 0, elementary_contractions_, free_vertex_vec);

  std::cout << "\nTotal:" << ncontractions_ << std::endl;
}

void WDiagTheorem::generate_contractions(
    std::vector<int> a, int k,
    const std::vector<std::vector<WDiagVertex>> &el_contr_vec,
    std::vector<WDiagVertex> &free_vertex_vec) {

  process_contraction(a, k, free_vertex_vec);

  k = k + 1;
  std::vector<int> candidates =
      construct_candidates(a, k, el_contr_vec, free_vertex_vec);

  for (const auto &c : candidates) {
    a[k - 1] = c;
    make_move(a, k, el_contr_vec, free_vertex_vec);
    generate_contractions(a, k, el_contr_vec, free_vertex_vec);
    unmake_move(a, k, el_contr_vec, free_vertex_vec);
  }
}

std::vector<int> WDiagTheorem::construct_candidates(
    std::vector<int> &a, int k,
    const std::vector<std::vector<WDiagVertex>> &el_contr_vec,
    const std::vector<WDiagVertex> &free_vertex_vec) {

  std::vector<int> candidates;
  int nops = free_vertex_vec.size();

  // determine the last elementary contraction used
  int minc = (k > 1) ? a[k - 2] : 0;
  int maxc = el_contr_vec.size();

  // loop over all potentially viable contractions
  for (int c = minc; c < maxc; c++) {
    const auto &el_contr = el_contr_vec[c];

    // test if this contraction is compatible, that is if the number of
    // operators we want to contract is less than or equal to the number of free
    // (uncontracted) operators
    bool compatible = true;
    for (int A = 0; A < nops; A++) {
      for (int s = 0; s < osi->num_spaces(); s++) {
        if (free_vertex_vec[A].cre(s) < el_contr[A].cre(s)) {
          compatible = false;
        }
        if (free_vertex_vec[A].ann(s) < el_contr[A].ann(s)) {
          compatible = false;
        }
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
    std::vector<WDiagVertex> &free_vertex_vec) {
  int nops = free_vertex_vec.size();

  // remove the current elementary contraction
  int c = a[k - 1];
  const auto &el_contr = el_contr_vec[c];

  for (int A = 0; A < nops; A++) {
    for (int s = 0; s < osi->num_spaces(); s++) {
      int ncre = free_vertex_vec[A].cre(s) - el_contr[A].cre(s);
      free_vertex_vec[A].cre(s, ncre);
      int nann = free_vertex_vec[A].ann(s) - el_contr[A].ann(s);
      free_vertex_vec[A].ann(s, nann);
    }
  }
}

void WDiagTheorem::unmake_move(
    const std::vector<int> &a, int k,
    const std::vector<std::vector<WDiagVertex>> &el_contr_vec,
    std::vector<WDiagVertex> &free_vertex_vec) {
  int nops = free_vertex_vec.size();

  // remove the current elementary contraction
  int c = a[k - 1];
  const auto &el_contr = el_contr_vec[c];

  for (int A = 0; A < nops; A++) {
    for (int s = 0; s < osi->num_spaces(); s++) {
      int ncre = free_vertex_vec[A].cre(s) + el_contr[A].cre(s);
      free_vertex_vec[A].cre(s, ncre);
      int nann = free_vertex_vec[A].ann(s) + el_contr[A].ann(s);
      free_vertex_vec[A].ann(s, nann);
    }
  }
}

void WDiagTheorem::process_contraction(
    const std::vector<int> &a, int k,
    std::vector<WDiagVertex> &free_vertex_vec) {
  contractions_.push_back(std::vector<int>(a.begin(), a.begin() + k));
  cout << " ";
  for (int i = 0; i < k; ++i) {
    cout << " " << a[i];
  }
  cout << endl;
  ncontractions_++;
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
          contr_vec.push_back(new_contr);

          PRINT_ELEMENTS(new_contr, "      ");
          cout << endl;
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
          contr_vec.push_back(new_contr);

          PRINT_ELEMENTS(new_contr, "      ");
          cout << endl;
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

      // loop over all possible contractions from 4 to max_legs
      for (int half_legs = 2; half_legs <= max_half_legs; half_legs++) {
        cout << "\n    * " << 2 * half_legs << "-legs cumulant contractions"
             << endl;
        auto half_legs_part = integer_partitions(half_legs);

        // create lists of leg partitionings among all operators that are
        // compatible with the number of creation and annihilation operators
        std::vector<std::vector<int>> cre_legs_vec, ann_legs_vec;
        for (const auto part : half_legs_part) {
          if (part.size() <= nops) {
            std::vector<int> perm(nops, 0);
            std::copy(part.begin(), part.end(), perm.begin());
            std::sort(perm.begin(), perm.end());
            do {
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
                cre_legs_vec.push_back(perm);
              }
              if (ann_compatible) {
                ann_legs_vec.push_back(perm);
              }
            } while (std::next_permutation(perm.begin(), perm.end()));
          }
        }

        // combine the creation and annihilation operators
        for (const auto cre_legs : cre_legs_vec) {
          for (const auto ann_legs : ann_legs_vec) {
            std::vector<WDiagVertex> new_contr(nops);
            int ncontracted = 0;
            for (int A = 0; A < nops; A++) {
              new_contr[A].cre(s, cre_legs[A]);
              new_contr[A].ann(s, ann_legs[A]);
              // count number of operators contracted
              if (cre_legs[A] + ann_legs[A] > 0){
                  ncontracted += 1;
              }
            }
            // exclude operators that have legs only on one operator
            if (ncontracted > 1){
                contr_vec.push_back(new_contr);
                PRINT_ELEMENTS(new_contr, "      ");
                cout << endl;
            }
          }
        }
      }
    }
  }
  return contr_vec;
}
