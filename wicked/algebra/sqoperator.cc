#include "sqoperator.h"
#include "../helpers/product.hpp"
#include "helpers/combinatorics.h"
#include "helpers/helpers.h"
#include "index.h"
#include "wicked-def.h"

SQOperator::SQOperator(SQOperatorType type, Index index)
    : operator_(std::make_pair(type, index)) {}

SQOperator::~SQOperator() {}

SQOperatorType SQOperator::type() const { return operator_.first; }

FieldType SQOperator::field_type() const {
  return orbital_subspaces->field_type(space());
}

const Index &SQOperator::index() const { return operator_.second; }

int SQOperator::space() const { return operator_.second.space(); }

SpaceType SQOperator::space_type() const {
  return orbital_subspaces->space_type(space());
}

bool SQOperator::is_creation() const {
  return type() == SQOperatorType::Creation;
}

bool SQOperator::is_quasiparticle_creation() const {
  return space_type() == SpaceType::Occupied ? not is_creation()
                                             : is_creation();
}

std::string SQOperator::op_symbol() const {
  return orbital_subspaces->op_symbol(space());
}

void SQOperator::reindex(index_map_t &idx_map) {
  if (idx_map.count(operator_.second) > 0) {
    operator_.second = idx_map[operator_.second];
  }
}

bool SQOperator::operator<(SQOperator const &other) const {
  // Here we compare the operators in the following order:
  // 1. creation operators go to the left of annihilation operators
  if (is_creation() != other.is_creation()) {
    return is_creation();
  }
  // 2. if both operators are creation or annihilation operators, then we
  // compare the space and the index. The space is compared in the reverse order
  // for annihilation operators.
  if (is_creation()) {
    return index() < other.index();
  }
  return other.index() < index();
}

bool SQOperator::normal_ordered_less(SQOperator const &other) const {
  const auto space_type = orbital_subspaces->space_type(space());
  // Here we compare the operators in the following order:
  // 1. quasi-particle creation operators go to the left of quasi-particle
  // annihilation operators
  if (is_quasiparticle_creation() != other.is_quasiparticle_creation()) {
    return is_quasiparticle_creation();
  }
  // 2. if both operators are quasi-particle creation or annihilation operators,
  // then we compare the space and the index. The space is compared in the
  // reverse order for quasi-particle annihilation operators.
  if (is_quasiparticle_creation()) {
    return index() < other.index();
  }
  return other.index() < index();
}

bool SQOperator::operator==(SQOperator const &other) const {
  return operator_ == other.operator_;
}

std::string SQOperator::str() const {
  std::string s = op_symbol();
  s += (is_creation() ? "+" : "-");
  s += "(" + index().str() + ")";
  return s;
}

std::string SQOperator::latex() const {
  std::string s("\\hat{" + op_symbol() + "}");
  s += (is_creation() ? "^" : "_");
  s += "{" + index().latex() + "}";
  return s;
}

SQOperator SQOperator::adjoint() const {
  SQOperatorType type =
      (is_creation() ? SQOperatorType::Annihilation : SQOperatorType::Creation);
  return SQOperator(type, index());
}

scalar_t SQOperator::commutator_factor(const SQOperator &other) const {
  // if both operators are fermionic, return -1
  if (field_type() == FieldType::Fermion and
      other.field_type() == FieldType::Fermion) {
    return -scalar_t(1);
  }
  // if both operators are bosonic, return +1
  if (field_type() == FieldType::Boson and
      other.field_type() == FieldType::Boson) {
    return scalar_t(1);
  }
  // if one operator is fermionic and the other is bosonic, return 1
  return scalar_t(1);
}

std::string SQOperator::compile(const std::string &format) const {
  throw std::runtime_error(
      "SQOperator::compile() - Operation does not make sense");
  return "";
}

SQOperator make_sqoperator(const std::string &index, SQOperatorType type) {
  return SQOperator(type, make_index_from_str(index));
}

std::ostream &operator<<(std::ostream &os, const SQOperator &op) {
  os << op.str();
  return os;
}

scalar_t canonicalize_sqops(Product<SQOperator> &sqops, bool reversed) {
  std::vector<std::pair<SQOperator, int>> sqop_pos;
  for (const auto &[n, sqop] : enumerate(sqops.vec())) {
    sqop_pos.push_back(std::make_pair(sqop, n));
  }
  // when reversed == false, we sort in increasing order
  // when reversed == true, we sort in decreasing order
  if (reversed) {
    std::sort(sqop_pos.rbegin(), sqop_pos.rend());
  } else {
    std::sort(sqop_pos.begin(), sqop_pos.end());
  }
  std::vector<int> perm;
  sqops.clear();
  for (const auto &[sqop, pos] : sqop_pos) {
    sqops.push_back(sqop);
    perm.push_back(pos);
  }
  return permutation_sign(perm);
}
