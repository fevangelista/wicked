//#ifndef _wicked_theorem_h_
//#define _wicked_theorem_h_

//#include <vector>

//#include "wterm.h"

// enum WTPrintLevel { No, Summary, Detailed, Full };

// using skeleton_map_t = std::map<std::tuple<int, int, int>,
//                                std::vector<std::vector<std::pair<int,
//                                int>>>>;

///// A class to apply Wick's theorem to a product of terms
// class WTheorem {
// public:
//  // ==> Constructor <==

//  WTheorem();

//  // ==> Class public interface <==

//  /// Apply Wick's theorem to a product of terms
//  void theorem(const std::vector<WSymbolicTerm> &terms);

//  /// Wick's theorem for a pair of terms with resulting rank ranging from
//  /// minrank to maxrank
//  void theorem_pair(const WSymbolicTerm &A, const WSymbolicTerm &B, int
//  minrank = -1,
//                    int maxrank = -1);

//  void contract_pair(const WSymbolicTerm &A, const std::vector<int> &naop,
//                     const WSymbolicTerm &B, const std::vector<int> &nbop,
//                     const std::vector<int> &contr_per_space);

//  void contract_pair_splitting(
//      const WSymbolicTerm &A, const WSymbolicTerm &B,
//      const std::vector<std::tuple<int, int, int>> &splitting);

//  void contract_pair_permute(
//      const WSymbolicTerm &A, const WSymbolicTerm &B,
//      const std::vector<std::vector<std::pair<int, int>>> &contractions);

//  void contract_pair_execute(const WSymbolicTerm &A, const WSymbolicTerm &B,
//                             const std::vector<std::vector<int>> &A_legs,
//                             const std::vector<std::vector<int>> &B_legs);

//  void set_print_level(WTPrintLevel print) { print_ = print; }

// private:
//  /// The largest allowed cumulant
//  int maxcumulant_ = 2;
//  /// The maximum number of legs in a pairwise contraction
//  int maxskeleton_ = 4;
//  WTPrintLevel print_ = Summary;
//  std::vector<std::vector<std::vector<int>>> integer_partitions_;

//  skeleton_map_t skeletons_;

//  // ==> Private class functions <==

//  void make_contraction_skeletons();

//  std::vector<std::pair<std::vector<int>, std::vector<int>>>
//  contraction_layout_to_edges(const std::vector<int> &A_legs,
//                              const std::vector<int> &B_legs);
//};

//#endif // _wicked_theorem_h_
