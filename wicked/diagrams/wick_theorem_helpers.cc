#include <iomanip>
#include <iostream>
#include <map>

#include "fmt/format.h"

#include "diag_operator.h"

#include "../algebra/sqoperator.h"
#include "../algebra/tensor.h"

using namespace std;

void print_contraction(const std::vector<DiagOperator> &ops,
                       const std::vector<std::vector<DiagVertex>> &contractions,
                       const std::vector<int> ops_perm,
                       const std::vector<int> contr_perm) {

  std::vector<DiagVertex> op_vertex_vec;
  for (int o : ops_perm) {
    op_vertex_vec.push_back(ops[o].vertex());
  }

  for (int o : ops_perm) {
    cout << setw(2) << ops[o].label() << "    ";
  }
  cout << endl;
  cout << to_string(op_vertex_vec) << endl;
  for (int c : contr_perm) {
    std::vector<DiagVertex> permuted_contr;
    for (int o : ops_perm) {
      permuted_contr.push_back(contractions[c][o]);
    }
    cout << "\n" << to_string(permuted_contr) << endl;
  }
  cout << endl;
}

void print_key(std::tuple<int, int, bool, int> key, int n) {
  cout << "key[vertex = " << std::get<0>(key)
       << ", space = " << std::get<1>(key)
       << ", creation = " << std::get<2>(key) << ", num = " << std::get<3>(key)
       << "] -> " << n << endl;
}

void print_contraction(const std::vector<DiagOperator> &ops,
                       const std::vector<Tensor> &tensors,
                       const std::vector<std::vector<bool>> &bit_map_vec,
                       const std::vector<SQOperator> &sqops,
                       const std::vector<int> sign_order) {
  std::string pre("          ");
  // 1. Draw the contraction legs
  for (const auto &bit_map : bit_map_vec) {
    int first = std::distance(bit_map.begin(),
                              std::find(bit_map.begin(), bit_map.end(), true));
    int last = std::distance(std::find(bit_map.rbegin(), bit_map.rend(), true),
                             bit_map.rend());
    std::cout << pre;
    for (int k = 0; k < first; k++) {
      cout << "   ";
    }
    std::cout << " ┌─";
    for (int k = first + 1; k < last - 1; k++) {
      std::cout << (bit_map[k] ? "─┬─" : "───");
    }
    std::cout << "─┐ " << std::endl;
  }

  // 2. Show the type of operator (+ = creation, - = annihilation)
  std::cout << "  type    ";
  for (const auto &sqop : sqops) {
    std::cout << ((sqop.type() == SQOperatorType::Creation) ? " + " : " - ");
  }
  std::cout << endl;

  // 3. Show the operator index
  std::cout << "  indices ";
  for (const auto &sqop : sqops) {
    std::cout << fmt::format("{:3s}", sqop.index().str());
  }
  std::cout << endl;

  // 3. Show the permutation of the operators
  std::cout << "  order  ";
  for (int order : sign_order) {
    std::cout << fmt::format("{:3d}", order);
  }
  std::cout << "\n" << endl;

  // 5. Show the indices of the tensors involved
  std::map<Index, int> index_map;
  int k = 0;
  for (int k = 0; k < sqops.size(); k++) {
    index_map[sqops[k].index()] = k;
  }
  for (const auto &tensor : tensors) {
    std::vector<int> indices(index_map.size(), 0);
    for (const auto &idx : tensor.upper()) {
      indices[index_map[idx]] = 1;
    }
    for (const auto &idx : tensor.lower()) {
      indices[index_map[idx]] = -1;
    }
    std::cout << "          ";
    for (int i : indices) {
      if (i == 1) {
        std::cout << "─┸─";
      } else if (i == -1) {
        std::cout << "─┰─";
      } else {
        std::cout << "───";
      }
    }
    std::cout << "── " << tensor.str() << std::endl;
  }
  std::cout << std::endl;
}
