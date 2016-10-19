#include <assert.h>
#include <algorithm>
#include <iostream>
#include <numeric>

#include "stl_utils.hpp"

#include "combinatorics.h"

#define DEBUG_PRODUCT_SPACE(code) //{ code }

int factorial(int n) {
  if (n == 0)
    return 1;
  int result = 1;
  for (int i = 2; i <= n; ++i) {
    result *= i;
  }
  return result;
}

int binomial(int n, int k) {
  if (k > n)
    return 0;
  if (k * 2 > n)
    k = n - k;
  if (k == 0)
    return 1;

  int result = n;
  for (int i = 2; i <= k; ++i) {
    result *= (n - i + 1);
    result /= i;
  }
  return result;
}

// see
// http://math.stackexchange.com/questions/65923/how-does-one-compute-the-sign-of-a-permutation
int permutation_sign(const std::vector<int> &vec) {
  int sign = 0;
  int n = vec.size();
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; ++j) {
      if (vec[i] > vec[j]) {
        sign += 1;
      }
    }
  }
  return (sign % 2 == 0) ? 1 : -1;
}

std::vector<std::vector<int>> integer_partitions(int n) {
  std::vector<std::vector<int>> partitions;
  if (n > 1) {
    // Implements the ZS2 algorithm by A. Zoghbiu and I. Stojmenovic'
    // see Intern. J. Computer Math., Vol. 70. pp. 319
    std::vector<int> x(n + 1, 1);
    partitions.push_back(std::vector<int>(x.begin() + 1, x.begin() + n + 1));
    x[0] = 1;
    x[1] = 2;
    int h = 1;
    int m = n - 1;
    partitions.push_back(std::vector<int>(x.begin() + 1, x.begin() + m + 1));
    while (x[1] != n) {
      if (m - h > 1) {
        h++;
        x[h] = 2;
        m--;
      } else {
        int j = m - 2;
        while (x[j] == x[m - 1]) {
          x[j] = 1;
          j--;
        }
        h = j + 1;
        x[h] = x[m - 1] + 1;
        int r = x[m] + x[m - 1] * (m - h - 1);
        x[m] = 1;
        if ((m - h) > 1) {
          x[m - 1] = 1;
        }
        m = h + r - 1;
      }
      partitions.push_back(std::vector<int>(x.begin() + 1, x.begin() + m + 1));
    }
  } else if (n == 0) {
    // Special case not handled by ZS2
    partitions.push_back({});
  } else if (n == 1) {
    // Special case not handled by ZS2
    partitions.push_back({1});
  }
  return partitions;
}

std::vector<std::vector<int>> even_integer_partitions(int n) {
  std::vector<std::vector<int>> even_partitions;
  std::vector<std::vector<int>> partitions = integer_partitions(n);
  for (std::vector<int> &p : partitions) {
    bool keep = true;
    for (int x : p) {
      if (x % 2 == 1)
        keep = false;
    }
    if (keep)
      even_partitions.push_back(p);
  }
  return (even_partitions);
}

void test_integer_partitions() {
  int maxn = 30;

  std::cout << "testing integer partitions in the range [0," << maxn << "]"
            << std::endl;

  // Exact values of the partition function (sequence A000041 in the OEIS)
  // see https://en.wikipedia.org/wiki/Partition_(number_theory)
  std::vector<int> exact = {1,    1,    2,    3,    5,    7,    11,   15,
                            22,   30,   42,   56,   77,   101,  135,  176,
                            231,  297,  385,  490,  627,  792,  1002, 1255,
                            1575, 1958, 2436, 3010, 3718, 4565, 5604};

  for (int n = 0; n <= maxn; n++) {
    auto partitions = integer_partitions(n);
    // remove duplicates
    sort(partitions.begin(), partitions.end());
    partitions.erase(unique(partitions.begin(), partitions.end()),
                     partitions.end());
    if (partitions.size() != exact[n]) {
      std::cout << "test_integer_partitions() has found a problem for n = " << n
                << std::endl;
      exit(1);
    }
  }
}

