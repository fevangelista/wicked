#ifndef _wicked_diag_theorem_h_
#define _wicked_diag_theorem_h_

#include <vector>
#include <string>

class WSQOperator;
class WTensor;
class WTerm;
class WDiagOperator;
class WDiagVertex;

enum class WDiagPrint { No, Summary, Detailed, All };

/// A class to contract a product of operators
class WDiagTheorem {

public:
  /// Constructor
  WDiagTheorem();

  /// Contract a product of operators
  void contract(scalar_t factor, const std::vector<WDiagOperator> &ops);

private:
  std::vector<std::vector<int>> contractions_;
  std::vector<std::vector<WDiagVertex>> elementary_contractions_;

  /// Generates all elementary contractions
  std::vector<std::vector<WDiagVertex>>
  generate_elementary_contractions(const std::vector<WDiagOperator> &ops);

  /// Backtracking algorithm used to generate all contractions product of
  /// elementary contractions
  void generate_contractions(
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
  WTerm evaluate_contraction(const std::vector<WDiagOperator> &ops,
                             const std::vector<int> &contraction,
                             scalar_t factor);

  /// Return the combinatorial factor corresponding to a contraction pattern
  scalar_t combinatorial_factor(const std::vector<WDiagOperator> &ops,
                                const std::vector<int> &contraction);

  //  /// Return the sign corresponding to a contraction pattern
  //  int contraction_sign(const std::vector<WDiagOperator> &ops,
  //                       const std::vector<int> &contraction);

  //  /// Return the factor corresponding to a contraction pattern
  //  scalar_t contraction_factor(const std::vector<WDiagOperator> &ops,
  //                              const std::vector<int> &contraction);

  //  /// Return the tensors corresponding to a contraction pattern
  //  WTerm
  //  contraction_tensors(const std::vector<WDiagOperator> &ops,
  //                      const std::vector<int> &contraction);

  //  /// Return the second quantized operators corresponding to a contraction
  //  /// pattern
  //  std::vector<WSQOperator>
  //  contraction_sqoperators(const std::vector<WDiagOperator> &ops,
  //                          const std::vector<int> &contraction);

  /// The number of contractions found
  int ncontractions_ = 0;
  /// The largest allowed cumulant
  int maxcumulant_ = 100;

  WDiagPrint print_ = WDiagPrint::Summary;
};

#endif // _wicked_diag_theorem_h_
