//#include <assert.h>
//#include <iostream>

//#include "combinatorics.h"
//#include "helpers.h"
//#include "wtheorem.h"
//#include "stl_utils.hpp"

// using namespace std;

//#define PRINT(detail, code)                                                    \
//  if (print_ >= detail) {                                                      \
//    code                                                                       \
//  }

// WTheorem::WTheorem() {
//  for (int n = 0; n <= 20; ++n) {
//    integer_partitions_.push_back(integer_partitions(n));
//  }
//  test_integer_partitions();

//  test_product_space();

//  make_contraction_skeletons();
//}

// void WTheorem::theorem(const std::vector<WSymbolicTerm> &terms) {}

// void WTheorem::theorem_pair(const WSymbolicTerm &A, const WSymbolicTerm &B,
// int minrank,
//                            int maxrank) {
//  PRINT(Summary, cout << "Contracting the following terms:" << endl;
//        cout << A.str() << endl; cout << B.str() << endl;)

//  int nspace = osi->num_spaces();

//  int naop = A.noperators();
//  int nbop = B.noperators();

//  // max number of contractions is largest multiple of two less than naop +
//  nbop int maxcontr = (naop + nbop) / 2;

//  std::vector<int> naop_space = A.noperators_per_space();
//  std::vector<int> nbop_space = B.noperators_per_space();

//  PRINT(Detailed, for (const auto &space
//                       : {naop_space, nbop_space}) {
//    cout << "Operators per space:        ";
//    for (const auto &i : space) {
//      cout << ' ' << i;
//    }
//    cout << endl;
//  })

//  std::vector<int> maxcontr_space;
//  for (int s = 0; s < nspace; ++s) {
//    // RDMType dms = osi->dmstructure(s);
//    maxcontr_space.push_back((naop_space[s] + nbop_space[s]) / 2);
//  }

//  PRINT(
//      Detailed, cout << "Max contractions per space: "; for (const auto &i
//                                                             : maxcontr_space)
//                                                             {
//        cout << ' ' << i;
//      } cout << endl;)

//  std::vector<int> contr_range;
//  for (int i : maxcontr_space) {
//    contr_range.push_back(i + 1);
//  }

//  product_space_iterator(
//      contr_range, [&](const std::vector<int> &contr_per_space) {
//        this->contract_pair(A, naop_space, B, nbop_space, contr_per_space);
//      });
//}

// void WTheorem::contract_pair(const WSymbolicTerm &A, const std::vector<int>
// &naop,
//                             const WSymbolicTerm &B, const std::vector<int>
//                             &nbop, const std::vector<int> &contr_per_space) {
//  PRINT(Detailed, cout << "Contraction pattern:";
//        for (int i
//             : contr_per_space) { cout << ' ' << i; };
//        cout << endl;)
//  // At this point we are given two operators and the number of contractions
//  // per space (contr_per_space).

//  // For each space find out the list of all possible ways to split the
//  // contractions among A and B
//  std::vector<std::vector<std::tuple<int, int, int>>> contraction_splittings;
//  std::vector<int> num_splitting_per_space;
//  for (int s = 0; s < osi->num_spaces(); s++) {
//    std::vector<std::tuple<int, int, int>> splittings;
//    PRINT(Detailed, cout << "Space: " << s << endl;)
//    //    PRINT(Detailed, cout << contr_per_space[s] << "-contractions with
//    max"
//    //    << : " << s << endl;)
//    int lt = 2 * contr_per_space[s];
//    for (int la = 1; la <= naop[s]; ++la) { // loop over the legs in A
//      int lb = lt - la;
//      if ((lb >= 1) and (lb <= nbop[s])) {
//        splittings.push_back(std::make_tuple(lt, la, lb));
//        PRINT(Detailed, cout << "  Skeleton (" << lt << "," << la << "," << lb
//                             << ")" << endl;)
//      }
//    }
//    // If no contractions are possible, add a void contraction
//    if (splittings.size() == 0)
//      splittings.push_back(std::make_tuple(0, 0, 0));
//    contraction_splittings.push_back(splittings);
//    num_splitting_per_space.push_back(splittings.size());
//  }