void product_space_iterator(
    const std::vector<int> &r,
    const std::function<void(const std::vector<int> &)> &func) {
  int n = r.size(); // number of indices

  int exact = 1;
  for (int ri : r)
    exact *= ri;

  // Zero indices -> no elements
  if (n == 0) {
    return;
  }
  // One or more spaces has zero dimension -> no elements
  if (exact == 0) {
    return;
  }

  std::vector<int> a(n, 0); // current configuration

  std::vector<std::vector<int>> ps;

  func(a);
  ps.push_back(a);
  int i = 0;
  int l = 1;
  for (;;) {
    // try to increase bin i
    if (a[i] < r[i] - 1) {
      a[i] += 1;

      // reset all previous bins
      for (int j = 0; j < i; ++j) {
        a[j] = 0;
      }

      // output configuration
      func(a);
      ps.push_back(a);

      // restart from the beginning
      i = 0;
      l++;
    } else { // try the next bin
      i++;
    }
    // if we go beyond the last bin we're done
    if (i == n)
      break;
  }
  DEBUG_PRODUCT_SPACE(std::cout << "Number of elements:" << l
                                << " (exact = " << exact << ")" << std::endl;)

  assert(l == exact);

  size_t all = ps.size();
  std::sort(ps.begin(), ps.end());
  ps.erase(std::unique(ps.begin(), ps.end()), ps.end());
  size_t unique = ps.size();
  assert(all == unique);
}

std::vector<std::vector<int>> product_space(const std::vector<int> &r) {
  std::vector<std::vector<int>> elements;
  product_space_iterator(
      r, [&](const std::vector<int> &el) { elements.push_back(el); });
  return elements;
}

void test_product_space() {
  int maxn = 30;

  std::cout << "testing product space" << std::endl;

  std::vector<std::vector<int>> tests{{1, 1, 1}, {2, 1, 1}};
  std::vector<std::vector<std::vector<int>>> results{{{0, 0, 0}},
                                                     {{0, 0, 0}, {0, 0, 1}}};
  for (const auto &test : tests) {
    auto results = product_space(test);
    std::sort(results.begin(), results.end());
    bool pass = true;
    // TODO: add check
    //    for (int n = 0; n < resul)
    //    for (auto &result : results) {

    //      PRINT_ELEMENTS(result);
    //    }
  }
}

void generalized_combinations_with_repetitions_iterator(
    const std::vector<int> &r, const std::vector<int> &k,
    const std::function<void(const std::vector<std::vector<int>> &)> &func) {
  int n = r.size();                // number of indices
  std::vector<std::vector<int>> a; // current configuration

  // special case
  if (n == 0)
    return;

  // Initialize each bin
  for (int i = 0; i < n; i++) {
    a.push_back(std::vector<int>(k[i], 0));
  }

  std::vector<std::vector<std::vector<int>>> ps;

  func(a);
  ps.push_back(a);
  int i = 0;
  int j = 0;
  int l = 1;
  for (;;) {
    // try to increase bin (i,j)
    if (a[i][j] == r[i] - 1) {
      // if this walker reached the top, move to the next bin
      i++;
    } else if ((j < k[i] - 1) and (a[i][j] == a[i][j + 1])) {
      // if another walker is blocking this
      // one try the next walker
      j++;
    } else {
      // increase walker j on bin i
      a[i][j] += 1;

      // reset all previous walkers
      for (int p = 0; p < i; ++p) {
        for (int m = 0; m < k[p]; m++) {
          a[p][m] = 0;
        }
      }
      for (int m = 0; m < j; m++) {
        a[i][m] = 0;
      }

      // output configuration
      func(a);
      ps.push_back(a);

      // restart from the beginning
      i = 0;
      j = 0;
      l++;
    }

    // if we go beyond the last bin we're done
    if (i == n)
      break;
  }

  int exact = 1;
  for (int m = 0; m < n; ++m) {
    exact *= binomial(r[m] + k[m] - 1, k[m]);
    //    std::cout << r[m] + k[m] - 1 << " " << k[m] << " "
    //              << binomial(r[m] + k[m] - 1, r[m]) << std::endl;
  }
  if (l != exact) {
    std::cout << "generalized_combinations_with_repetitions_iterator():"
              << std::endl;
    std::cout << "l = " << l << "exact = " << exact << std::endl;
  }
  assert(l == exact);

  size_t all = ps.size();
  std::sort(ps.begin(), ps.end());
  ps.erase(std::unique(ps.begin(), ps.end()), ps.end());
  size_t unique = ps.size();
  assert(all == unique);
}

