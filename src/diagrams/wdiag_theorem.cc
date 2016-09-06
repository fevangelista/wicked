#include <iostream>

#include "stl_utils.hpp"
#include "orbital_space.h"
#include "combinatorics.h"
#include "wdiag_operator.h"
#include "wdiag_theorem.h"
#include "wterm.h"

#define PRINT(detail, code)                                                    \
  if (print_ >= detail) {                                                      \
    code                                                                       \
  }

using namespace std;

WDiagTheorem::WDiagTheorem() {}

void WDiagTheorem::contract(scalar_t factor,
                            const std::vector<WDiagOperator> &ops) {

  ncontractions_ = 0;
  contractions_.clear();
  elementary_contractions_.clear();

  std::cout << "\nContracting the following operators:" << std::endl;
  for (auto &op : ops) {
    std::cout << "    " << op;
  }

  elementary_contractions_ = generate_elementary_contractions(ops);

  std::vector<int> a(50, -1);
  // create a vector of
  std::vector<WDiagVertex> free_vertex_vec;
  for (const auto &op : ops) {
    free_vertex_vec.push_back(op.vertex());
  }

  std::cout << "\nPossible contractions:" << std::endl;

  generate_contractions(a, 0, elementary_contractions_, free_vertex_vec);

  std::cout << "\nTotal:" << ncontractions_ << std::endl;

  int ops_rank = operators_rank(ops);
  for (const auto &contraction : contractions_) {
    std::vector<std::vector<WDiagVertex>> vertices;
    int contr_rank = 0;
    for (int c : contraction) {
      contr_rank += vertices_rank(elementary_contractions_[c]);
    }
    cout << "  " << ops_rank - contr_rank << endl;

    WTerm term = evaluate_contraction(ops, contraction, factor);

    cout << term << endl;
  }
}

void WDiagTheorem::generate_contractions(
    std::vector<int> a, int k,
    const std::vector<std::vector<WDiagVertex>> &el_contr_vec,
    std::vector<WDiagVertex> &free_vertex_vec) {

  process_contraction(a, k, free_vertex_vec);

  k = k + 1;
  std::vector<int> candidates =
      construct_candidates(a, k, el_contr_vec, free_vertex_vec);

  for (const auto &c : candidates) {
    a[k - 1] = c;
    make_move(a, k, el_contr_vec, free_vertex_vec);
    generate_contractions(a, k, el_contr_vec, free_vertex_vec);
    unmake_move(a, k, el_contr_vec, free_vertex_vec);
  }
}

std::vector<int> WDiagTheorem::construct_candidates(
    std::vector<int> &a, int k,
    const std::vector<std::vector<WDiagVertex>> &el_contr_vec,
    const std::vector<WDiagVertex> &free_vertex_vec) {

  std::vector<int> candidates;
  int nops = free_vertex_vec.size();

  // determine the last elementary contraction used
  int minc = (k > 1) ? a[k - 2] : 0;
  int maxc = el_contr_vec.size();

  // loop over all potentially viable contractions
  for (int c = minc; c < maxc; c++) {
    const auto &el_contr = el_contr_vec[c];

    // test if this contraction is compatible, that is if the number of
    // operators we want to contract is less than or equal to the number of free
    // (uncontracted) operators
    bool compatible = true;
    for (int A = 0; A < nops; A++) {
      for (int s = 0; s < osi->num_spaces(); s++) {
        if (free_vertex_vec[A].cre(s) < el_contr[A].cre(s)) {
          compatible = false;
        }
        if (free_vertex_vec[A].ann(s) < el_contr[A].ann(s)) {
          compatible = false;
        }
      }
    }
    if (compatible) {
      candidates.push_back(c);
    }
  }
  return candidates;
}

void WDiagTheorem::make_move(
    const std::vector<int> &a, int k,
    const std::vector<std::vector<WDiagVertex>> &el_contr_vec,
    std::vector<WDiagVertex> &free_vertex_vec) {
  int nops = free_vertex_vec.size();

  // remove the current elementary contraction
  int c = a[k - 1];
  const auto &el_contr = el_contr_vec[c];

  for (int A = 0; A < nops; A++) {
    for (int s = 0; s < osi->num_spaces(); s++) {
      int ncre = free_vertex_vec[A].cre(s) - el_contr[A].cre(s);
      free_vertex_vec[A].cre(s, ncre);
      int nann = free_vertex_vec[A].ann(s) - el_contr[A].ann(s);
      free_vertex_vec[A].ann(s, nann);
    }
  }
}