//  // Loop over all combinations of contraction splittings
//  product_space_iterator(
//      num_splitting_per_space,
//      [&](const std::vector<int> &splitting_per_space) {
//        PRINT(Detailed, cout << "  Picking combination:" << endl;)
//        std::vector<std::tuple<int, int, int>> splitting_list;
//        for (int s = 0; s < osi->num_spaces(); s++) {
//          PRINT(Detailed, for (int i
//                               : splitting_per_space) { cout << " " << i; })

//          auto t = contraction_splittings[s][splitting_per_space[s]];
//          splitting_list.push_back(
//              contraction_splittings[s][splitting_per_space[s]]);
//          PRINT(Detailed, cout << " -> (" << std::get<0>(t) << ","
//                               << std::get<1>(t) << "," << std::get<2>(t) <<
//                               ")"
//                               << endl;)
//        }
//        this->contract_pair_splitting(A, B, splitting_list);
//      });
//}

// void WTheorem::contract_pair_splitting(
//    const WSymbolicTerm &A, const WSymbolicTerm &B,
//    const std::vector<std::tuple<int, int, int>> &splitting) {
//  // look into the skeleton database to find all compatible splittings
//  PRINT(
//      Detailed, cout << "Contract using the following splitting ->";
//      for (auto t
//           : splitting) {
//        cout << " (" << std::get<0>(t) << "," << std::get<1>(t) << ","
//             << std::get<2>(t) << ")";
//      } cout
//      << endl;)
//  std::vector<int> num_skeletons_per_space;
//  for (auto t : splitting) {
//    const auto search = skeletons_.find(t);
//    if (search != skeletons_.end()) {
//      int n = search->second.size();
//      num_skeletons_per_space.push_back(n);
//      PRINT(Detailed,
//            cout << "Found " << n
//                 << " skeleton(s) that match(es) this splitting:" << endl;
//            int k = 0; for (const auto &s_vec
//                            : search->second) {
//              cout << k + 1 << ":";
//              for (const auto &s : s_vec) {
//                cout << " (" << s.first << "," << s.second << ")" << endl;
//              }
//              k++;
//            });
//    } else {
//      PRINT(Detailed, cout << "Found no skeletons that match this splitting"
//                           << endl;);
//      num_skeletons_per_space.push_back(0);
//    }
//  }

//  product_space_iterator(
//      num_skeletons_per_space,
//      [&](const std::vector<int> &skeletons_per_space) {
//        std::vector<std::vector<std::pair<int, int>>> contractions;
//        PRINT(Summary, cout << "  Picking the following contractions:" <<
//        endl;) for (int s = 0; s < osi->num_spaces(); s++) {
//          int sk = skeletons_per_space[s];
//          const auto &key = splitting[s];
//          const auto &skeleton = skeletons_[key][skeletons_per_space[s]];
//          contractions.push_back(skeleton);
//          cout << "  " << osi->label(s) << " ->";
//          for (auto p : skeleton) {
//            cout << " (" << p.first << "," << p.second << ")";
//          }
//          cout << endl;
//        }
//        contract_pair_permute(A, B, contractions);
//      });
//}

// void WTheorem::contract_pair_permute(
//    const WSymbolicTerm &A, const WSymbolicTerm &B,
//    const std::vector<std::vector<std::pair<int, int>>> &contractions) {

//  // lay out the legs and perform all permutations for each separate space
//  // this will include duplicate contractions that need to be eliminated

//  std::vector<std::vector<std::vector<int>>> A_perms;
//  std::vector<std::vector<std::vector<int>>> B_perms;
//  for (int s = 0; s < osi->num_spaces(); s++) {
//    //    cout << "  " << osi->label(s) << endl;
//    const auto &space_contr = contractions[s];
//    std::vector<std::vector<int>> A_perms_space;
//    std::vector<std::vector<int>> A_perms_space_unique;
//    std::vector<std::vector<int>> B_perms_space;
//    int naops = A.noperators(s);
//    int nbops = B.noperators(s);
//    std::vector<int> A_perm(naops, 0);
//    std::vector<int> B_perm(nbops, 0);

//    if (naops > 0) {
//      int offa = 0;
//      int ncontr = 1;
//      for (const auto &contr : space_contr) {
//        for (int i = 0; i < contr.first; i++) {
//          A_perm[offa + i] = ncontr;
//        }
//        offa += contr.first;
//        ncontr++;
//      }
//      std::sort(A_perm.begin(), A_perm.end());

