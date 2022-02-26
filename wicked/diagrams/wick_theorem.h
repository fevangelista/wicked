#ifndef _wicked_diag_theorem_h_
#define _wicked_diag_theorem_h_

#include <string>
#include <vector>

class SQOperator;
class Tensor;
class SymbolicTerm;
class DiagOperator;
class DiagOpExpression;
class DiagVertex;

#include "../algebra/expression.h"

enum class PrintLevel { No, Basic, Summary, Detailed, All };

/// A class to contract a product of operators
class WickTheorem {

public:
  /// Constructor
  WickTheorem();

  /// Contract a product of operators
  Expression contract(scalar_t factor, const std::vector<DiagOperator> &ops,
                      const int minrank, const int maxrank);

  /// Contract a product of sums of operators
  Expression contract(scalar_t factor, const DiagOpExpression &expr,
                      const int minrank, const int maxrank);

  /// Set the amount of printing
  void set_print(PrintLevel print);

  void set_max_cumulant(int val);

private:
  std::vector<std::vector<int>> contractions_;
  std::vector<std::vector<DiagVertex>> elementary_contractions_;

  // Create a canonical
  std::pair<std::vector<DiagOperator>, std::vector<std::vector<DiagVertex>>>
  canonicalize_contraction(const std::vector<DiagOperator> &ops,
                           const std::vector<int> &contraction_vec);

  void compare_contraction_perm(
      const std::vector<DiagOperator> &ops,
      const std::vector<std::vector<DiagVertex>> &contractions,
      const std::vector<int> &ops_perm, const std::vector<int> &contr_perm,
      std::vector<int> &best_ops_perm, std::vector<int> &best_contr_perm);

  /// Generates all elementary contractions
  std::vector<std::vector<DiagVertex>>
  generate_elementary_contractions(const std::vector<DiagOperator> &ops);

  /// Generates elementary contractions of occupied spaces
  void elementary_contractions_occupied(
      const std::vector<DiagOperator> &ops, int s,
      std::vector<std::vector<DiagVertex>> &contr_vec);

  /// Generates elementary contractions of occupied spaces
  void elementary_contractions_unoccupied(
      const std::vector<DiagOperator> &ops, int s,
      std::vector<std::vector<DiagVertex>> &contr_vec);

  void elementary_contractions_general(
      const std::vector<DiagOperator> &ops, int s,
      std::vector<std::vector<DiagVertex>> &contr_vec);

  /// Generates all composite contractions
  void generate_composite_contractions(const std::vector<DiagOperator> &ops,
                                       const int minrank, const int maxrank);

  /// Process the contractions
  Expression process_contractions(scalar_t factor,
                                  const std::vector<DiagOperator> &ops,
                                  const int minrank, const int maxrank);

  // ==> Backtracking routines <==

  /// Backtracking algorithm used to generate all contractions product of
  /// elementary contractions
  void generate_contractions_backtrack(
      std::vector<int> a, int k,
      const std::vector<std::vector<DiagVertex>> &el_contr_vec,
      std::vector<DiagVertex> &free_vertex_vec, const int minrank,
      const int maxrank);

  /// Return a vector of indices of elementary contractions that can be added to
  /// the current solution. Used in backtracking algorithm
  std::vector<int>
  construct_candidates(std::vector<int> &a, int k,
                       const std::vector<std::vector<DiagVertex>> &el_contr_vec,
                       const std::vector<DiagVertex> &free_vertex_vec);

  /// Applies a contraction to the list of free vertices. Used in backtracking
  /// algorithm
  void make_move(const std::vector<int> &a, int k,
                 const std::vector<std::vector<DiagVertex>> &el_contr_vec,
                 std::vector<DiagVertex> &free_vertex_vec);

  /// Undoes the application of a contraction to the list of free vertices. Used
  /// in backtracking algorithm
  void unmake_move(const std::vector<int> &a, int k,
                   const std::vector<std::vector<DiagVertex>> &el_contr_vec,
                   std::vector<DiagVertex> &free_vertex_vec);

  /// Process a contraction found by the backtracking algorithm
  void process_contraction(const std::vector<int> &a, int k,
                           const std::vector<DiagVertex> &free_vertex_vec,
                           const int minrank, const int maxrank);

  /// Apply the contraction to this set of operators and produce a term
  std::pair<SymbolicTerm, scalar_t>
  evaluate_contraction(const std::vector<DiagOperator> &ops,
                       const std::vector<std::vector<DiagVertex>> &contractions,
                       scalar_t factor);

  /// Return the tensors and operators correspoding to a product of operators
  std::tuple<std::vector<Tensor>, std::vector<SQOperator>,
             std::map<std::tuple<int, int, bool, int>, int>>
  contraction_tensors_sqops(const std::vector<DiagOperator> &ops);

  std::vector<int>
  vertex_vec_to_pos(const std::vector<DiagVertex> &vertex_vec,
                    std::vector<DiagVertex> &ops_offset,
                    std::map<std::tuple<int, int, bool, int>, int> &op_map,
                    bool creation);

  /// Return the combinatorial factor corresponding to a contraction pattern
  scalar_t combinatorial_factor(
      const std::vector<DiagOperator> &ops,
      const std::vector<std::vector<DiagVertex>> &contractions);

  /// The number of contractions found
  int ncontractions_ = 0;

  /// The largest allowed cumulant
  int maxcumulant_ = 100;

  PrintLevel print_ = PrintLevel::No;
};

#endif // _wicked_diag_theorem_h_