std::vector<std::vector<std::vector<int>>>
generalized_combinations_with_repetitions(const std::vector<int> &r,
                                          const std::vector<int> &k) {
  std::vector<std::vector<std::vector<int>>> elements;
  generalized_combinations_with_repetitions_iterator(
      r, k,
      [&](const std::vector<std::vector<int>> &el) { elements.push_back(el); });
  return elements;
}

/**
 * Generate combinations of 0,1,...,(n-1) taken k at a time
 * @param n
 * @param k
 */
std::vector<std::vector<int>> generate_combinations(int n, int k,
                                                    std::vector<int> &labels) {
  std::vector<std::vector<int>> combinations;
  if ((n > 0) && (k > 0)) {
    std::vector<int> combination;
    bool *a = new bool[n];
    for (int i = 0; i < n - k; ++i)
      a[i] = false;
    for (int i = n - k; i < n; ++i)
      a[i] = true;
    do {
      combination.clear();
      for (int i = 0; i < n; ++i) {
        if (a[i])
          combination.push_back(labels[i]);
      }
      combinations.push_back(combination);
    } while (std::next_permutation(a, a + n));
    delete[] a;
  }
  return combinations;
}

/**
 * Generate combinations of 0,1,...,(n-1) taken k at a time
 * @param n
 * @param k
 */
std::vector<std::vector<int>> generate_combinations(int n, int k) {
  std::vector<int> labels;
  for (int i = 0; i < n; ++i)
    labels.push_back(i);
  return generate_combinations(n, k, labels);
}

/**
 * Generate permutation of n labels
 * @param n
 * @param labels a vector of integers
 */
std::vector<std::vector<int>> generate_permutations(int n,
                                                    std::vector<int> &labels) {
  std::vector<std::vector<int>> permutations;
  if (n > 0) {
    do {
      permutations.push_back(labels);
    } while (std::next_permutation(labels.begin(), labels.end()));
  }
  return permutations;
}

void reset_class(bool *a, std::vector<int> &n, std::vector<int> &k,
                 std::vector<int> &offset, int q);

void reset_class(bool *a, std::vector<int> &n, std::vector<int> &k,
                 std::vector<int> &offset, int q) {
  for (int i = 0; i < n[q] - k[q]; ++i)
    a[i + offset[q]] = false;
  for (int i = n[q] - k[q]; i < n[q]; ++i)
    a[i + offset[q]] = true;
}

/**
 *  Generate direct products of combinations
 */
std::vector<std::vector<int>>
generate_direct_product_combinations(std::vector<int> &n, std::vector<int> &k) {
  std::vector<int> labels;
  std::vector<int> offset;
  std::vector<std::vector<int>> combinations;

  int total_combinations = 1;
  int sumn = 0;
  int nclass = static_cast<int>(n.size());
  for (int n_i : n) {
    sumn += n_i;
  }
  bool *a = new bool[sumn];
  offset.push_back(0);
  for (int q = 0; q < nclass; ++q) {
    reset_class(a, n, k, offset, q);
    for (int i = 0; i < n[q]; ++i)
      labels.push_back(i);
    offset.push_back(offset[q] + n[q]);
    total_combinations *= binomial(n[q], k[q]);
  }
  //  std::cout << "\n nclass = " << nclass << std::endl;
  // Start by permuting the first class
  std::vector<int> combination;
  int permute_class = 0;
  bool continue_loop = true;
  bool skip_next = false;
  do {
    if (not skip_next) {
      combination.clear();
      for (int i = 0; i < sumn; ++i) {
        if (a[i])
          combination.push_back(labels[i]);
      }
      combinations.push_back(combination);
      //      std::cout << "\n ";
      //      for(int i = 0; i < sumn; ++i)
      //        std::cout << " " << a[i];
    } else {
      skip_next = false;
    }
    bool continue_with_class =
        std::next_permutation(a + offset[permute_class],
                              a + offset[permute_class] + n[permute_class]);
    //    std::cout << "\n continue_with_class = " << continue_with_class <<
    //    std::endl;
    if (not continue_with_class) {
      reset_class(a, n, k, offset, permute_class);
      permute_class += 1;
      skip_next = true;
    } else {
      permute_class = 0;
    }
    if (permute_class == nclass) {
      continue_loop = false;
    }
  } while (continue_loop);

  if (total_combinations != combinations.size()) {
    std::cout << "\n There is a problem with the direct product of combinations"
              << total_combinations;
    std::cout << "\n total combinations = " << total_combinations;
    std::cout << "\n total combinations = " << combinations.size() << std::endl;
  }

  delete[] a;
  return combinations;
}