//      do {
//        //        PRINT_ELEMENTS(A_perm, "A perm -> ",true);
//        A_perms_space.push_back(A_perm);
//      } while (std::next_permutation(A_perm.begin(), A_perm.end()));

//      std::vector<std::vector<bitset<64>>> A_perms_bitset_rep;
//      for (const auto &Ap : A_perms_space) {
//        std::vector<bitset<64>> vec_bitset(space_contr.size(), bitset<64>(0));

//        // form a bit representation of a contraction
//        for (int i = 0; i < naops; i++) {
//          int contr = Ap[i] - 1;
//          if (contr >= 0) {
//            vec_bitset[contr].set(i);
//          }
//        }

//        std::sort(vec_bitset.begin(), vec_bitset.end(),
//                  [](const bitset<64> &lhs, const bitset<64> &rhs) {
//                    return lhs.to_ullong() < rhs.to_ullong();
//                  });

//        if (std::find(A_perms_bitset_rep.begin(), A_perms_bitset_rep.end(),
//                      vec_bitset) == A_perms_bitset_rep.end()) {
//          A_perms_space_unique.push_back(Ap);
//          A_perms_bitset_rep.push_back(vec_bitset);
//          //          PRINT_ELEMENTS(Ap, "A perm -> ",true);
//        } else {
//          //          PRINT_ELEMENTS(Ap, "A perm -> ",false);
//          //          cout << " non unique" << endl;
//        }
//      }
//    } else {
//      A_perms_space_unique.push_back(A_perm);
//    }

//    if (nbops > 0) {
//      int offb = 0;
//      int ncontr = 1;
//      for (const auto &contr : space_contr) {
//        for (int i = 0; i < contr.second; i++) {
//          B_perm[offb + i] = ncontr;
//        }
//        offb += contr.second;
//        ncontr++;
//      }
//      std::sort(B_perm.begin(), B_perm.end());

//      do {
//        //        PRINT_ELEMENTS(B_perm, "B perm -> ");
//        B_perms_space.push_back(B_perm);
//      } while (std::next_permutation(B_perm.begin(), B_perm.end()));
//    } else {
//      B_perms_space.push_back(B_perm);
//    }

//    A_perms.push_back(A_perms_space_unique);
//    B_perms.push_back(B_perms_space);
//  }

//  // combine contractions for each orbital space
//  std::vector<int> range;
//  for (const auto &space_perms : A_perms) {
//    range.push_back(space_perms.size());
//  }
//  for (const auto &space_perms : B_perms) {
//    range.push_back(space_perms.size());
//  }

//  int n = 0;
//  product_space_iterator(range, [&](const std::vector<int> &product) {
//    PRINT(Summary, cout << "    Layout of legs " << n << endl;)
//    std::vector<std::vector<int>> A_legs;
//    std::vector<std::vector<int>> B_legs;
//    for (int s = 0; s < osi->num_spaces(); s++) {
//      A_legs.push_back(A_perms[s][product[s]]);
//      B_legs.push_back(B_perms[s][product[osi->num_spaces() + s]]);
//      cout << "    " << osi->label(s) << " -> ";
//      PRINT_ELEMENTS(A_perms[s][product[s]]);
//      PRINT_ELEMENTS(B_perms[s][product[osi->num_spaces() + s]], " ");
//      cout << endl;
//    }
//    this->contract_pair_execute(A, B, A_legs, B_legs);
//    cout << endl;
//    n++;
//  });

//  // execute the contraction
//}

// void WTheorem::contract_pair_execute(
//    const WSymbolicTerm &A, const WSymbolicTerm &B, const
//    std::vector<std::vector<int>> &A_legs, const std::vector<std::vector<int>>
//    &B_legs) {

