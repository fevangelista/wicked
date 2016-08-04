#include <iostream>

#include "combinatorics.h"
#include "helpers.h"
#include "wtheorem.h"

using namespace std;

#define PRINT(detail, code)                                                    \
  if (print_ >= detail) {                                                      \
    code                                                                       \
  }

WTheorem::WTheorem() {
  for (int n = 0; n <= 20; ++n) {
    integer_partitions_.push_back(integer_partitions(n));
  }

  make_contraction_partitions();

  make_contraction_skeletons();

  test_integer_partitions();
}

void WTheorem::theorem(const std::vector<WTerm> &terms) {}

void WTheorem::theorem_pair(const WTerm &A, const WTerm &B, int minrank,
                            int maxrank) {
  PRINT(Summary, cout << "Contracting the following terms:" << endl;
        cout << A.str() << endl; cout << B.str() << endl;)

  int nspace = osi->num_spaces();

  int naop = A.noperators();
  int nbop = B.noperators();

  // max number of contractions is largest multiple of two less than naop + nbop
  int maxcontr = (naop + nbop) / 2;

  std::vector<int> naop_space = A.noperators_per_space();
  std::vector<int> nbop_space = B.noperators_per_space();

  PRINT(Detailed, for (const auto &space
                       : {naop_space, nbop_space}) {
    cout << "Operators per space:        ";
    for (const auto &i : space) {
      cout << ' ' << i;
    }
    cout << endl;
  })

  std::vector<int> maxcontr_space;
  for (int s = 0; s < nspace; ++s) {
    // DMStructure dms = osi->dmstructure(s);
    maxcontr_space.push_back((naop_space[s] + nbop_space[s]) / 2);
  }

  PRINT(
      Detailed, cout << "Max contractions per space: "; for (const auto &i
                                                             : maxcontr_space) {
        cout << ' ' << i;
      } cout << endl;)

  // determine the number of ways we can contract the two operators
  int nways = 1; // the no-contraction case
  for (int s = 0; s < nspace; s++) {
    nways *= (1 + maxcontr_space[s]);
  }

  PRINT(Detailed, cout << "Operators can be contracted in " << nways << " ways"
                       << endl;)

  std::vector<int> contr_range;
  for (int i : maxcontr_space) {
    contr_range.push_back(i + 1);
  }

  product_space(contr_range, [&](const std::vector<int> &contr_per_space) {
    this->contract_pair(A, B, contr_per_space);
  });
}

void WTheorem::contract_pair(const WTerm &A, const WTerm &B,
                             const std::vector<int> &contr_per_space) {
  PRINT(Detailed, cout << "Contraction pattern:";
        for (int i
             : contr_per_space) { cout << ' ' << i; };
        cout << endl;)


}

void WTheorem::make_contraction_partitions() {
  for (int n = 0; n <= maxcumulant_; n++) {
    contr_part_t part;
    for (int i = 1; i < 2 * n; i++) {
      part.push_back(std::make_pair(2 * n - i, i));
    }
    contraction_partitions_.push_back(part);
  }

  PRINT(Detailed, int k = 0; for (auto cp
                                  : contraction_partitions_) {
    cout << 2 * k << "-legged contractions:" << endl;
    for (auto p : cp) {
      cout << p.first << " " << p.second << endl;
    }
    k++;
  })
}

void WTheorem::make_contraction_skeletons() {
  for (int n = 0; n <= maxskeleton_; ++n) {
    PRINT(Detailed, cout << "Number of contractions: " << n << endl;)
    // Partition the number of contractions (n)
    const auto &partitions = integer_partitions_[n];

    PRINT(Detailed, cout << "Type of " << n << "-contractions:" << endl;)
    for (const auto &partition : partitions) {
      PRINT(Detailed, for (const auto &i : partition) { cout << " " << i; })
      PRINT(Detailed, cout << endl;)

      std::map<int, int> count;
      for (int i : partition) {
        count[i] += 1;
      }
      for (auto kv : count) {
        PRINT(Detailed, cout << "class = " << kv.first
                             << " multp = " << kv.second << endl;)
      }
    }
  }
}

// std::vector<std::map<std::pair<int, int>, int>>
// WTheorem::make_contraction_skeletons(const std::vector<int>
// &contraction_list) {
//  std::vector<std::map<std::pair<int, int>, int>> result;
//  // Classify the type of partitionings of contractions
//  // E.g. a 2-contraction can be writtens as a 1+3-legs,2+2-legs,3+1-legs
//  //  std::map<int,std::vector<std::pair<int,int>>>

//  std::map<int, int> count;
//  for (int i : contraction_list) {
//    count[i] += 1;
//  }

//  return result;
//}