// Williamson's implementation (seems to have a bug if one of the r_i = 1)
// void product_space(const std::vector<int> &r,
//                   const std::function<void(const std::vector<int> &)> &func)
//                   {
//  int n = r.size(); // number of indices
//  std::vector<int> a(n, 0);
//  std::vector<int> d(n, 1);
//  std::vector<int> l(n + 1);
//  std::iota(l.begin(), l.end(), 1);
//  int p = 1;

//  int k = 0;
//  while (p <= n) {
//    for (int i : a) {
//      std::cout << ' ' << i;
//    }

//    for (int i : d) {
//      std::cout << ' ' << i;
//    }

//    for (int i : l) {
//      std::cout << ' ' << i;
//    }
//    std::cout << std::endl;

//    func(a);
//    l[0] = 1;
//    a[p - 1] += d[p - 1];
//    if ((a[p - 1] == 0) or (a[p - 1] == r[p - 1] - 1)) {
//      d[p - 1] = -d[p - 1];
//      l[p - 1] = l[p];
//      l[p] = p + 1;
//    }
//    p = l[0];
//    k++;
//    if (k > 100)
//      p = n + 1;
//  }
//  func(a);
//}

// void product_space(const std::vector<int> &r,
//                   const std::function<void(const std::vector<int> &)> &func)
//                   {

//  int n = r.size(); // number of indices

//  size_t size = 1; // size of the product space
//  for (int ri : r) {
//    size *= static_cast<size_t>(ri);
//  }

//  assert(size <= 64);

//  // build masks to count bits
//  std::vector<std::bitset<64>> mask(n);
//  std::vector<int> offset(n);
//  int current_offset = 0;
//  for (int i = 0; i < n; i++) {
//    offset[i] = current_offset;
//    std::bitset<64> &mask_i = mask[i];
//    for (int k = 0; k < r[i] - 1; k++) {
//      mask_i.set(current_offset + k);
//    }
//    current_offset += r[i] - 1;
//    DEBUG_PRODUCT_SPACE(std::cout << "Mask " << i << " : " << mask_i
//                                  << std::endl;)
//  }

//  // Generate the product space by creating all possible strings of bits
//  std::bitset<64> bs;
//  std::bitset<64> bs_count;
//  std::vector<int> s(n);
//  for (size_t r = 0; r < size; r++) {
//    bs = r;
//    DEBUG_PRODUCT_SPACE(std::cout << r << " = " << bs << std::endl;)
//    for (int i = 0; i < n; i++) {
//      DEBUG_PRODUCT_SPACE(
//          std::cout << "bs             = " << bs << std::endl;
//          std::cout << "mask[i]        = " << mask[i] << std::endl;
//          std::cout << "(bs & mask[i]) = " << (bs & mask[i]) << std::endl;
//          std::cout << "(bs & mask[i]) = " << ((bs & mask[i]) >> offset[i])
//                    << std::endl;)
//      s[i] = ((bs & mask[i]) >> offset[i]).to_ulong();
//      DEBUG_PRODUCT_SPACE(std::cout << i << " -> " << s[i] << std::endl;)
//    }
//    func(s);
//  }
//}
