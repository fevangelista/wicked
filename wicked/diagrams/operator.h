#ifndef _wicked_diag_operator_h_
#define _wicked_diag_operator_h_

#include <string>
#include <vector>

#include "../product.hpp"
#include "vertex.h"
#include "wicked-def.h"

/// A class to represent operators
class Operator {

public:
  /// Constructor
  /// @param label: the label of the operator
  /// @param cre: the number of creation operators per orbital space
  /// @param ann: the number of annihilation operators per orbital space
  Operator(const std::string &label, const std::vector<int> &cre,
           const std::vector<int> &ann);

  /// Return the label of the operator
  const std::string &label() const;

  /// The vertex object
  Vertex vertex() const;

  /// One over the number of permutations of equivalent operators
  scalar_t factor() const;

  /// Return the number of creation operators in space
  int cre(int space) const;

  /// Return the number of annihilation operators in space
  int ann(int space) const;

  /// Return the number of creation + annilation operators represented by this
  /// vertex
  int num_ops() const;

  /// Comparison operator used for sorting
  bool operator<(Operator const &other) const;

  /// Return a string representation of the operator
  std::string str() const;

  /// Write a string representation of the operator to a stream
  friend std::ostream &operator<<(std::ostream &os, const Operator &op);

private:
  /// The label of the operator
  std::string label_;

  /// The number of creation/annihilation operators in each space
  Vertex vertex_;
};

bool do_operators_commute(const Operator &a, const Operator &b);

bool operator_noncommuting_less(const Operator &a, const Operator &b);

// Helper functions

/// Create an operator
Operator make_diag_operator(const std::string &label,
                            const std::vector<char> &cre_labels,
                            const std::vector<char> &ann_labels);

/// Return the particle rank of a vector of operators
int sum_num_ops(const std::vector<Operator> &ops);

#endif // _wicked_diag_operator_h_
