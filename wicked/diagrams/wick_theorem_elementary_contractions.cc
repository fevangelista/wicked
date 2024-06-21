#include "fmt/format.h"

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
WickTheorem::generate_elementary_contractions(const OperatorProduct &ops, bool inter_general) {
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
      if (!inter_general){elementary_contractions_general(ops, s, contr_vec);}
    }
  }

  if (inter_general) {
    std::vector<int> s = orbital_subspaces->indices_of_type(SpaceType::General);
    if (s.size() == 2) {
      elementary_contractions_inter_general(ops, s, contr_vec);}
    else {
      for (int i = 0; i < s.size(); i++) {
        elementary_contractions_general(ops, i, contr_vec);
      }
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
              cout << fmt::format("\n    {:5d}:", contr_vec.size());
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
              cout << fmt::format("\n    {:5d}:", contr_vec.size());
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
              cout << fmt::format("\n    {:5d}:", contr_vec.size());
              PRINT_ELEMENTS(new_contr, " "););
      }
    }
  }
}

void WickTheorem::elementary_contractions_inter_general(
    const OperatorProduct &ops, std::vector<int> &s,
    std::vector<ElementaryContraction> &contr_vec) {
  int nops = ops.size();
  // compute the largest possible cumulant for this space
  int sumcre = 0;
  int sumann = 0;
  for (int A = 0; A < nops; A++) {
    for (int i = 0; i < s.size(); i++) {
      sumcre += ops[A].cre(s[i]);
      sumann += ops[A].ann(s[i]);
    }
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
          int sumcre_A = 0;
          int sumann_A = 0;
          for (int i = 0; i < s.size(); i++) {
            sumcre_A += ops[A].cre(s[i]);
            sumann_A += ops[A].ann(s[i]);
          }
          if (sumcre_A < perm[A]) {
            cre_compatible = false;
          }
          if (sumann_A < perm[A]) {
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

        std::vector<std::vector<std::vector<int>>> cre_legs_spaces(nops);
        std::vector<std::vector<std::vector<int>>> ann_legs_spaces(nops);
        for (int A = 0; A < nops; A++){
          for (int i = 0; i <= cre_legs[A]; i++){
            int legs1 = i;
            int legs2 = cre_legs[A]-i;
            if (legs1 <= ops[A].cre(s[0]) && legs2 <= ops[A].cre(s[1])){
              cre_legs_spaces[A].push_back({legs1, legs2});
            }
          }
          for (int i = 0; i <= ann_legs[A]; i++){
            int legs1 = i;
            int legs2 = ann_legs[A]-i;
            if (legs1 <= ops[A].ann(s[0]) && legs2 <= ops[A].ann(s[1])){
              ann_legs_spaces[A].push_back({legs1, legs2});
            }
          }
        }

        // the nc and na vectors are used to loop over all possible ways
        // to distribute the legs over the two general spaces
        std::vector<int> nc(nops,0);
        std::vector<int> na(nops,0);

        while (true){
          while (true){
            std::vector<GraphMatrix> new_contr(nops);
            for (int A = 0; A < nops; A++){
              new_contr[A].set_cre(s[0], cre_legs_spaces[A][nc[A]][0]);
              new_contr[A].set_cre(s[1], cre_legs_spaces[A][nc[A]][1]);
              new_contr[A].set_ann(s[0], ann_legs_spaces[A][na[A]][0]);
              new_contr[A].set_ann(s[1], ann_legs_spaces[A][na[A]][1]);
            }
            // only add the contraction if it is has Ms = 0
            if (ms_of_contraction(new_contr, s) == 0){
              contr_vec.push_back(new_contr);
              PRINT(PrintLevel::Summary,
                    cout << fmt::format("\n    {:5d}:", contr_vec.size());
                    PRINT_ELEMENTS(new_contr, " "););
            }
            // increment the counter vectors
            int A = 0;
            while (A < nops){
              if (nc[A] < cre_legs_spaces[A].size()-1){
                nc[A]++;
                break;
              }
              else{
                nc[A] = 0;
                A++;
              }
            }
            if (A == nops){
              break;
            }
          }
          int A = 0;
          while (A < nops){
            if (na[A] < ann_legs_spaces[A].size()-1){
              na[A]++;
              break;
            }
            else{
              na[A] = 0;
              A++;
            }
          }
          if (A == nops){
            break;
          }
        }
      }
    }
  }
}

int WickTheorem::ms_of_contraction(std::vector<GraphMatrix> &contr, std::vector<int> &s) {
  int ms = 0;
  int nops = contr.size();
  for (int A = 0; A < nops; A++) {
    // wlog, s[0] is assumed to have spin +1, s[1] is assumed to have spin -1
    ms += contr[A].cre(s[0]) - contr[A].cre(s[1]);
    ms -= contr[A].ann(s[0]) - contr[A].ann(s[1]);
  }
  return ms;
}