#include "sqoperator.h"
#include "index.h"
#include "wicked-def.h"

std::map<SQOperatorStatistics, std::string> SQOperatorStatistics_to_str{
    {SQOperatorStatistics::Fermion, "a"}, {SQOperatorStatistics::Boson, "b"}};

SQOperator::SQOperator(SQOperatorType type, Index index,
                       SQOperatorStatistics statistics)
    : operator_(std::make_pair(type, index)), statistics_(statistics) {}

SQOperator::~SQOperator() {}

SQOperatorType SQOperator::type() const { return operator_.first; }

SQOperatorStatistics SQOperator::statistics() const { return statistics_; }

Index SQOperator::index() const { return operator_.second; }

bool SQOperator::is_creation() const {
  return type() == SQOperatorType::Creation;
}

std::string SQOperator::op_symbol() const {
  return SQOperatorStatistics_to_str[statistics_];
}

void SQOperator::reindex(index_map_t &idx_map) {
  if (idx_map.count(operator_.second) > 0) {
    operator_.second = idx_map[operator_.second];
  }
}

bool SQOperator::operator<(SQOperator const &other) const {
  // first compare the type
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

std::string SQOperator::ambit() const {
  throw std::runtime_error(
      "SQOperator::ambit() - Operation does not make sense");
  return "";
}

SQOperator make_sqoperator(const std::string &index, SQOperatorType type,
                           SQOperatorStatistics statistics) {
  return SQOperator(type, make_index(index), statistics);
}

std::ostream &operator<<(std::ostream &os, const SQOperator &op) {
  os << op.str();
  return os;
}
