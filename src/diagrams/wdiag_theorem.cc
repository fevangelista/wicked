#include <algorithm>
#include <iostream>

#include "combinatorics.h"
#include "helpers.h"
#include "orbital_space.h"
#include "stl_utils.hpp"
#include "walgebraicterm.h"
#include "wdiag_operator.h"
#include "wdiag_operator_sum.h"
#include "wdiag_theorem.h"
#include "wsum.h"
#include "wtensor.h"
#include "wsqoperator.h"

#define PRINT(detail, code)                                                    \
  if (print_ >= detail) {                                                      \
    code                                                                       \
  }

void print_key(std::tuple<int, int, bool, int> key, int n);
void print_contraction(const std::vector<WDiagOperator> &ops,
                       const std::vector<WTensor> &tensors,
                       const std::vector<std::vector<bool>> &bit_map_vec,
                       const std::vector<WSQOperator> &sqops,
                       const std::vector<int> sign_order);

using namespace std;

WDiagTheorem::WDiagTheorem() {}

WSum WDiagTheorem::contract(scalar_t factor,
                            const std::vector<WDiagOperator> &ops, int minrank,
                            int maxrank) {

  WSum result;
  ncontractions_ = 0;
  contractions_.clear();
  elementary_contractions_.clear();

  PRINT(WDiagPrint::Summary,
        std::cout << "\nContracting the following operators:" << std::endl;
        for (auto &op
             : ops) { std::cout << "    " << op; })

  elementary_contractions_ = generate_elementary_contractions(ops);

  std::vector<int> a(100, -1);
  // create a vector of
  std::vector<WDiagVertex> free_vertex_vec;
  for (const auto &op : ops) {
    free_vertex_vec.push_back(op.vertex());
  }

  PRINT(WDiagPrint::Summary, std::cout << "\nPossible contractions:"
                                       << std::endl;)
  generate_contractions(a, 0, elementary_contractions_, free_vertex_vec);
  PRINT(WDiagPrint::Summary, std::cout << "\nTotal:" << ncontractions_
                                       << std::endl;)

  int ops_rank = operators_rank(ops);
  for (const auto &contraction : contractions_) {
    std::vector<std::vector<WDiagVertex>> vertices;
    int contr_rank = 0;
    for (int c : contraction) {
      contr_rank += vertices_rank(elementary_contractions_[c]);
    }
    int term_rank = ops_rank - contr_rank;

    if ((term_rank >= minrank) and (term_rank <= maxrank)) {
      PRINT(WDiagPrint::Basic, cout << "\n  Operator rank "
                                    << ops_rank - contr_rank << endl;)

      std::pair<WAlgebraicTerm, scalar_t> term_factor =
          evaluate_contraction(ops, contraction, factor);
      PRINT(WDiagPrint::Summary, cout << term_factor << endl;)

      WAlgebraicTerm &term = term_factor.first;
      PRINT(WDiagPrint::Basic, cout << term << endl;)
      scalar_t canonicalize_factor = term.canonicalize();
      result.add(
          std::make_pair(term, term_factor.second * canonicalize_factor));
    }
  }
  return result;
}

WSum WDiagTheorem::contract_sum(scalar_t factor, WDiagOperatorSum &dop_sum,
                                int minrank, int maxrank) {
  WSum result;

  for (const auto &dop_factor : dop_sum.sum()) {
    scalar_t this_factor = dop_factor.second;
    const std::vector<WDiagOperator> &ops = dop_factor.first;
    result += contract(factor * this_factor, ops, minrank, maxrank);
  }

  return result;
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

  PRINT(WDiagPrint::Summary, cout << " " << ncontractions_ << ":";
        for (int i = 0; i < k; ++i) { cout << " " << a[i]; })

  WDiagVertex free_ops;
  for (const auto &free_vertex : free_vertex_vec) {
    free_ops += free_vertex;
  }

  PRINT(WDiagPrint::Summary, cout << " " << free_ops
                                  << " rank = " << free_ops.rank() << endl;)

  ncontractions_++;
}

