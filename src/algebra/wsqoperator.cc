#include "wicked-def.h"
#include "windex.h"
#include "wsqoperator.h"

WSQOperator::WSQOperator(SQOperatorType type, WIndex index)
    : operator_(std::make_pair(type, index)) {}

WSQOperator::~WSQOperator() {}

SQOperatorType WSQOperator::type() const { return operator_.first; }

WIndex WSQOperator::index() const { return operator_.second; }

void WSQOperator::reindex(index_map_t &idx_map) {
  if (idx_map.count(operator_.second) > 0) {
    operator_.second = idx_map[operator_.second];
  }
}

bool WSQOperator::operator<(WSQOperator const &other) const {
  return operator_ < other.operator_;
}

bool WSQOperator::operator==(WSQOperator const &other) const {
  return operator_ == other.operator_;
}

std::string WSQOperator::str() const {
  std::string str_("a");
  str_ += (type() == Creation ? "+" : "-");
  str_ += "(";
  str_ += index().str();
  str_ += ")";
  return (str_);
}

std::string WSQOperator::latex() const {
  std::string str_("\\hat{a}");
  str_ += (type() == Creation ? "^" : "_");
  str_ += "{";
  str_ += index().latex();
  str_ += "}";
  return (str_);
}

std::string WSQOperator::ambit() const {
  std::string str_("\\hat{a}");
  str_ += (type() == Creation ? "^" : "_");
  str_ += "{";
  str_ += index().ambit();
  str_ += "}";
  return (str_);
}

std::ostream &operator<<(std::ostream &os, const WSQOperator &op) {
  os << op.str();
  return os;
}
