#ifndef _wicked_expression_h_
#define _wicked_expression_h_

#include <map>
#include <vector>

#include "equation.h"
#include "index.h"
#include "term.h"
#include "wicked-def.h"

/// A class to represent a sum of SQ terms.
class Expression {
public:
  // ==> Constructor <==
  Expression();

  // ==> Class public interface <==

  /// Return a map term -> factor
  const std::map<SymbolicTerm, scalar_t> &terms() const { return terms_; }

  /// Add a term that can optionally be scaled
  void add(const Term &term);

  /// Add a term that can optionally be scaled
  void add(const SymbolicTerm &term, scalar_t coefficient = 1);

  /// Add a term that can optionally be scaled
  void add(const std::pair<SymbolicTerm, scalar_t> &term_factor,
           scalar_t scale = 1);

  /// Add a term that can optionally be scaled
  void add(const Expression &expr, scalar_t scale = 1);

  /// Canonicalize this sum
  Expression &canonicalize();

  /// Reindex this sum
  Expression &reindex(index_map_t &idx_map);

  /// Compare this expression to another one
  bool operator==(const Expression &other);

  /// Add an expression
  Expression &operator+=(const Expression &terms);

  /// Substract an expression
  Expression &operator-=(const Expression &terms);

  /// Return a string representation
  std::string str() const;

  /// Return a LaTeX representation
  std::string latex(const std::string &sep = " \\\\ \n") const;

  /// Convert this sum to a vector of many-body equations
  std::map<int, Expression> to_manybody_equation(const std::string &label);

  // /// Convert this sum to a vector of many-body expressions
  // Expression to_manybody_expression(const std::string &label);
  // #for (const auto &eq : sum.to_manybody_equation("r")) {
  // #val.add(eq.rhs(), eq.rhs_factor());
  // #}

private:
  // ==> Class private data <==

  std::map<SymbolicTerm, scalar_t> terms_;

  // ==> Class private functions <==
};

/// addition
Expression operator+(Expression lhs, const Expression &rhs);
/// subtraction
Expression operator-(Expression lhs, const Expression &rhs);

/// Print to an output stream
std::ostream &operator<<(std::ostream &os, const Expression &sum);

/// The syntax used to input a tensor expression
enum class TensorSyntax { Wicked, TCE };

///// Create a sum from a string
Expression string_to_sum(const std::string &s);

Expression make_operator_expr(const std::string &label,
                              const std::vector<std::string> &components,
                              bool normal_ordered,
                              scalar_t coefficient = scalar_t(1));

#endif // _wicked_expression_h_