void WDiagTheorem::unmake_move(
    const std::vector<int> &a, int k,
    const std::vector<std::vector<WDiagVertex>> &el_contr_vec,
    std::vector<WDiagVertex> &free_vertex_vec) {
  int nops = free_vertex_vec.size();

  // remove the current elementary contraction
  int c = a[k - 1];
  const auto &el_contr = el_contr_vec[c];

  for (int A = 0; A < nops; A++) {
    for (int s = 0; s < osi->num_spaces(); s++) {
      int ncre = free_vertex_vec[A].cre(s) + el_contr[A].cre(s);
      free_vertex_vec[A].cre(s, ncre);
      int nann = free_vertex_vec[A].ann(s) + el_contr[A].ann(s);
      free_vertex_vec[A].ann(s, nann);
    }
  }
}

void WDiagTheorem::process_contraction(
    const std::vector<int> &a, int k,
    std::vector<WDiagVertex> &free_vertex_vec) {
  contractions_.push_back(std::vector<int>(a.begin(), a.begin() + k));
  cout << " " << ncontractions_ << ":";
  for (int i = 0; i < k; ++i) {
    cout << " " << a[i];
  }

  WDiagVertex free_ops;
  for (const auto &free_vertex : free_vertex_vec) {
    free_ops += free_vertex;
  }

  cout << " " << free_ops << " rank = " << free_ops.rank();
  cout << endl;

  ncontractions_++;
}

std::vector<std::vector<WDiagVertex>>
WDiagTheorem::generate_elementary_contractions(
    const std::vector<WDiagOperator> &ops) {

  int nops = ops.size();

  // holds all the contractions
  std::vector<std::vector<WDiagVertex>> contr_vec;

  // loop over orbital spaces
  for (int s = 0; s < osi->num_spaces(); s++) {
    std::cout << "\n  => Basic contractions for space " << s << std::endl;

    // differentiate between various types of spaces
    DMStructure dmstruc = osi->dmstructure(s);

    // Pairwise contractions creation-annihilation:
    // _____
    // |   |
    // a^+ a

    cout << "\n    * c/a pairwise contractions" << endl;
    // loop over the creation operators of each operator
    for (int c = 0; c < nops; c++) {
      // loop over the annihilation operators of each operator (right to the
      // creation operators)
      for (int a = c + 1; a < nops; a++) {
        // check if contraction is viable
        if ((ops[c].num_cre(s) > 0) and (ops[a].num_ann(s) > 0)) {
          std::vector<WDiagVertex> new_contr(nops);
          new_contr[c].cre(s, 1);
          new_contr[a].ann(s, 1);
          contr_vec.push_back(new_contr);

          PRINT_ELEMENTS(new_contr, "      ");
          cout << endl;
        }
      }
    }

    // Pairwise contractions creation-annihilation:
    // _____
    // |   |
    // a   a^+

    cout << "\n    * a/c pairwise contractions" << endl;
    // loop over the creation operators of each operator
    for (int a = 0; a < nops; a++) {
      // loop over the annihilation operators of each operator (right to the
      // creation operators)
      for (int c = a + 1; c < nops; c++) {
        // check if contraction is viable
        if ((ops[c].num_cre(s) > 0) and (ops[a].num_ann(s) > 0)) {
          std::vector<WDiagVertex> new_contr(nops);
          new_contr[c].cre(s, 1);
          new_contr[a].ann(s, 1);
          contr_vec.push_back(new_contr);

          PRINT_ELEMENTS(new_contr, "      ");
          cout << endl;
        }
      }
    }

    // 2k-legged contractions (k >= 2) of k creation and k annihilation
    // operators:
    // _____________
    // |   |   |   |
    // a^+ a   a   a^+

    if (dmstruc == DMStructure::General) {

      // compute the largest possible cumulant
      int sumcre = 0;
      int sumann = 0;
      for (int A = 0; A < nops; A++) {
        sumcre += ops[A].num_cre(s);
        sumann += ops[A].num_ann(s);
      }
      int max_half_legs = std::min(std::min(sumcre, sumann), maxcumulant_);
      int max_legs = 2 * max_half_legs;

      // loop over all possible contractions from 4 to max_legs
      for (int half_legs = 2; half_legs <= max_half_legs; half_legs++) {
        cout << "\n    * " << 2 * half_legs << "-legs cumulant contractions"
             << endl;
        auto half_legs_part = integer_partitions(half_legs);

        // create lists of leg partitionings among all operators that are
        // compatible with the number of creation and annihilation operators
        std::vector<std::vector<int>> cre_legs_vec, ann_legs_vec;
        for (const auto part : half_legs_part) {
          if (part.size() <= nops) {
            std::vector<int> perm(nops, 0);
            std::copy(part.begin(), part.end(), perm.begin());
            std::sort(perm.begin(), perm.end());
            do {
              // check if compatible with creation/annihilation operators
              bool cre_compatible = true;
              bool ann_compatible = true;
              for (int A = 0; A < nops; A++) {
                if (ops[A].num_cre(s) < perm[A]) {
                  cre_compatible = false;
                }
                if (ops[A].num_ann(s) < perm[A]) {
                  ann_compatible = false;
                }
              }
              if (cre_compatible) {
                cre_legs_vec.push_back(perm);
              }
              if (ann_compatible) {
                ann_legs_vec.push_back(perm);
              }
            } while (std::next_permutation(perm.begin(), perm.end()));
          }
        }

        // combine the creation and annihilation operators
        for (const auto cre_legs : cre_legs_vec) {
          for (const auto ann_legs : ann_legs_vec) {
            std::vector<WDiagVertex> new_contr(nops);
            int ncontracted = 0;
            for (int A = 0; A < nops; A++) {
              new_contr[A].cre(s, cre_legs[A]);
              new_contr[A].ann(s, ann_legs[A]);
              // count number of operators contracted
              if (cre_legs[A] + ann_legs[A] > 0) {
                ncontracted += 1;
              }
            }
            // exclude operators that have legs only on one operator
            if (ncontracted > 1) {
              contr_vec.push_back(new_contr);
              PRINT_ELEMENTS(new_contr, "      ");
              cout << endl;
            }
          }
        }
      }
    }
  }
  return contr_vec;
}

