#include <iostream>

#include "contraction.h"
#include "operator.h"
#include "operator_expression.h"

#include "wick_theorem.h"

#define PRINT(detail, code)                                                    \
  if (print_ >= detail) {                                                      \
    code                                                                       \
  }

using namespace std;

WickTheorem::WickTheorem() {}

void WickTheorem::set_print(PrintLevel print) { print_ = print; }

void WickTheorem::set_max_cumulant(int n) { maxcumulant_ = n; }

void WickTheorem::do_canonicalize_graph(bool val) {
  do_canonicalize_graph_ = val;
}

Expression WickTheorem::contract(scalar_t factor, const OperatorProduct &ops,
                                 const int minrank, const int maxrank) {
  ncontractions_ = 0;
  contractions_.clear();
  elementary_contractions_.clear();

  PRINT(
      PrintLevel::Summary, std::cout << "\nContracting the operators: ";
      for (auto &op
           : ops) { std::cout << " " << op; };
      std::cout << std::endl;)

  // Step 1. Generate elementary contractions
  elementary_contractions_ = generate_elementary_contractions(ops);

  // Step 2. Generate allowed composite contractions
  generate_composite_contractions(ops, minrank, maxrank);

  // Step 3. Process contractions
  Expression result = process_contractions(factor, ops, minrank, maxrank);
  return result;
}

Expression WickTheorem::contract(scalar_t factor, const DiagOpExpression &expr,
                                 const int minrank, const int maxrank) {
  Expression result;
  for (const auto &[ops, f] : expr.terms()) {
    result += contract(factor * f, ops, minrank, maxrank);
  }
  return result;
}
