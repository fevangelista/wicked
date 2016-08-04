#include <iostream>
#include "combinatorics.h"

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

int binomial(int n, int k);

int binomial(int n, int k) {
  double combinations = 1.0;
  for (int i = 0; i < k; ++i) {
    combinations *= static_cast<double>(n - i) / static_cast<double>(k - i);
  }
  return (static_cast<int>(combinations));
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