void print_key(std::tuple<int, int, bool, int> key, int n);

void print_key(std::tuple<int, int, bool, int> key, int n) {
  cout << "key[vertex = " << std::get<0>(key)
       << ", space = " << std::get<1>(key)
       << ", creation = " << std::get<2>(key) << ", num = " << std::get<3>(key)
       << "] -> " << n << endl;
}

WTerm WDiagTheorem::evaluate_contraction(const std::vector<WDiagOperator> &ops,
                                         const std::vector<int> &contraction,
                                         scalar_t factor) {
  std::vector<WTensor> tensors;
  std::vector<WSQOperator> sqops;

  std::vector<int> index_counter(osi->num_spaces(), 0);

  // create the tensors corresponding to the operators, lay out the operators on
  // a vector, and create mappings

  // this map takes the operator index (op), orbital space (s), the sqop type,
  // and an index and maps it to the operators as they are stored in a vector
  std::map<std::tuple<int, int, bool, int>, int> op_map;
  //                   op space cre    n

  int n = 0;
  for (int o = 0; o < ops.size(); o++) {
    const auto &op = ops[o];
    std::vector<WIndex> lower;
    std::vector<WIndex> upper;
    for (int s = 0; s < osi->num_spaces(); s++) {
      for (int c = 0; c < op.num_cre(s); c++) {
        WIndex idx(s, index_counter[s]);
        lower.push_back(idx);
        sqops.push_back(WSQOperator(Creation, idx));
        auto key = std::make_tuple(o, s, true, c);
        op_map[key] = n;
        PRINT(WDiagPrint::All, print_key(key, n););
        index_counter[s] += 1;
        n += 1;
      }
    }
    // the annihilation operators will be layed out as normal and then reversed
    std::vector<WSQOperator> sqops_ann;
    for (int s = 0; s < osi->num_spaces(); s++) {
      for (int a = 0; a < op.num_ann(s); a++) {
        WIndex idx(s, index_counter[s]);
        upper.push_back(idx);
        sqops_ann.push_back(WSQOperator(Annihilation, idx));
        auto key = std::make_tuple(o, s, false, op.num_ann(s) - a - 1);
        op_map[key] = n;
        PRINT(WDiagPrint::All, print_key(key, n););
        index_counter[s] += 1;
        n += 1;
      }
    }
    std::reverse(sqops_ann.begin(), sqops_ann.end());
    sqops.insert(sqops.end(), sqops_ann.begin(), sqops_ann.end());
    tensors.push_back(WTensor(op.label(), lower, upper));
  }

  // now apply the contractions.  this means contracting operators, creating
  // density matrices and cumulant tensors

  // stores the offset for each uncontracted operator
  std::vector<WDiagVertex> ops_offset(ops.size());

  cout << "\n  Applying contraction" << endl;
  for (int c : contraction) {
    const auto &vertex_vec = elementary_contractions_[c];
    PRINT_ELEMENTS(vertex_vec);
  }
  cout << endl;

  // this vector will keep track of the which operators have been assigned an
  // order
  //    std::vector<bool> is_op_ordered_flag(ops.size(), false);
  std::vector<int> sign_order(sqops.size(), -1);

  int sorted_position = 0;
  int nsqops_contracted = 0;

  std::vector<std::vector<bool>> bit_map_vec;
  // a sign factor due to the contractions with the hole-density matrix
  int eta_sign = 1;
  for (int c : contraction) {
    std::vector<bool> bit_map(sqops.size(), false);
    const auto &vertex_vec = elementary_contractions_[c];
    int rank = vertices_rank(vertex_vec);
    nsqops_contracted += rank;
    std::string label;
    if (rank == 2) {
      // find the last operator
      bool is_right_op_annihilation = true;
      for (const WDiagVertex &vertex : vertex_vec) {
        for (int s = 0; s < osi->num_spaces(); s++) {
          if (vertex.cre(s) > 0)
            is_right_op_annihilation = false;
          if (vertex.ann(s) > 0)
            is_right_op_annihilation = true;
        }
      }
      // if the operator contracted on the right is an annihilation op then this
      // is a one-particle density matrix (gamma)
      if (is_right_op_annihilation) {
        label = "Gamma";
      } else {
        label = "Eta";
        eta_sign *= -1;
      }
    } else {
      label = "Lambda" + std::to_string(rank);
    }

    std::vector<WIndex> lower;
    std::vector<WIndex> upper;

    // collect all the creators
    for (int v = 0; v < vertex_vec.size(); v++) {
      const WDiagVertex &vertex = vertex_vec[v];
      for (int s = 0; s < osi->num_spaces(); s++) {
        int ncre = vertex.cre(s);
        // assign the creation indices
        int cre_off = ops_offset[v].cre(s);
        for (int c = 0; c < ncre; c++) {
          // find the operator corresponding to this leg
          auto key = std::make_tuple(v, s, true, cre_off + c);
          if (op_map.count(key) == 0) {
            PRINT(WDiagPrint::All, print_key(key, -1););
            cout << " NOT FOUND!!!" << endl;
            exit(1);
          } else {
            int sqop_pos = op_map[key];
            PRINT(WDiagPrint::All, print_key(key, sqop_pos););
            upper.push_back(sqops[sqop_pos].index());
            // assign a position to this operator
            sign_order[sqop_pos] = sorted_position;
            bit_map[sqop_pos] = true;
            sorted_position += 1;
          }
        }
        // update the creator's offset
        ops_offset[v].cre(s, cre_off + ncre);
      }
    }

    for (int v = 0; v < vertex_vec.size(); v++) {
      const WDiagVertex &vertex = vertex_vec[v];
      for (int s = 0; s < osi->num_spaces(); s++) {
        int nann = vertex.ann(s);
        // assign the annihilation indices
        int ann_off = ops_offset[v].ann(s);
        for (int a = 0; a < nann; a++) {
          // find the operator corresponding to this leg
          auto key = std::make_tuple(
              v, s, false,
              ann_off + nann - 1 - a); // start from the rightmost operator
          if (op_map.count(key) == 0) {
            PRINT(WDiagPrint::All, print_key(key, -1););
            cout << " NOT FOUND!!!" << endl;
            exit(1);
          } else {
            int sqop_pos = op_map[key];
            PRINT(WDiagPrint::All, print_key(key, sqop_pos););
            lower.push_back(sqops[sqop_pos].index());
            // assign a position to this operator
            sign_order[sqop_pos] = sorted_position;
            bit_map[sqop_pos] = true;
            sorted_position += 1;
          }
        }
        // update the annihilation's offset
        ops_offset[v].ann(s, ann_off + nann);
      }
    }
    std::reverse(lower.begin(), lower.end());
    tensors.push_back(WTensor(label, lower, upper));
    bit_map_vec.push_back(bit_map);
  }

  for (const auto &bit_map : bit_map_vec) {
    int ntrue = std::count(bit_map.begin(), bit_map.end(), true);
    bool line = false;
    cout << "";
    for (bool b : bit_map) {
      if (b) {
        line = true;
        ntrue -= 1;
      }
      if (line) {
        cout << "___";
      } else {
        cout << "   ";
      }
      if (ntrue == 0)
        line = false;
    }
    cout << endl;
    for (bool b : bit_map) {
      cout << (b ? " | " : "   ");
    }
    cout << endl;
  }
  for (const auto &sqop : sqops) {
    cout << ((sqop.type() == Creation) ? " + " : " - ");
  }
  cout << endl;
  for (const auto &sqop : sqops) {
    cout << " " << sqop.index();
  }
  cout << endl;

  for (int s = 0; s < osi->num_spaces(); s++) {
    for (int i = 0; i < sqops.size(); i++) {
      if ((sign_order[i] == -1) and (sqops[i].index().space() == s) and
          (sqops[i].type() == Creation)) {
        sign_order[i] = sorted_position;
        sorted_position += 1;
      }
    }
  }
  for (int s = 0; s < osi->num_spaces(); s++) {
    for (int i = 0; i < sqops.size(); i++) {
      if ((sign_order[i] == -1) and (sqops[i].index().space() == s) and
          (sqops[i].type() == Annihilation)) {
        sign_order[i] = sorted_position;
        sorted_position += 1;
      }
    }
  }

  for (int order : sign_order) {
    cout << " " << order << " ";
  }
  cout << endl;

  int sign = eta_sign * permutation_sign(sign_order);

  PRINT(WDiagPrint::All,  PRINT_ELEMENTS(sign_order, "\n  positions: "););

  std::vector<std::pair<int, WSQOperator>> sorted_sqops;
  sorted_position = 0;
  for (const auto &sqop : sqops) {
    sorted_sqops.push_back(std::make_pair(sign_order[sorted_position], sqop));
    sorted_position += 1;
  }
  std::sort(sorted_sqops.begin(), sorted_sqops.end());
  sqops.clear();

  for (int i = nsqops_contracted; i < sorted_sqops.size(); ++i) {
    sqops.push_back(sorted_sqops[i].second);
  }

  // find the combinatorial factor associated with this contraction
  scalar_t comb_factor = combinatorial_factor(ops, contraction);

  WTerm term;

  for (const auto &tensor : tensors) {
    term.add(tensor);
  }
  for (const auto &sqop : sqops) {
    term.add(sqop);
  }
  for (const auto &op : ops) {
    factor *= op.factor();
  }
  term.set_factor(sign * factor * comb_factor);

  cout << "  sign = " << sign << endl;
  cout << "  factor = " << factor << endl;
  cout << "  combinatorial_factor = " << comb_factor << endl;

  return term;
}

