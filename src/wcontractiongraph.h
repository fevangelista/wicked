#ifndef _wicked_contraction_graph_h_
#define _wicked_contraction_graph_h_

#include "wicked-def.h"
#include "wdiag_operator.h"
#include "wconnectionmatrix.h"

/// A class to contract a product of operators
class WContractionGraph {

public:
  /// Construct a graph with no connections
  WContractionGraph(double factor, const std::vector<WDiagOperator> &ops);

private:
  double factor_;
  std::vector<WDiagOperator> ops_;
  std::vector<WDiagOperator> ref_;
  WConnectionMatrix mat_;
};

#endif // _wicked_contraction_graph_h_
