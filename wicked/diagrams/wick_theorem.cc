#include <iostream>

#include "contraction.h"
#include "helpers/timer.hpp"
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

void WickTheorem::set_single_threaded(bool val) { single_threaded_ = val; }

const std::map<std::string, double> &WickTheorem::timers() const {
  return timers_;
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
  timer t1;
  elementary_contractions_ = generate_elementary_contractions(ops);
  timers_["step 1"] += t1.get();

  // Step 2. Generate allowed composite contractions
  timer t2;
  generate_composite_contractions(ops, minrank, maxrank);
  timers_["step 2"] += t2.get();

  // Step 3. Process contractions
  timer t3;
  Expression result =
      single_threaded_
          ? process_contractions(factor, ops, minrank, maxrank)
          : process_contractions_threads(factor, ops, minrank, maxrank);
  timers_["step 3"] += t3.get();
  return result;
}

Expression WickTheorem::contract(scalar_t factor,
                                 const OperatorExpression &expr,
                                 const int minrank, const int maxrank) {
  Expression result;
  for (const auto &[ops, f] : expr.terms()) {
    result += contract(factor * f, ops, minrank, maxrank);
  }
  return result;
}
