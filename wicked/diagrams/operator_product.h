#ifndef _wicked_operator_product_h_
#define _wicked_operator_product_h_

#include "helpers/product.hpp"
#include "wicked-def.h"

class Operator;

class OperatorProduct : public Product<Operator> {
public:
  /// Constructors
  OperatorProduct() : Product<Operator>() {}
  OperatorProduct(const std::vector<Operator> &operators)
      : Product<Operator>(operators) {}
  OperatorProduct(std::initializer_list<Operator> operators)
      : Product<Operator>(operators) {}

  scalar_t canonicalize();

  int num_ops() const;
};

OperatorProduct operator*(const OperatorProduct &l, const OperatorProduct &r);

#endif // _wicked_operator_product_h_
