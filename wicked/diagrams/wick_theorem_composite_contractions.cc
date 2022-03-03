#include <iostream>
#include <vector>

#include "fmt/format.h"

#include "contraction.h"
#include "diag_operator.h"
#include "diag_vertex.h"

#include "wick_theorem.h"

#define PRINT(detail, code)                                                    \
  if (print_ >= detail) {                                                      \
    code                                                                       \
  }

using namespace std;

void WickTheorem::generate_composite_contractions(
    const std::vector<DiagOperator> &ops, const int minrank,
    const int maxrank) {
  PRINT(PrintLevel::Summary,
        std::cout << "\n- Step 2. Generating composite contractions"
                  << std::endl;)

  // this vector is used in the backtracking algorithm to hold the list of
  // elementary contractions
  std::vector<int> a(100, -1);
  // create a vector that keeps track of the free (uncontracted vertices)
  std::vector<DiagVertex> free_vertex_vec;
  for (const auto &op : ops) {
    free_vertex_vec.push_back(op.vertex());
  }
  PRINT(PrintLevel::Summary,
        std::cout << "\n    Contractions found by backtracking:";
        std::cout
        << "\n\n      N   Op. Rank  Elementary      Uncontracted operators";
        std::cout << "\n                    Contractions ";
        std::cout
        << "\n    "
           "----------------------------------------------------------";)

  // generate all contractions by backtracking
  generate_contractions_backtrack(a, 0, elementary_contractions_,
                                  free_vertex_vec, minrank, maxrank);
  PRINT(PrintLevel::Summary, std::cout << "\n\n    Total contractions: "
                                       << ncontractions_ << std::endl;)
}

void WickTheorem::generate_contractions_backtrack(
    std::vector<int> a, int k,
    const std::vector<ElementaryContraction> &el_contr_vec,
    std::vector<DiagVertex> &free_vertex_vec, const int minrank,
    const int maxrank) {

  // process this contraction
  process_contraction(a, k, free_vertex_vec, minrank, maxrank);

  // build a list of candidate contractions to add to this solution
  k = k + 1;
  std::vector<int> candidates =
      construct_candidates(a, k, el_contr_vec, free_vertex_vec);

  // test each candidate contraction
  for (const auto &c : candidates) {
    make_move(a, k, c, el_contr_vec, free_vertex_vec);
    generate_contractions_backtrack(a, k, el_contr_vec, free_vertex_vec,
                                    minrank, maxrank);
    unmake_move(a, k, c, el_contr_vec, free_vertex_vec);
  }
}

void WickTheorem::process_contraction(
    const std::vector<int> &a, int k,
    const std::vector<DiagVertex> &free_vertex_vec, const int minrank,
    const int maxrank) {
  int num_ops = sum_num_ops(free_vertex_vec);
  if ((num_ops >= minrank) and (num_ops <= maxrank)) {
    contractions_.push_back(std::vector<int>(a.begin(), a.begin() + k));
    ncontractions_++;
    PRINT(
        PrintLevel::Summary, DiagVertex free_ops;
        for (const auto &free_vertex
             : free_vertex_vec) { free_ops += free_vertex; };
        cout << fmt::format("\n  {:5d}    {:3d}    ", ncontractions_ + 1,
                            free_ops.num_ops());
        for (int i = 0; i < k; ++i) { cout << fmt::format(" {:3d}", a[i]); };
        cout << std::string(std::max(24 - 4 * k, 2), ' ') << free_ops;)
  }
}

std::vector<int> WickTheorem::construct_candidates(
    std::vector<int> &a, int k,
    const std::vector<ElementaryContraction> &el_contr_vec,
    const std::vector<DiagVertex> &free_vertex_vec) {

  std::vector<int> candidates;
  int nops = free_vertex_vec.size();

  // determine the last elementary contraction used
  // the -2 is here because k is incremented just before calling this function
  int minc = (k > 1) ? a[k - 2] : 0;
  int maxc = el_contr_vec.size();

  // loop over all potentially viable contractions
  for (int c = minc; c < maxc; c++) {
    const auto &el_contr = el_contr_vec[c];

    // test if this contraction is valid: check that the number of
    // operators to contract is less than or equal to the number of
    // free (uncontracted) operators
    bool is_valid_contraction = true;
    for (int A = 0; A < nops; A++) {
      for (int s = 0; s < osi->num_spaces(); s++) {
        if (free_vertex_vec[A].cre(s) < el_contr[A].cre(s)) {
          is_valid_contraction = false;
        }
        if (free_vertex_vec[A].ann(s) < el_contr[A].ann(s)) {
          is_valid_contraction = false;
        }
      }
    }
    if (is_valid_contraction) {
      candidates.push_back(c);
    }
  }
  return candidates;
}

void WickTheorem::make_move(
    std::vector<int> &a, int k, int c,
    const std::vector<ElementaryContraction> &el_contr_vec,
    std::vector<DiagVertex> &free_vertex_vec) {
  // add this contraction to the solution
  a[k - 1] = c;

  // update the number of uncontracted operators
  const auto &el_contr = el_contr_vec[c];
  int nops = free_vertex_vec.size();
  for (int A = 0; A < nops; A++) {
    free_vertex_vec[A] -= el_contr[A];
  }
}

void WickTheorem::unmake_move(
    std::vector<int> &a, int k, int c,
    const std::vector<ElementaryContraction> &el_contr_vec,
    std::vector<DiagVertex> &free_vertex_vec) {

  // remove this contraction from the solution
  a[k - 1] = -1;

  // update the number of uncontracted operators
  const auto &el_contr = el_contr_vec[c];
  int nops = free_vertex_vec.size();
  for (int A = 0; A < nops; A++) {
    free_vertex_vec[A] += el_contr[A];
  }
}