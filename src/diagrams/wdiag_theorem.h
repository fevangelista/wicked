#ifndef _wicked_diag_theorem_h_
#define _wicked_diag_theorem_h_

#include <vector>
#include <string>

class WDiagOperator;
class WDiagVertex;

/// A class to contract a product of operators
class WDiagTheorem {

public:
  /// Constructor
  WDiagTheorem();

  /// Contract a product of operators
  void contract(double factor, const std::vector<WDiagOperator> &ops);

private:
  std::vector<std::vector<int>> contractions_;
  std::vector<std::vector<WDiagVertex>> elementary_contractions_;

  /// Generates all elementary contractions
  std::vector<std::vector<WDiagVertex>>
  generate_elementary_contractions(const std::vector<WDiagOperator> &ops);

  /// Backtracking algorithm used to generate all contractions product of
  /// elementary contractions
  void generate_contractions(std::vector<int> a, int k,
      const std::vector<std::vector<WDiagVertex>> &el_contr_vec,
      std::vector<WDiagVertex> &free_vertex_vec);

  /// Return a vector of indices of elementary contractions that can be added to
  /// the current solution. Used in backtracking algorithm
  std::vector<int> construct_candidates(
      std::vector<int> &a, int k,
      const std::vector<std::vector<WDiagVertex>> &el_contr_vec,
      const std::vector<WDiagVertex> &free_vertex_vec);

  /// Applies a contraction to the list of free vertices. Used in backtracking
  /// algorithm
  void make_move(const std::vector<int> &a, int k,
                 const std::vector<std::vector<WDiagVertex>> &el_contr_vec,
                 std::vector<WDiagVertex> &free_vertex_vec);

  /// Undoes the application of a contraction to the list of free vertices. Used
  /// in backtracking algorithm
  void unmake_move(const std::vector<int> &a, int k,
                   const std::vector<std::vector<WDiagVertex>> &el_contr_vec,
                   std::vector<WDiagVertex> &free_vertex_vec);

  /// Process a contraction found by the backtracking algorithm
  void process_contraction(const std::vector<int> &a, int k,
                           std::vector<WDiagVertex> &free_vertex_vec);

  /// The number of contractions found
  int ncontractions_ = 0;
  /// The largest allowed cumulant
  int maxcumulant_ = 100;
};

#endif // _wicked_diag_theorem_h_