//  // relabel indices to avoid redundant terms
//  // find the indices that are summed
//  std::map<Index, int> Aidx_count;
//  std::vector<Index> Aidx = A.indices();
//  std::vector<Index> Aidx_un, Aidx_sum;
//  for (const Index idx : Aidx) {
//    Aidx_count[idx] += 1;
//  }
//  for (const auto &kv : Aidx_count) {
//    if (kv.second == 1) {
//      Aidx_un.push_back(kv.first);
//    } else {
//      Aidx_sum.push_back(kv.first);
//    }
//  }
//  std::sort(Aidx_un.begin(), Aidx_un.end());
//  std::sort(Aidx_sum.begin(), Aidx_sum.end());

//  std::map<Index, int> Bidx_count;
//  std::vector<Index> Bidx = B.indices();
//  std::vector<Index> Bidx_un, Bidx_sum;
//  for (const Index idx : Bidx) {
//    Bidx_count[idx] += 1;
//  }
//  for (const auto &kv : Bidx_count) {
//    if (kv.second == 1) {
//      Bidx_un.push_back(kv.first);
//    } else {
//      Bidx_sum.push_back(kv.first);
//    }
//  }
//  std::sort(Bidx_un.begin(), Bidx_un.end());
//  std::sort(Bidx_sum.begin(), Bidx_sum.end());

//  // find the common non-summed indices of A and B
//  std::vector<Index> ABidx_un;
//  std::set_intersection(Aidx_un.begin(), Aidx_un.end(), Bidx_un.begin(),
//                        Bidx_un.end(), std::back_inserter(ABidx_un));

//  // check that A and B do not have common non-summed indices
//  assert(ABidx_un.size() == 0);

//  // find the reindexing map for the repeated indices of B
//  index_map_t B_idx_map = remap(Aidx_sum, Bidx_sum);

//  //  PRINT_ELEMENTS(Aidx);
//  //  PRINT_ELEMENTS(Bidx);

//  cout << "\n    Contractions:" << endl;

//  // merge the two tensors
//  WSymbolicTerm Amod = A;
//  WSymbolicTerm Bmod = B;

//  // reindex B
//  Bmod.reindex(B_idx_map);

//  cout << "    " << Amod.str() << endl;
//  cout << "    " << Bmod.str() << endl;

//  std::vector<std::vector<SQOperator>> Aops = Amod.operators();
//  std::vector<std::vector<SQOperator>> Bops = Bmod.operators();
//  std::vector<std::vector<SQOperator>> Aops_mod, Bops_mod;

//  std::vector<Tensor> Atens = Amod.tensors();
//  std::vector<Tensor> Btens = Bmod.tensors();

//  std::vector<Tensor> ABtens;
//  // insert the tensors
//  ABtens.insert(ABtens.end(), Atens.begin(), Atens.end());
//  ABtens.insert(ABtens.end(), Btens.begin(), Btens.end());

//  scalar_t ABfact = Amod.factor() * Bmod.factor();

//  // contract indices and keep track of signs
//  bool valid = true;
//  for (int s = 0; s < osi->num_spaces(); s++) {
//    // get the edges for all contractions
//    auto legs = contraction_layout_to_edges(A_legs[s], B_legs[s]);

//    // differentiate between various types of spaces
//    RDMType dmstruc = osi->dmstructure(s);

//    // Create density matrices and cumulants
//    for (const auto &c : legs) {
//      int order = c.first.size() + c.second.size();
//      std::vector<Index> upper_idx;
//      std::vector<Index> lower_idx;
//      // pair contractions
//      if (order == 2) {
//        // pairwise contraction with Kronecker delta
//        if (dmstruc == RDMType::Delta) {

//        }
//        // pairwise contraction with density matrix
//        else {
//          const SQOperator &left_op = Aops[s][c.first[0]];
//          const SQOperator &right_op = Bops[s][c.second[0]];
//          if ((left_op.type() == Creation) and
//              (right_op.type() == Annihilation)) {
//            upper_idx.push_back(left_op.index());
//            lower_idx.push_back(right_op.index());
//            // add particle density matrix
//            Tensor gamma("Gamma", upper_idx, lower_idx);
//            ABtens.push_back(gamma);
//          } else if ((left_op.type() == Annihilation) and
//                     (right_op.type() == Creation)) {
//            upper_idx.push_back(left_op.index());
//            lower_idx.push_back(right_op.index());
//            // add hole density matrix
//            Tensor eta("Eta", upper_idx, lower_idx);
//            ABtens.push_back(eta);
//          } else {
//            valid = false;
//          }
//        }
//      } else {
//        // cumulants
//        for (int a : c.first) {
//        }
//        for (int b : c.second) {
//        }
//      }
//    }

