#include "wicked-def.h"
#include "helpers.h"
#include "wterm.h"

WTerm::WTerm() {}

void WTerm::add(WSQOperator &op) { operators_.push_back(op); }

void WTerm::add(WTensor &tensor_) { tensors_.push_back(tensor_); }

scalar_t WTerm::factor() const { return factor_; }

void WTerm::set_factor(scalar_t value) { factor_ = value; }

std::vector<int> WTerm::noperators_per_space() const {
  std::vector<int> counter(osi->num_spaces());
  for (const auto &op : operators_) {
    counter[op.index().space()] += 1;
  }
  return counter;
}

void WTerm::canonicalize() {
  // 1. Sort the tensors according to a score function
  using score_t =
      std::tuple<std::string, int, std::vector<int>, std::vector<int>, WTensor>;
  std::vector<score_t> scores;

  int n = 0;
  for (const auto &tensor : tensors_) {
    // a) label
    const std::string &label = tensor.label();

    // b) rank
    int rank = tensor.rank();

    // c) number of indices per space
    std::vector<int> num_low = num_indices_per_space(tensor.lower());
    std::vector<int> num_upp = num_indices_per_space(tensor.lower());

    scores.push_back(std::make_tuple(label, rank, num_low, num_upp, tensor));
    n += 1;
  }
  std::sort(scores.begin(), scores.end());

  tensors_.clear();
  for (const auto &score : scores) {
    tensors_.push_back(std::get<4>(score));
  }

  // 2. Relabel indices of tensors and operators

  // 3. Sort operators according to canonical form
}

bool WTerm::operator<(const WTerm &other) const {
  if (tensors_ > other.tensors_) {
    return false;
  }
  if (tensors_ < other.tensors_) {
    return true;
  }
  return operators_ < other.operators_;
}

bool WTerm::operator==(const WTerm &other) const {
  return (tensors_ == other.tensors_) and (operators_ == other.operators_);
}

std::string WTerm::str() const {
  std::vector<std::string> str_vec;

  for (const WTensor &tensor : tensors_) {
    str_vec.push_back(tensor.str());
  }
  str_vec.push_back("{");
  for (const WSQOperator &q : operators_) {
    str_vec.push_back(q.str());
  }
  str_vec.push_back("}");

  return (to_string(str_vec, " "));
}

std::string WTerm::tensor_str() const {
  std::vector<std::string> str_vec;
  for (const WTensor &tensor : tensors_) {
    str_vec.push_back(tensor.str());
  }
  return (to_string(str_vec, " "));
}

std::string WTerm::operator_str() const {
  std::vector<std::string> str_vec;
  str_vec.push_back("{");
  for (const WSQOperator &q : operators_) {
    str_vec.push_back(q.str());
  }
  str_vec.push_back("}");
  return (to_string(str_vec, " "));
}

std::string WTerm::latex() const {
  std::vector<std::string> str_vec;
  for (const WTensor &tensor : tensors_) {
    str_vec.push_back(tensor.latex());
  }
  str_vec.push_back("\\{");
  for (const WSQOperator &q : operators_) {
    str_vec.push_back(q.latex());
  }
  str_vec.push_back("\\}");
  return (to_string(str_vec, " "));
}

WTerm make_operator(const std::string &label,
                    const std::vector<std::string> &cre,
                    const std::vector<std::string> &ann) {
  WTerm term;

  std::vector<WIndex> cre_ind = make_indices_from_space_labels(cre);
  std::vector<WIndex> ann_ind = make_indices_from_space_labels(ann);

  // Add the tensor
  WTensor tensor(label, cre_ind, ann_ind);
  term.add(tensor);

  // Add the creation operators
  for (const WIndex c : cre_ind) {
    WSQOperator sqop(Creation, c);
    term.add(sqop);
  }

  // Add the annihilation operators
  std::reverse(ann_ind.begin(), ann_ind.end()); // reverse the annihilation ops
  for (const WIndex a : ann_ind) {
    WSQOperator sqop(Annihilation, a);
    term.add(sqop);
  }
  return term;
}
