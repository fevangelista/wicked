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
  test_integer_partitions();

  test_product_space();

  make_contraction_skeletons();
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

  std::vector<int> contr_range;
  for (int i : maxcontr_space) {
    contr_range.push_back(i + 1);
  }

  product_space_iterator(
      contr_range, [&](const std::vector<int> &contr_per_space) {
        this->contract_pair(A, naop_space, B, nbop_space, contr_per_space);
      });
}

void WTheorem::contract_pair(const WTerm &A, const std::vector<int> &naop,
                             const WTerm &B, const std::vector<int> &nbop,
                             const std::vector<int> &contr_per_space) {
  PRINT(Detailed, cout << "Contraction pattern:";
        for (int i
             : contr_per_space) { cout << ' ' << i; };
        cout << endl;)
  // At this point we are given two operators and the number of contractions
  // per space (contr_per_space).

  // For each space find out the list of all possible ways to split the
  // contractions among A and B
  std::vector<std::vector<std::tuple<int, int, int>>> contraction_splittings;
  std::vector<int> num_splitting_per_space;
  for (int s = 0; s < osi->num_spaces(); s++) {
    std::vector<std::tuple<int, int, int>> splittings;
    PRINT(Detailed, cout << "Space: " << s << endl;)
    //    PRINT(Detailed, cout << contr_per_space[s] << "-contractions with max"
    //    << : " << s << endl;)
    int lt = 2 * contr_per_space[s];
    for (int la = 1; la <= naop[s]; ++la) { // loop over the legs in A
      int lb = lt - la;
      if ((lb >= 1) and (lb <= nbop[s])) {
        splittings.push_back(std::make_tuple(lt, la, lb));
        PRINT(Detailed, cout << "  Skeleton (" << lt << "," << la << "," << lb
                             << ")" << endl;)
      }
    }
    // If no contractions are possible, add a void contraction
    if (splittings.size() == 0)
      splittings.push_back(std::make_tuple(0, 0, 0));
    contraction_splittings.push_back(splittings);
    num_splitting_per_space.push_back(splittings.size());
  }

  // Loop over all combinations of contraction splittings
  product_space_iterator(
      num_splitting_per_space,
      [&](const std::vector<int> &splitting_per_space) {
        PRINT(Detailed, cout << "  Picking combination:" << endl;)
        std::vector<std::tuple<int, int, int>> splitting_list;
        for (int s = 0; s < osi->num_spaces(); s++) {
          PRINT(Detailed, for (int i
                               : splitting_per_space) { cout << " " << i; })

          auto t = contraction_splittings[s][splitting_per_space[s]];
          splitting_list.push_back(
              contraction_splittings[s][splitting_per_space[s]]);
          PRINT(Detailed, cout << " -> (" << std::get<0>(t) << ","
                               << std::get<1>(t) << "," << std::get<2>(t) << ")"
                               << endl;)
          //            JUST introduced a bug somewhere here.
        }
        this->contract_pair_splitting(A, B, splitting_list);
      });
}

void WTheorem::contract_pair_splitting(
    const WTerm &A, const WTerm &B,
    const std::vector<std::tuple<int, int, int>> &splitting) {
  // look into the skeleton database to find all compatible splittings
  PRINT(
      Detailed, cout << "Contract using the following splitting ->";
      for (auto t
           : splitting) {
        cout << " (" << std::get<0>(t) << "," << std::get<1>(t) << ","
             << std::get<2>(t) << ")";
      } cout
      << endl;)
  std::vector<int> num_skeletons_per_space;
  for (auto t : splitting) {
    const auto search = skeletons_.find(t);
    if (search != skeletons_.end()) {
      int n = search->second.size();
      num_skeletons_per_space.push_back(n);
      PRINT(Detailed,
            cout << "Found " << n
                 << " skeleton(s) that match(es) this splitting:" << endl;
            int k = 0; for (const auto &s_vec
                            : search->second) {
              cout << k + 1 << ":";
              for (const auto &s : s_vec) {
                cout << " (" << s.first << "," << s.second << ")" << endl;
              }
              k++;
            });
    } else {
      PRINT(Detailed, cout << "Found no skeletons that match this splitting"
                           << endl;);
      num_skeletons_per_space.push_back(0);
    }
  }

  product_space_iterator(
      num_skeletons_per_space,
      [&](const std::vector<int> &skeletons_per_space) {
        PRINT(Summary, cout << "  Picking the following contractions:" << endl;)
        for (int s = 0; s < osi->num_spaces(); s++) {
          int sk = skeletons_per_space[s];
          const auto &key = splitting[s];
          const auto &skeleton = skeletons_[key][skeletons_per_space[s]];
          for (auto p : skeleton) {
            cout << " (" << p.first << "," << p.second << ")";
          }
          cout << endl;
        }
      });
}

