#ifndef _wicked_combinatorics_h_
#define _wicked_combinatorics_h_

#include <vector>
#include <functional>

typedef std::vector<std::vector<int>> vecvecint;
typedef std::vector<int> vecint;

// ==> Integer partitions <==

/// Generate all the partitions of the integer n
std::vector<std::vector<int>> integer_partitions(int n);

/// Generate all the even partitions of the even integer n
std::vector<std::vector<int>> even_integer_partitions(int n);

/// Function to test integer partitions
void test_integer_partitions();

// ==> Product space <==

/// Given a vector of ranges (r_i, i = 0,...,n-1) iterate over the product space
/// S = S(0) x S(1) x ... S(n-1) where S(i) = {0,1,2,...,r_i - 1)
/// If one or more spaces are empty (that is, r_i = 0 for some i) then this
/// iterator will not call func
void product_space_iterator(
    const std::vector<int> &r,
    const std::function<void(const std::vector<int> &)> &func);

/// Given a vector of ranges (r_i, i = 0,...,n-1) compute the product space
/// S = S(0) x S(1) x ... S(n-1) where S(i) = {0,1,2,...,r_i - 1)
/// If one or more spaces are empty (that is, r_i = 0 for some i) then this
/// function will return an empty vector
std::vector<std::vector<int>> product_space(const std::vector<int> &r);

/// Function to test product space
void test_product_space();

/// Given a vector of ranges (r_i, i = 0,...,n-1) and number of elements
/// (k_i, i = 0,...,n-1) iterate over the product space
/// C = C(S(0),k_0) x C(S(1),k_1) x C(S(2),k_2) x ... where C(S,k) stands for
/// all the combinations of k elements taken from S (with repetitions).
void generalized_combinations_with_repetitions_iterator(
    const std::vector<int> &r, const std::vector<int> &k,
    const std::function<void(const std::vector<std::vector<int>> &)> &func);

/// Given a vector of ranges (r_i, i = 0,...,n-1) and number of elements
/// (k_i, i = 0,...,n-1) compute the product space
/// C = C(S(0),k_0) x C(S(1),k_1) x C(S(2),k_2) x ... where C(S,k) stands for
/// all the combinations of k elements taken from S (with repetitions).
std::vector<std::vector<std::vector<int>>>
generalized_combinations_with_repetitions(const std::vector<int> &r,
                                          const std::vector<int> &k);

// ==> Combinations <==

std::vector<std::vector<int>> generate_combinations(int n, int k,
                                                    std::vector<int> &labels);
std::vector<std::vector<int>> generate_combinations(int n, int k);
std::vector<std::vector<int>>
generate_direct_product_combinations(std::vector<int> &n, std::vector<int> &k);

// Permutations
std::vector<std::vector<int>> generate_permutations(int n,
                                                    std::vector<int> &labels);

template <typename T>
std::vector<std::vector<T>>
enumeration_elements(const std::vector<std::vector<T>> &sets) {
  size_t nbins = sets.size();
  std::vector<size_t> bin_offset(1, nbins);
  std::vector<size_t> bin_size(nbins);

  size_t ncomb = 1;
  for (size_t i = 0; i < nbins; i++) {
    bin_size[i] = sets[i].size();
    ncomb *= bin_size[i];
  }

  for (size_t i = 1; i < nbins; i++) {
    bin_offset[nbins - i - 1] =
        bin_offset[nbins - i] * sets[nbins - i - 1].size();
  }

  std::vector<std::vector<T>> result(std::vector<T>(nbins), ncomb);

  for (size_t i = 0; i < nbins; i++) {
    size_t n = 0;
    size_t maxl = ncomb / (bin_size[i] * bin_offset[i]);
    for (size_t l = 0; l < maxl; l++) {
      for (size_t j = 0; j < bin_size[i]; j++) {
        for (size_t k = 0; k < bin_offset[i]; k++) {
          result[n][i] = sets[i][j];
          n++;
        }
      }
    }
  }
  return result;
}

#endif // _wicked_combinatorics_h_
