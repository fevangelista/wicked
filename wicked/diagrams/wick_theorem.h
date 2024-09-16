#pragma once

#include <optional>
#include <string>
#include <vector>

class SQOperator;
class Tensor;
class SymbolicTerm;
class Operator;
class OperatorProduct;
class OperatorExpression;
class GraphMatrix;
class ElementaryContraction;
class CompositeContraction;

#include "../algebra/expression.h"

enum class PrintLevel { None, Basic, Summary, Detailed, All };

/// A class to contract a product of operators
class WickTheorem {

public:
  /// Constructor
  WickTheorem();

  /// Contract a product of operators
  Expression contract(scalar_t factor, const OperatorProduct &ops,
                      const int minrank, const int maxrank);

  /// Contract a product of sums of operators
  Expression contract(scalar_t factor, const OperatorExpression &expr,
                      const int minrank, const int maxrank);

  /// Set the amount of printing
  void set_print(PrintLevel print);

  /// Turn on/off graph canonicalization
  void do_canonicalize_graph(bool val);

  /// Set the maximum cumulant level
  void set_max_cumulant(int val);

  /// Set the single-threaded mode
  void set_single_threaded(bool val);

  const std::map<std::string, double> &timers() const;

private:
  /// A vector of elementary contractions
  std::vector<ElementaryContraction> elementary_contractions_;

  /// The allowed contractions stored as a vector of indices of elementary
  /// contractions
  std::vector<std::vector<int>> contractions_;

  std::map<std::string, double> timers_;

  /// The number of contractions found
  int ncontractions_ = 0;

  /// The largest allowed cumulant
  int maxcumulant_ = 100;

  /// Turn on/off graph canonicalization
  bool do_canonicalize_graph_ = true;

  /// The default print level
  PrintLevel print_ = PrintLevel::None;

  bool single_threaded_ = false;

  //
  // Functions for step 1. of the Wick's theorem algorithm
  // implemented in wich_theorem_elementary_contractions.cc
  //

  /// Generates all elementary contractions
  std::vector<ElementaryContraction>
  generate_elementary_contractions(const OperatorProduct &ops);

  /// Generates elementary contractions of occupied spaces
  void elementary_contractions_occupied(
      const OperatorProduct &ops, int s,
      std::vector<ElementaryContraction> &contr_vec);

  /// Generates elementary contractions of occupied spaces
  void elementary_contractions_unoccupied(
      const OperatorProduct &ops, int s,
      std::vector<ElementaryContraction> &contr_vec);

  /// Generates elementary contractions of general spaces
  void elementary_contractions_general(
      const OperatorProduct &ops, int s,
      std::vector<ElementaryContraction> &contr_vec);

  //
  // Functions for step 2. of the Wick's theorem algorithm
  // implemented in wich_theorem_composite_contractions.cc
  //

  /// Generates all composite contractions for a given contraction
  /// pattern stored in ops
  void generate_composite_contractions(const OperatorProduct &ops,
                                       const int minrank, const int maxrank);

  /// Backtracking algorithm used to generate all contractions product of
  /// elementary contractions
  void generate_contractions_backtrack(
      std::vector<int> a, int k,
      const std::vector<ElementaryContraction> &el_contr_vec,
      std::vector<GraphMatrix> &free_graph_matrix_vec, const int minrank,
      const int maxrank);

  /// Process a contraction (store it) found by the backtracking algorithm
  void
  process_contraction(const std::vector<int> &a, int k,
                      const std::vector<GraphMatrix> &free_graph_matrix_vec,
                      const int minrank, const int maxrank);

  /// Return a vector of indices of elementary contractions that can be
  /// added to the current backtracking solution. All candidates generated
  /// here lead to valid contractions
  std::vector<int>
  construct_candidates(std::vector<int> &a, int k,
                       const std::vector<ElementaryContraction> &el_contr_vec,
                       const std::vector<GraphMatrix> &free_graph_matrix_vec);

  /// Applies a contraction to the list of free graph matrices. Used in
  /// backtracking algorithm
  void make_move(std::vector<int> &a, int k, int c,
                 const std::vector<ElementaryContraction> &el_contr_vec,
                 std::vector<GraphMatrix> &free_graph_matrix_vec);

  /// Undoes the application of a contraction to the list of free graph
  /// matrices. Used in backtracking algorithm
  void unmake_move(std::vector<int> &a, int k, int c,
                   const std::vector<ElementaryContraction> &el_contr_vec,
                   std::vector<GraphMatrix> &free_graph_matrix_vec);

  //
  // Functions for step 3. of the Wick's theorem algorithm
  // implemented in wich_theorem_process_contractions.cc
  //

  /// Process the contractions generated in step 2.
  Expression process_contractions(scalar_t factor, const OperatorProduct &ops,
                                  const int minrank, const int maxrank);

  /// Process the contractions generated in step 2. using threads
  Expression process_contractions_threads(scalar_t factor,
                                          const OperatorProduct &ops,
                                          const int minrank, const int maxrank);

  Expression process_single_contraction(const OperatorProduct &ops,
                                        const std::vector<int> &contraction_vec,
                                        const int minrank, const int maxrank,
                                        const scalar_t factor,
                                        const int ops_rank);

  /// Apply the contraction to this set of operators and produce a term
  std::pair<SymbolicTerm, scalar_t>
  evaluate_contraction(const OperatorProduct &ops,
                       const CompositeContraction &contractions,
                       scalar_t factor);

  /// Return the tensors and operators correspoding to a product of operators
  std::tuple<std::vector<Tensor>, std::vector<SQOperator>,
             std::map<std::tuple<int, int, bool, int>, int>>
  contraction_tensors_sqops(const OperatorProduct &ops);

  std::vector<int>
  elements_vec_to_pos(const ElementaryContraction &elements_vec,
                      std::vector<GraphMatrix> &ops_offset,
                      std::map<std::tuple<int, int, bool, int>, int> &op_map,
                      bool creation);

  /// Return the combinatorial factor corresponding to a contraction pattern
  scalar_t combinatorial_factor(const OperatorProduct &ops,
                                const CompositeContraction &contractions);

  // Create a canonical contraction graph
  std::tuple<OperatorProduct, CompositeContraction, scalar_t>
  canonicalize_contraction_graph(const OperatorProduct &ops,
                                 const CompositeContraction &contractions);
};
