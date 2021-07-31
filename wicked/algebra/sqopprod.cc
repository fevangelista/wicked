
#include "sqopprod.h"
#include "../helpers.h"
#include "../orbital_space.h"

SQOpProd::SQOpProd() {
  for (auto &b : cre_) {
    assert(b.none());
  }
  for (auto &b : ann_) {
    assert(b.none());
  }
}

SQOpProd::SQOpProd(const std::vector<Index> &cre,
                   const std::vector<Index> &ann) {
  for (auto &b : cre_) {
    assert(b.none());
  }
  for (auto &b : ann_) {
    assert(b.none());
  }
  for (const auto &c : cre) {
    const auto &space = c.space();
    const auto &pos = c.pos();
    cre_[space].set(pos, true);
  }
  for (const auto &a : ann) {
    const auto &space = a.space();
    const auto &pos = a.pos();
    ann_[space].set(pos, true);
  }
}

bool SQOpProd::operator<(SQOpProd const &other) const {
  for (int space = maxspaces - 1; space >= 0; space--) {
    if (cre_[space] < other.cre_[space]) {
      return true;
    }
    if (other.cre_[space] < cre_[space]) {
      return false;
    }
  }
  for (int space = maxspaces - 1; space >= 0; space--) {
    if (ann_[space] < other.ann_[space]) {
      return true;
    }
    if (other.ann_[space] < ann_[space]) {
      return false;
    }
  }
  return false;
}

bool SQOpProd::operator==(SQOpProd const &other) const {
  return (cre_ == other.cre_) and (ann_ == other.ann_);
}

void SQOpProd::citerate(const std::function<void(bool, const size_t &,
                                                 const size_t &)> &func) const {
  for (size_t space = 0; space < maxspaces; space++) {
    if (cre_[space].any()) {
      for (int i = 0, maxi = cre_.size(); i < maxi; i++) {
        if (cre_[space].test(i)) {
          func(true, space, i);
        }
      }
    }
  }
  for (size_t space = 0; space < maxspaces; space++) {
    if (ann_[space].any()) {
      // loop in reverse
      for (int i = ann_.size() - 1; i >= 0; i--) {
        if (ann_[space].test(i)) {
          func(false, space, i);
        }
      }
    }
  }
}

void SQOpProd::citerate_spaces(
    const std::function<void(bool, const size_t &, const bitarray &)> &func)
    const {
  for (size_t space = 0; space < maxspaces; space++) {
    func(true, space, cre_[space]);
  }
  for (size_t space = 0; space < maxspaces; space++) {
    func(true, space, ann_[space]);
  }
}

int SQOpProd::nops() const {
  int n = 0;
  citerate_spaces([&](bool cre, const size_t &space, const bitarray &b) {
    n += b.count();
  });
  return n;
}

std::string SQOpProd::str() const {
  std::vector<std::string> s;
  citerate([&](bool cre, const size_t &space, const size_t &i) {
    s.push_back(std::string(cre ? "a+" : "a-") + "(" + osi->label(space) +
                std::to_string(i) + ")");
  });
  return join(s, " ");
}

std::string SQOpProd::latex() const {
  std::vector<std::string> s;
  citerate([&](bool cre, const size_t &space, const size_t &i) {
    auto idx = Index(space, i);
    s.push_back(std::string(cre ? "\\hat{a}^\\dagger" : "\\hat{a}") + "_{" +
                idx.latex() + "}");
  });
  return join(s, " ");
}

std::ostream &operator<<(std::ostream &os, const SQOpProd &op) {
  os << op.str();
  return os;
}

SQOpProd make_sqopprod(const std::vector<Index> &cre,
                       const std::vector<Index> &ann) {
  return SQOpProd(cre, ann);
}

SQOpProd make_sqopprod(const std::vector<std::string> &cre_str,
                       const std::vector<std::string> &ann_str) {
  std::vector<Index> cre, ann;
  for (const auto &c : cre_str) {
    cre.push_back(make_index(c));
  }
  for (const auto &c : ann_str) {
    ann.push_back(make_index(c));
  }
  return SQOpProd(cre, ann);
}