std::vector<std::vector<WDiagVertex>>
WDiagTheorem::generate_elementary_contractions(
    const std::vector<WDiagOperator> &ops) {

  int nops = ops.size();

  // a vector that holds all the contractions
  std::vector<std::vector<WDiagVertex>> contr_vec;

  // loop over orbital spaces
  for (int s = 0; s < osi->num_spaces(); s++) {
    PRINT(WDiagPrint::Summary, std::cout
                                   << "\n  => Basic contractions for space "
                                   << s << std::endl;)

    // differentiate between various types of spaces
    DMStructure dmstruc = osi->dmstructure(s);

    // Pairwise contractions creation-annihilation:
    // _____
    // |   |
    // a^+ a

    if (dmstruc == DMStructure::DoublyOccupied) {
      PRINT(WDiagPrint::Summary, cout << "\n    * c/a pairwise contractions"
                                      << endl;)
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

            PRINT(WDiagPrint::Summary, PRINT_ELEMENTS(new_contr, "      ");
                  cout << endl;)
          }
        }
      }
    }

    // Pairwise contractions creation-annihilation:
    // _____
    // |   |
    // a   a^+

    if (dmstruc == DMStructure::Unoccupied) {
      PRINT(WDiagPrint::Summary, cout << "\n    * a/c pairwise contractions"
                                      << endl;)
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

            PRINT(WDiagPrint::Summary, PRINT_ELEMENTS(new_contr, "      ");
                  cout << endl;)
          }
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

      // loop over all possible contractions from 2 to max_legs
      for (int half_legs = 1; half_legs <= max_half_legs; half_legs++) {
        PRINT(WDiagPrint::Summary, cout << "\n    * " << 2 * half_legs
                                        << "-legs contractions" << endl;)
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

std::pair<WAlgebraicTerm, scalar_t>
WDiagTheorem::evaluate_contraction(const std::vector<WDiagOperator> &ops,
                                   const std::vector<int> &contraction,
                                   scalar_t factor) {
  PRINT(WDiagPrint::Basic, cout << "\n----------------------------" << endl;)

  // 1. Create tensors, lay out the second quantized operators on a vector,
  // and create mappings
  auto tensors_sqops_op_map = contration_tensors_sqops(ops);
  std::vector<WTensor> &tensors = std::get<0>(tensors_sqops_op_map);
  std::vector<WSQOperator> &sqops = std::get<1>(tensors_sqops_op_map);
  // this map takes the operator index (op), orbital space (s), the sqop type,
  // and an index and maps it to the operators as they are stored in a vector
  //  std::map<std::tuple<int, int, bool, int>, int> op_map;
  //                   op space cre    n
  std::map<std::tuple<int, int, bool, int>, int> &op_map =
      std::get<2>(tensors_sqops_op_map);

  PRINT(
      WDiagPrint::Summary, cout << "  Contraction(s):" << endl;
      for (int c
           : contraction) {
        const auto &vertex_vec = elementary_contractions_[c];
        for (const auto &vertex : vertex_vec) {
          cout << vertex;
        }
        cout << endl;
      } cout
      << endl;)

  // 2. Apply the contractions to the second quantized operators and add new
  // tensors (density matrices, cumulants)

  // counter of how many second quantized operators are not contracted
  std::vector<WDiagVertex> ops_offset(ops.size());
  // vector to store the order of operators
  std::vector<int> sign_order(sqops.size(), -1);
  std::vector<std::vector<bool>> bit_map_vec;

  // a counter to keep track of the positions assigned to operators
  int sorted_position = 0;

  // a counter that keeps track of the number of sq operators contracted
  int nsqops_contracted = 0;

  // a sign factor that keeps into account a negative sign introduced by sorting
  // the operators in a unoccupied-unoccupied contraction
  int unoccupied_sign = 1;

  index_map_t pair_contraction_reindex_map;
  // Loop over elementary contractions
  for (int c : contraction) {
    std::vector<bool> bit_map(sqops.size(), false);

    // Find the rank and space of this contraction
    const auto &vertex_vec = elementary_contractions_[c];
    int rank = vertices_rank(vertex_vec);
    int s = vertices_space(vertex_vec);
    nsqops_contracted += rank;

    // find the position of the creation operators
    std::vector<int> pos_cre_sqops =
        vertex_vec_to_pos(vertex_vec, ops_offset, op_map, true);
    // find the position of the annihilation operators
    std::vector<int> pos_ann_sqops =
        vertex_vec_to_pos(vertex_vec, ops_offset, op_map, false);

    // mark the creation operators contracted and their order
    for (int c : pos_cre_sqops) {
      bit_map[c] = true;
      sign_order[c] = sorted_position;
      sorted_position += 1;
    }
    // mark the annihilation operators contracted and their order
    for (int a : pos_ann_sqops) {
      bit_map[a] = true;
      sign_order[a] = sorted_position;
      sorted_position += 1;
    }

    DMStructure dmstruc = osi->dmstructure(s);

    // Pairwise contractions creation-annihilation:
    // ________
    // |      |
    // a^+(i) a(j) = delta(i,j)

    if (dmstruc == DMStructure::DoublyOccupied) {
      // Reindex the annihilator (j) to the creator (i)
      WIndex cre_index = sqops[pos_cre_sqops[0]].index();
      WIndex ann_index = sqops[pos_ann_sqops[0]].index();
      pair_contraction_reindex_map[ann_index] = cre_index;
    }

    // Pairwise contractions creation-annihilation:
    // ______
    // |    |
    // a(i) a^+(j) = delta(i,j)

    if (dmstruc == DMStructure::Unoccupied) {
      // Reindex the creator (j) to the annihilator (i)
      WIndex cre_index = sqops[pos_cre_sqops[0]].index();
      WIndex ann_index = sqops[pos_ann_sqops[0]].index();
      pair_contraction_reindex_map[cre_index] = ann_index;
      unoccupied_sign *= -1;
    }

    // 2k-legged contractions (k >= 2) of k creation and k annihilation
    // operators:
    // _____________
    // |   |   |   |
    // a^+ a   a   a^+

    if (dmstruc == DMStructure::General) {
      std::vector<WIndex> lower;
      std::vector<WIndex> upper;
      // collect indices of creation operators for the upper indices
      for (int c : pos_cre_sqops) {
        upper.push_back(sqops[c].index());
      }
      // collect indices of annihilation operators for the lower indices
      for (int a : pos_ann_sqops) {
        lower.push_back(sqops[a].index());
      }
      // reverse the lower indices
      std::reverse(lower.begin(), lower.end());
      // prepare the label
      std::string label = "Lambda" + std::to_string(rank / 2);
      // add the cumulant to the list of tensors
      tensors.push_back(WTensor(label, lower, upper));
    }
    bit_map_vec.push_back(bit_map);
  }

  // assign an order to the uncontracted operators
  // creation operators come before annihilation operators
  for (SQOperatorType type : {Creation, Annihilation}) {
    for (int s = 0; s < osi->num_spaces(); s++) {
      for (int i = 0; i < sqops.size(); i++) {
        if ((sign_order[i] == -1) and (sqops[i].index().space() == s) and
            (sqops[i].type() == type)) {
          sign_order[i] = sorted_position;
          sorted_position += 1;
        }
      }
    }
  }

  PRINT(WDiagPrint::Basic,
        print_contraction(ops, tensors, bit_map_vec, sqops, sign_order);)

  int sign = unoccupied_sign * permutation_sign(sign_order);

  PRINT(WDiagPrint::All, PRINT_ELEMENTS(sign_order, "\n  positions: "););

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

  WAlgebraicTerm term;

  for (const auto &tensor : tensors) {
    term.add(tensor);
  }
  for (const auto &sqop : sqops) {
    term.add(sqop);
  }
  for (const auto &op : ops) {
    factor *= op.factor();
  }

  term.reindex(pair_contraction_reindex_map);

  PRINT(WDiagPrint::All, cout << "  sign = " << sign << endl;
        cout << "  factor = " << factor << endl;
        cout << "  combinatorial_factor = " << comb_factor << endl;)

  return std::make_pair(term, sign * factor * comb_factor);
}

std::tuple<std::vector<WTensor>, std::vector<WSQOperator>,
           std::map<std::tuple<int, int, bool, int>, int>>
WDiagTheorem::contration_tensors_sqops(const std::vector<WDiagOperator> &ops) {

  std::vector<WSQOperator> sqops;
  std::vector<WTensor> tensors;
  std::map<std::tuple<int, int, bool, int>, int> op_map;

  index_counter ic(osi->num_spaces());

  // Loop over all operators
  int n = 0;
  for (int o = 0; o < ops.size(); o++) {
    const auto &op = ops[o];

    // Loop over creation operators (lower indices)
    std::vector<WIndex> lower;
    for (int s = 0; s < osi->num_spaces(); s++) {
      for (int c = 0; c < op.num_cre(s); c++) {
        WIndex idx(s, ic.next_index(s)); // get next available index
        sqops.push_back(WSQOperator(Creation, idx));
        lower.push_back(idx);
        auto key = std::make_tuple(o, s, true, c);
        op_map[key] = n;
        PRINT(WDiagPrint::All, print_key(key, n););
        n += 1;
      }
    }

    // Loop over annihilation operators (upper indices)
    // the annihilation operators are layed out in a reversed order (hence the
    // need to reverse the upper indices of the tensor, see below)
    std::vector<WIndex> upper;
    for (int s = osi->num_spaces() - 1; s >= 0; s--) {
      for (int a = op.num_ann(s) - 1; a >= 0; a--) {
        WIndex idx(s, ic.next_index(s)); // get next available index
        sqops.push_back(WSQOperator(Annihilation, idx));
        upper.push_back(idx);
        auto key = std::make_tuple(o, s, false, a);
        op_map[key] = n;
        PRINT(WDiagPrint::All, print_key(key, n););
        n += 1;
      }
    }

    // reverse the order of the upper indices
    std::reverse(upper.begin(), upper.end());
    tensors.push_back(WTensor(op.label(), lower, upper));
  }
  return make_tuple(tensors, sqops, op_map);
}

std::vector<int> WDiagTheorem::vertex_vec_to_pos(
    const std::vector<WDiagVertex> &vertex_vec,
    std::vector<WDiagVertex> &ops_offset,
    std::map<std::tuple<int, int, bool, int>, int> &op_map, bool creation) {

  std::vector<int> result;

  int s = vertices_space(vertex_vec);

  PRINT(WDiagPrint::All, cout << "\n  Vertex to position:" << endl;);

  // Loop over all vertices
  for (int v = 0; v < vertex_vec.size(); v++) {
    const WDiagVertex &vertex = vertex_vec[v];
    int nops = creation ? vertex.cre(s) : vertex.ann(s);
    // assign the operator indices
    int ops_off = creation ? ops_offset[v].cre(s) : ops_offset[v].ann(s);
    for (int i = 0; i < nops; i++) {
      // find the operator corresponding to this leg
      auto key =
          creation
              ? std::make_tuple(v, s, true,
                                ops_off + i) // start from the leftmost operator
              : std::make_tuple(v, s, false, ops_off + i);
      if (op_map.count(key) == 0) {
        PRINT(WDiagPrint::All, print_key(key, -1););
        cout << " NOT FOUND!!!" << endl;
        exit(1);
      } else {
        int sqop_pos = op_map[key];
        result.push_back(sqop_pos);
        PRINT(WDiagPrint::All, print_key(key, sqop_pos););
      }
    }
    // update the creator's offset
    if (creation) {
      ops_offset[v].cre(s, ops_off + nops);
    } else {
      ops_offset[v].ann(s, ops_off + nops);
    }
  }
  return result;
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

void print_key(std::tuple<int, int, bool, int> key, int n) {
  cout << "key[vertex = " << std::get<0>(key)
       << ", space = " << std::get<1>(key)
       << ", creation = " << std::get<2>(key) << ", num = " << std::get<3>(key)
       << "] -> " << n << endl;
}

void print_contraction(const std::vector<WDiagOperator> &ops,
                       const std::vector<WTensor> &tensors,
                       const std::vector<std::vector<bool>> &bit_map_vec,
                       const std::vector<WSQOperator> &sqops,
                       const std::vector<int> sign_order) {
  for (const auto &bit_map : bit_map_vec) {
    int ntrue = std::count(bit_map.begin(), bit_map.end(), true);
    bool line = false;
    bool first = true;
    cout << "";
    for (bool b : bit_map) {
      if (b) {
        line = true;
        ntrue -= 1;
      }
      if (line) {
        if (first) {
          cout << " __";
          first = false;
        } else if (ntrue == 0) {
          cout << "__ ";
        } else {
          cout << "___";
        }
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
  for (int order : sign_order) {
    cout << " " << order << " ";
  }
  cout << "\n" << endl;

  int nsqops = sqops.size();
  int opoffset = 0;
  for (const auto &tensor : tensors) {
    int oprank = tensor.rank();
    for (int i = 0; i < opoffset; i++) {
      cout << "   ";
    }
    for (int i = 0; i < oprank; i++) {
      cout << "---";
    }
    for (int i = 0; i < nsqops - oprank - opoffset; i++) {
      cout << "   ";
    }
    cout << " " << tensor.str();
    opoffset += oprank;
    cout << endl;
  }
  //  for (const auto &op : ops) {
  //    int oprank = op.rank();
  //    for (int i = 0; i < opoffset; i++) {
  //      cout << "   ";
  //    }
  //    for (int i = 0; i < oprank; i++) {
  //      cout << "---";
  //    }
  //    for (int i = 0; i < nsqops - oprank - opoffset; i++) {
  //      cout << "   ";
  //    }
  //    cout << " " << op.label();
  //    opoffset += oprank;
  //    cout << endl;
  //  }
}
