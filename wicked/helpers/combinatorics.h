#pragma once

#include <vector>

/// Compute the factorial of an integer
long long int factorial(int n);

/// Compute a binomial coefficient
long long int binomial(int n, int k);

/// Generate all the integer partitions of the integer n
/// For example, for n = 4 this code generates
/// [[1, 1, 1, 1], [2, 1, 1], [2, 2], [3, 1], [4]]
std::vector<std::vector<int>> integer_partitions(int n, int maxlen = 1024);

// Computes the sign of a permutation of integers
int permutation_sign(const std::vector<int> &vec);