//    // Remove contracted operators
//    std::vector<bool> Aelim(Aops[s].size(), false);
//    std::vector<bool> Belim(Bops[s].size(), false);
//    for (const auto &c : legs) {
//      for (int a : c.first) {
//        Aelim[a] = true;
//      }
//      for (int b : c.second) {
//        Belim[b] = true;
//      }
//    }
//    std::vector<SQOperator> Aops_mod_space;
//    int a = 0;
//    for (const SQOperator op : Aops[s]) {
//      if (not Aelim[a]) {
//        Aops_mod_space.push_back(op);
//      }
//    }
//    std::vector<SQOperator> Bops_mod_space;
//    int b = 0;
//    for (const SQOperator op : Bops[s]) {
//      if (not Belim[a]) {
//        Bops_mod_space.push_back(op);
//      }
//    }
//    Aops_mod.push_back(Aops_mod_space);
//    Bops_mod.push_back(Bops_mod_space);
//  }

//  if (valid) {
//    WSymbolicTerm AB;
//    for (auto const &t : ABtens) {
//      AB.add(t);
//    }
//    for (auto const &ops : Aops_mod) {
//      for (auto const &op : ops) {
//        AB.add(op);
//      }
//    }
//    for (auto const &ops : Bops_mod) {
//      for (auto const &op : ops) {
//        AB.add(op);
//      }
//    }
//    cout << "\n    Result:\n    " << AB.str();
//  }
//}

// std::vector<std::pair<std::vector<int>, std::vector<int>>>
// WTheorem::contraction_layout_to_edges(const std::vector<int> &A_legs,
//                                      const std::vector<int> &B_legs) {
//  std::vector<std::pair<std::vector<int>, std::vector<int>>> legs;
//  if ((A_legs.size() == 0) or (B_legs.size() == 0))
//    return legs;
//  // find the number of contractions
//  int maxa = 0;
//  for (int a : A_legs) {
//    maxa = std::max(a, maxa);
//  }
//  int maxb = 0;
//  for (int b : B_legs) {
//    maxb = std::max(b, maxb);
//  }
//  assert(maxa == maxb);

//  legs.resize(maxa);

//  for (int i = 0, maxi = A_legs.size(); i < maxi; ++i) {
//    int a = A_legs[i];
//    if (a > 0) {
//      legs[a - 1].first.push_back(i);
//    }
//  }
//  for (int i = 0, maxi = B_legs.size(); i < maxi; ++i) {
//    int b = B_legs[i];
//    if (b > 0) {
//      legs[b - 1].second.push_back(i);
//    }
//  }
//  return legs;
//}

// void WTheorem::make_contraction_skeletons() {
//  // the null contraction
//  std::tuple<int, int, int> key(0, 0, 0);
//  skeletons_[key].push_back({std::make_pair(0, 0)});

//  for (int n = 0; n <= maxskeleton_; ++n) {
//    PRINT(Detailed, cout << "\nNumber of contractions: " << n << endl;)
//    // Partition the number of contractions (n)
//    const auto &partitions = integer_partitions_[n];

//    PRINT(Detailed, cout << "Type of " << n << "-contractions:" << endl;)
//    for (const auto &partition : partitions) {
//      PRINT(
//          Detailed, std::vector<std::string> s; for (const auto &i
//                                                     : partition) {
//            s.push_back(to_string(i));
//          } cout << "  "
//                 << to_string(s, "+");)
//      PRINT(Detailed, cout << " | ";)

//      std::map<int, int> leg_count;
//      int highest_cumulant = 0;
//      for (int i : partition) {
//        leg_count[i] += 1;
//        highest_cumulant = std::max(highest_cumulant, i);
//      }

//      std::vector<int> nlegs;
//      std::vector<int> ntypes;
//      std::vector<int> multp;
//      std::vector<std::string> s;

//      for (auto kv : leg_count) {
//        PRINT(Detailed, s.push_back(to_string(kv.first * 2) + "-leg x " +
//                                    to_string(kv.second)););
//        nlegs.push_back(2 * kv.first);
//        ntypes.push_back(2 * kv.first - 1);
//        multp.push_back(kv.second);
//      }
//      PRINT(Detailed, cout << to_string(s, " + "););