scalar_t
WDiagTheorem::combinatorial_factor(const std::vector<WDiagOperator> &ops,
                                   const std::vector<int> &contraction) {

  scalar_t factor = 1;

  // stores the offset for each uncontracted operator
  std::vector<WDiagVertex> free_vertices;
  for (const auto &op : ops) {
    free_vertices.push_back(op.vertex());
  }

  // for each contraction find the combinatorial factor
  for (int c : contraction) {
    const auto &vertex_vec = elementary_contractions_[c];
    for (int v = 0; v < vertex_vec.size(); v++) {
      const WDiagVertex &vertex = vertex_vec[v];
      for (int s = 0; s < osi->num_spaces(); s++) {
        int kcre = vertex.cre(s);
        int kann = vertex.ann(s);
        int ncre = free_vertices[v].cre(s);
        int nann = free_vertices[v].ann(s);
        factor *= binomial(ncre, kcre);
        factor *= binomial(nann, kann);
        free_vertices[v].cre(s, ncre - kcre);
        free_vertices[v].ann(s, nann - kann);
      }
    }
  }

  std::map<int, int> contraction_count;
  for (int c : contraction) {
    contraction_count[c] += 1;
  }
  for (const auto &kv : contraction_count) {
    factor /= binomial(kv.second, 1);
  }

  return factor;
}

// std::vector<WSQOperator>
// WDiagTheorem::contraction_sqoperators(const std::vector<WDiagOperator> &ops,
//                                      const std::vector<int> &contraction) {
//  std::vector<WSQOperator> sqops;
//  return sqops;
//}
