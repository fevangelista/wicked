#include "combinatorics.h"

long long int factorial(int n) {
  long long int result = 1;
  for (long long int i = 2; i <= n; ++i) {
    result *= i;
  }
  return result;
}

long long int binomial(int n, int k) {
  if (k > n)
    return 0;
  if (k * 2 > n)
    k = n - k;
  if (k == 0)
    return 1;

  long long int result = n;
  for (long long int i = 2; i <= k; ++i) {
    result *= (n - i + 1);
    result /= i;
  }
  return result;
}

int permutation_sign(const std::vector<int> &vec) {
  /// A quadratic algorithm to compute the sign of a permutation
  /// http://math.stackexchange.com/questions/65923/how-does-one-compute-the-sign-of-a-permutation
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

std::vector<std::vector<int>> integer_partitions(int n, int maxlen) {
  std::vector<std::vector<int>> partitions;
  if (n > 1) {
    // Implements the ZS2 algorithm by A. Zoghbiu and I. Stojmenovic'
    // see Intern. J. Computer Math., Vol. 70. pp. 319
    std::vector<int> x(n + 1, 1);
    if (n <= maxlen) {
      partitions.push_back(std::vector<int>(x.begin() + 1, x.begin() + n + 1));
    }
    x[0] = 1;
    x[1] = 2;
    int h = 1;
    int m = n - 1;
    if (m <= maxlen) {
      partitions.push_back(std::vector<int>(x.begin() + 1, x.begin() + m + 1));
    }
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
      if (m <= maxlen) {
        partitions.push_back(
            std::vector<int>(x.begin() + 1, x.begin() + m + 1));
      }
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
