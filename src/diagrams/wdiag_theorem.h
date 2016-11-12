#ifndef _wicked_diag_theorem_h_
#define _wicked_diag_theorem_h_

#include <string>
#include <vector>

class WSQOperator;
class WTensor;
class WAlgebraicTerm;
class WDiagOperator;
class WDiagOperatorSum;
class WDiagVertex;
class WSum;

enum class WDiagPrint { No, Basic, Summary, Detailed, All };

/// A class to contract a product of operators
class WDiagTheorem {

public:
  /// Constructor
  WDiagTheorem();

  /// Contract a product of operators
  WSum contract(scalar_t factor, const std::vector<WDiagOperator> &ops,
                int minrank, int maxrank);

  /// Contract a product of sums of operators
  WSum contract_sum(scalar_t factor, const WDiagOperatorSum &dop_sum,
                    int minrank, int maxrank);

  void set_print(WDiagPrint print);

private:
  std::vector<std::vector<int>> contractions_;
  std::vector<std::vector<WDiagVertex>> elementary_contractions_;

  // Create a canonical
  std::pair<std::vector<WDiagOperator>, std::vector<std::vector<WDiagVertex>>>
  canonicalize_contraction(const std::vector<WDiagOperator> &ops,
                           const std::vector<int> &contraction_vec);

  void compare_contraction_perm(
      const std::vector<WDiagOperator> &ops,
      const std::vector<std::vector<WDiagVertex>> &contractions,
      const std::vector<int> &ops_perm, const std::vector<int> &contr_perm,
      std::vector<int> &best_ops_perm, std::vector<int> &best_contr_perm);

  /// Generates all elementary contractions
  std::vector<std::vector<WDiagVertex>>
  generate_elementary_contractions(const std::vector<WDiagOperator> &ops);

  /// Generates all composite contractions
  void generate_composite_contractions(const std::vector<WDiagOperator> &ops);

  // ==> Backtracking routines <==

  /// Backtracking algorithm used to generate all contractions product of
  /// elementary contractions
  void generate_contractions_backtrack(
      std::vector<int> a, int k,
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

  /// Apply the contraction to this set of operators and produce a term
  std::pair<WAlgebraicTerm, scalar_t> evaluate_contraction(
      const std::vector<WDiagOperator> &ops,
      const std::vector<std::vector<WDiagVertex>> &contractions,
      scalar_t factor);

  /// Return the tensors and operators correspoding to a product of operators
  std::tuple<std::vector<WTensor>, std::vector<WSQOperator>,
             std::map<std::tuple<int, int, bool, int>, int>>
  contration_tensors_sqops(const std::vector<WDiagOperator> &ops);

  std::vector<int>
  vertex_vec_to_pos(const std::vector<WDiagVertex> &vertex_vec,
                    std::vector<WDiagVertex> &ops_offset,
                    std::map<std::tuple<int, int, bool, int>, int> &op_map,
                    bool creation);

  /// Return the combinatorial factor corresponding to a contraction pattern
  scalar_t combinatorial_factor(
      const std::vector<WDiagOperator> &ops,
      const std::vector<std::vector<WDiagVertex>> &contractions);

  /// The number of contractions found
  int ncontractions_ = 0;
  /// The largest allowed cumulant
  int maxcumulant_ = 100;

  WDiagPrint print_ = WDiagPrint::No;
};

#endif // _wicked_diag_theorem_h_
