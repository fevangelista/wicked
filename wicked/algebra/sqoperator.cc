#include "sqoperator.h"
#include "helpers/combinatorics.h"
#include "helpers/helpers.h"
#include "index.h"
#include "wicked-def.h"

SQOperator::SQOperator(SQOperatorType type, Index index)
    : operator_(std::make_pair(type, index)) {}

SQOperator::~SQOperator() {}

SQOperatorType SQOperator::type() const { return operator_.first; }

FieldType SQOperator::field_type() const { return osi->field_type(space()); }

Index SQOperator::index() const { return operator_.second; }

int SQOperator::space() const { return operator_.second.space(); }

bool SQOperator::is_creation() const {
  return type() == SQOperatorType::Creation;
}

std::string SQOperator::op_symbol() const { return osi->op_symbol(space()); }

void SQOperator::reindex(index_map_t &idx_map) {
  if (idx_map.count(operator_.second) > 0) {
    operator_.second = idx_map[operator_.second];
  }
}

bool SQOperator::operator<(SQOperator const &other) const {
  // first compare the type (annihilators come before creation operators)
  if (operator_.first < other.operator_.first) {
    return true;
  }
  if (operator_.first > other.operator_.first) {
    return false;
  }
  // same type
  // if creation, then just compare the index
  if (operator_.first == SQOperatorType::Creation) {
    return operator_.second < other.operator_.second;
  }
  // if annihilation, flip comparison
  return other.operator_.second < operator_.second;
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

scalar_t canonicalize_sqops(std::vector<SQOperator> &sqops, bool reversed) {
  std::vector<std::pair<SQOperator, int>> sqop_pos;
  for (const auto &[n, sqop] : enumerate(sqops)) {
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
