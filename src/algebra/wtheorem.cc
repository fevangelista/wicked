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

  test_product_space();
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

  product_space_iterator(contr_range,
                         [&](const std::vector<int> &contr_per_space) {
                           this->contract_pair(A, B, contr_per_space);
                         });

  //  product_space_iterator({2,3,2}, [&](const std::vector<int> &vec) {
  //    for (int i : vec) {
  //      cout << i << ' ';
  //    }
  //    cout << endl;
  //  });

  //  product_space_iterator({2,4,2}, [&](const std::vector<int> &vec) {
  //    for (int i : vec) {
  //      cout << i << ' ';
  //    }
  //    cout << endl;
  //  });

  //  generalized_combinations_with_repetitions_iterator({2, 2, 2}, {2, 2, 2},
  //                             [&](const std::vector<std::vector<int>> &res) {
  //                               for (auto &vec : res) {
  //                                 for (int i : vec) {
  //                                   cout << i << ' ';
  //                                 }
  //                                 cout << endl;
  //                               }
  //                               cout << endl;
  //                             });

  //  generalized_combinations_with_repetitions_iterator({2, 3, 3}, {2, 2, 2},
  //                             [&](const std::vector<std::vector<int>> &res) {
  //                               for (auto &vec : res) {
  //                                 for (int i : vec) {
  //                                   cout << i << ' ';
  //                                 }
  //                                 cout << endl;
  //                               }
  //                               cout << endl;
  //                             });
}

void WTheorem::contract_pair(const WTerm &A, const WTerm &B,
                             const std::vector<int> &contr_per_space) {
  PRINT(Detailed, cout << "Contraction pattern:";
        for (int i
             : contr_per_space) { cout << ' ' << i; };
        cout << endl;)
  // At this point we are given two operators and the number of contractions per
  // space (contr_per_space).

  // For each space find out the list of all possible contractions
  for (int s = 0; s < osi->num_spaces(); s++) {
  }
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

  //  std::vector<std::vector<std::pair<int, int>>> contraction_partition_;
  //  for ()

  for (int n = 0; n <= maxskeleton_; ++n) {
    PRINT(Detailed, cout << "\nNumber of contractions: " << n << endl;)
    // Partition the number of contractions (n)
    const auto &partitions = integer_partitions_[n];

    PRINT(Detailed, cout << "Type of " << n << "-contractions:" << endl;)
    for (const auto &partition : partitions) {
      PRINT(Detailed, std::vector<std::string> s; for (const auto &i
                                                       : partition) {
        s.push_back(to_string(i));
      } cout << to_string(s, "+");)
      PRINT(Detailed, cout << " | ";)

      std::map<int, int> leg_count;
      for (int i : partition) {
        leg_count[i] += 1;
      }
      std::vector<int> nlegs;
      std::vector<int> ntypes;
      std::vector<int> multp;
      std::vector<std::string> s;
      //      PRINT(Detailed, )

      for (auto kv : leg_count) {
        PRINT(Detailed, s.push_back(to_string(kv.first * 2) + "-leg x " +
                                    to_string(kv.second)););
        nlegs.push_back(2 * kv.first);
        ntypes.push_back(2 * kv.first - 1);
        multp.push_back(kv.second);
      }
      PRINT(Detailed, cout << to_string(s, " + ") << endl;);
      auto sets = generalized_combinations_with_repetitions(ntypes, multp);
      for (auto &set : sets) {
        std::vector<std::pair<int, int>> contraction_skeleton;
        int nlegs_left_total = 0;
        int nlegs_right_total = 0;
        for (int n = 0; n < nlegs.size(); n++) {
          for (int i : set[n]) {
            int nlegs_left = i + 1;
            int nlegs_right = nlegs[n] - i - 1;
            cout << nlegs[n] << "-leg(" << nlegs_left << "|" << nlegs_right
                 << ")  ";
            nlegs_left_total += nlegs_left;
            nlegs_right_total += nlegs_right;
            contraction_skeleton.push_back(
                std::make_pair(nlegs_left, nlegs_right));
          }
        }
        cout << " -> (" << nlegs_left_total << "|" << nlegs_right_total << ")"
             << endl;
        // save this skeleton
        std::tuple<int, int, int> key(2 * n, nlegs_left_total, nlegs_right_total);
        skeletons_[key].push_back(contraction_skeleton);
      }
      cout << endl;
    }
  }

  std::vector<std::tuple<int, int, int>> sorted_keys;
  for (auto &kv : skeletons_) {
    sorted_keys.push_back(kv.first);
  }
  std::sort(sorted_keys.begin(), sorted_keys.end());

  for (auto &k : sorted_keys) {
      cout << "Total legs:" << std::get<0>(k) << " (" << std::get<1>(k) << "|" << std::get<2>(k) << ")" << endl;
    const auto &skeleton_familty = skeletons_[k];
    for (auto &el : skeleton_familty) {
      std::vector<std::string> str;
      for (auto p : el) {
        str.push_back("(" + to_string(p.first) + "|" + to_string(p.second) +
                      ")");
      }
      cout << to_string(str, " ");
      cout << endl;
    }
    cout << endl;
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