void WTheorem::make_contraction_skeletons() {
  // the null contraction
  std::tuple<int, int, int> key(0, 0, 0);
  skeletons_[key].push_back({std::make_pair(0, 0)});

  for (int n = 0; n <= maxskeleton_; ++n) {
    PRINT(Detailed, cout << "\nNumber of contractions: " << n << endl;)
    // Partition the number of contractions (n)
    const auto &partitions = integer_partitions_[n];

    PRINT(Detailed, cout << "Type of " << n << "-contractions:" << endl;)
    for (const auto &partition : partitions) {
      PRINT(
          Detailed, std::vector<std::string> s; for (const auto &i
                                                     : partition) {
            s.push_back(to_string(i));
          } cout << "  "
                 << to_string(s, "+");)
      PRINT(Detailed, cout << " | ";)

      std::map<int, int> leg_count;
      int highest_cumulant = 0;
      for (int i : partition) {
        leg_count[i] += 1;
        highest_cumulant = std::max(highest_cumulant, i);
      }

      std::vector<int> nlegs;
      std::vector<int> ntypes;
      std::vector<int> multp;
      std::vector<std::string> s;

      for (auto kv : leg_count) {
        PRINT(Detailed, s.push_back(to_string(kv.first * 2) + "-leg x " +
                                    to_string(kv.second)););
        nlegs.push_back(2 * kv.first);
        ntypes.push_back(2 * kv.first - 1);
        multp.push_back(kv.second);
      }
      PRINT(Detailed, cout << to_string(s, " + "););

      PRINT(Detailed, cout << " (highest cumulant: " << highest_cumulant << ")"
                           << endl;);

      if (highest_cumulant > maxcumulant_)
        continue;
      auto sets = generalized_combinations_with_repetitions(ntypes, multp);
      for (auto &set : sets) {
        std::vector<std::pair<int, int>> contraction_skeleton;
        int nlegs_left_total = 0;
        int nlegs_right_total = 0;

        PRINT(Detailed, cout << "    ";)
        for (int n = 0; n < nlegs.size(); n++) {
          for (int i : set[n]) {
            int nlegs_left = i + 1;
            int nlegs_right = nlegs[n] - i - 1;
            PRINT(Detailed, cout << nlegs[n] << "-leg(" << nlegs_left << "|"
                                 << nlegs_right << ")  ";)
            nlegs_left_total += nlegs_left;
            nlegs_right_total += nlegs_right;
            contraction_skeleton.push_back(
                std::make_pair(nlegs_left, nlegs_right));
          }
        }
        PRINT(Detailed, cout << " -> (" << nlegs_left_total << "|"
                             << nlegs_right_total << ")" << endl;)
        // save this skeleton
        std::tuple<int, int, int> key(2 * n, nlegs_left_total,
                                      nlegs_right_total);
        skeletons_[key].push_back(contraction_skeleton);
      }
      PRINT(Detailed, cout << endl;)
    }
  }

  std::vector<std::tuple<int, int, int>> sorted_keys;
  for (auto &kv : skeletons_) {
    sorted_keys.push_back(kv.first);
  }
  std::sort(sorted_keys.begin(), sorted_keys.end());

  PRINT(Detailed, for (auto &k
                       : sorted_keys) {
    cout << "Total legs:" << std::get<0>(k) << " (" << std::get<1>(k) << "|"
         << std::get<2>(k) << ")" << endl;
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
  })
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
