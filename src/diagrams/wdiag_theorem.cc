#include <iostream>

#include "wdiag_operator.h"
#include "wcontract.h"
#include "wcontractiongraph.h"

WContract::WContract() {}

void WContract::contract(double factor, const std::vector<WDiagOperator> &ops) {

  std::cout << "Contracting the following operators" << std::endl;
  for (auto &op : ops) {
    std::cout << op.str() << std::endl;
  }

  /// Create the contraction graph
  WContractionGraph cg(factor,ops);

}
