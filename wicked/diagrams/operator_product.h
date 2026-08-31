#pragma once

#include "helpers/product.hpp"
#include "helpers/helpers.h"
#include "operator.h"
#include "wicked-def.h"

class OperatorProduct : public Product<Operator> {
public:
  /// Constructors
  OperatorProduct() : Product<Operator>() {}
  OperatorProduct(Product<Operator> &&opprod) : Product<Operator>(opprod) {}
  OperatorProduct(const std::vector<Operator> &operators)
      : Product<Operator>(operators) {}
  OperatorProduct(std::initializer_list<Operator> operators)
      : Product<Operator>(operators) {}

  scalar_t canonicalize();

  int num_ops() const;

  std::string str() const;
};

OperatorProduct operator*(const OperatorProduct &l, const OperatorProduct &r);
