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
  /// Generates all elementary contractions
  std::vector<std::vector<WDiagVertex>>
  generate_elementary_contractions(const std::vector<WDiagOperator> &ops);

  /// Generates all contractions product of elementary contractions
  void generate_contractions(
      std::vector<int> a, int k,
      const std::vector<std::vector<WDiagVertex>> &el_contr_vec,
      std::vector<WDiagVertex> &free_vertices);

  /// Return a vector of indices of elementary contractions that can be added to
  /// the current solution
  std::vector<int> construct_candidates(std::vector<int> &a, int k,
      const std::vector<std::vector<WDiagVertex>> &el_contr_vec,
      const std::vector<WDiagVertex> &free_vertices);

  void make_move(const std::vector<int> &a, int k,
                 const std::vector<std::vector<WDiagVertex>> &el_contr_vec,
                 std::vector<WDiagVertex> &free_vertices);

  void unmake_move(const std::vector<int> &a, int k,
                   const std::vector<std::vector<WDiagVertex>> &el_contr_vec,
                   std::vector<WDiagVertex> &free_vertices);

  /// The largest allowed cumulant
  int maxcumulant_ = 3;

  bool bt_finished_;
};

#endif // _wicked_diag_theorem_h_
