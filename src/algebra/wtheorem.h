#ifndef _wicked_wtheorem_h_
#define _wicked_wtheorem_h_

#include <vector>

#include "wterm.h"

enum WTPrintLevel { No, Summary, Detailed, Full };

// Contraction partitions are stored as vectors of pairs
using contr_part_t = std::vector<std::pair<int, int>>;

using skeleton_map_t = std::map<std::tuple<int, int, int>,
                                std::vector<std::vector<std::pair<int, int>>>>;

/// A class to apply Wick's theorem to a product of terms
class WTheorem {
public:
  // ==> Constructor <==

  WTheorem();

  // ==> Class public interface <==

  /// Apply Wick's theorem to a product of terms
  void theorem(const std::vector<WTerm> &terms);

  /// Wick's theorem for a pair of terms with resulting rank ranging from
  /// minrank to maxrank
  void theorem_pair(const WTerm &A, const WTerm &B, int minrank = -1,
                    int maxrank = -1);

  void contract_pair(const WTerm &A, const WTerm &B,
                     const std::vector<int> &contr_per_space);

  std::vector<std::map<std::pair<int, int>, int>>
  make_contraction_configurations();

  void set_print_level(WTPrintLevel print) { print_ = print; }

private:
  /// The largest allowed cumulant
  int maxcumulant_ = 20;
  int maxskeleton_ = 10;
  WTPrintLevel print_ = Full;
  std::vector<std::vector<std::vector<int>>> integer_partitions_;

  /// contraction_partitions_[n] stores a list of all the partitions of legs
  /// between two operators for a n-contraction (with 2n legs)
  ///
  /// Example: for a 2-contraction with 4 legs
  /// contraction_partitions_[2] = {(1,3),(2,2),(3,1)}
  ///
  /// Used for: to divide a contraction between two operators
  std::vector<contr_part_t> contraction_partitions_;

  skeleton_map_t skeletons_;

  // ==> Private class functions <==

  // Find all the ways we can split multi-leg contractions
  void make_contraction_partitions();

  void make_contraction_skeletons();
};

#endif // _wicked_wtheorem_h_