//      PRINT(Detailed, cout << " (highest cumulant: " << highest_cumulant <<
//      ")"
//                           << endl;);

//      if (highest_cumulant > maxcumulant_)
//        continue;
//      auto sets = generalized_combinations_with_repetitions(ntypes, multp);
//      for (auto &set : sets) {
//        std::vector<std::pair<int, int>> contraction_skeleton;
//        int nlegs_left_total = 0;
//        int nlegs_right_total = 0;

//        PRINT(Detailed, cout << "    ";)
//        for (int n = 0; n < nlegs.size(); n++) {
//          for (int i : set[n]) {
//            int nlegs_left = i + 1;
//            int nlegs_right = nlegs[n] - i - 1;
//            PRINT(Detailed, cout << nlegs[n] << "-leg(" << nlegs_left << "|"
//                                 << nlegs_right << ")  ";)
//            nlegs_left_total += nlegs_left;
//            nlegs_right_total += nlegs_right;
//            contraction_skeleton.push_back(
//                std::make_pair(nlegs_left, nlegs_right));
//          }
//        }
//        PRINT(Detailed, cout << " -> (" << nlegs_left_total << "|"
//                             << nlegs_right_total << ")" << endl;)
//        // save this skeleton
//        std::tuple<int, int, int> key(2 * n, nlegs_left_total,
//                                      nlegs_right_total);
//        skeletons_[key].push_back(contraction_skeleton);
//      }
//      PRINT(Detailed, cout << endl;)
//    }
//  }

//  std::vector<std::tuple<int, int, int>> sorted_keys;
//  for (auto &kv : skeletons_) {
//    sorted_keys.push_back(kv.first);
//  }
//  std::sort(sorted_keys.begin(), sorted_keys.end());

//  PRINT(Detailed, for (auto &k
//                       : sorted_keys) {
//    cout << "Total legs:" << std::get<0>(k) << " (" << std::get<1>(k) << "|"
//         << std::get<2>(k) << ")" << endl;
//    const auto &skeleton_familty = skeletons_[k];
//    for (auto &el : skeleton_familty) {
//      std::vector<std::string> str;
//      for (auto p : el) {
//        str.push_back("(" + to_string(p.first) + "|" + to_string(p.second) +
//                      ")");
//      }
//      cout << to_string(str, " ");
//      cout << endl;
//    }
//    cout << endl;
//  })
//}

//// std::vector<std::map<std::pair<int, int>, int>>
//// WTheorem::make_contraction_skeletons(const std::vector<int>
//// &contraction_list) {
////  std::vector<std::map<std::pair<int, int>, int>> result;
////  // Classify the type of partitionings of contractions
////  // E.g. a 2-contraction can be writtens as a 1+3-legs,2+2-legs,3+1-legs
////  //  std::map<int,std::vector<std::pair<int,int>>>

////  std::map<int, int> count;
////  for (int i : contraction_list) {
////    count[i] += 1;
////  }

////  return result;
////}

////  product_space_iterator({2,3,2}, [&](const std::vector<int> &vec) {
////    for (int i : vec) {
////      cout << i << ' ';
////    }
////    cout << endl;
////  });

////  product_space_iterator({2,4,2}, [&](const std::vector<int> &vec) {
////    for (int i : vec) {
////      cout << i << ' ';
////    }
////    cout << endl;
////  });

////  generalized_combinations_with_repetitions_iterator({2, 2, 2}, {2, 2, 2},
////                             [&](const std::vector<std::vector<int>> &res) {
////                               for (auto &vec : res) {
////                                 for (int i : vec) {
////                                   cout << i << ' ';
////                                 }
////                                 cout << endl;
////                               }
////                               cout << endl;
////                             });

////  generalized_combinations_with_repetitions_iterator({2, 3, 3}, {2, 2, 2},
////                             [&](const std::vector<std::vector<int>> &res) {
////                               for (auto &vec : res) {
////                                 for (int i : vec) {
////                                   cout << i << ' ';
////                                 }
////                                 cout << endl;
////                               }
////                               cout << endl;
////                             });
