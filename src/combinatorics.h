#ifndef _wicked_combinatorics_h_
#define _wicked_combinatorics_h_

#include <vector>

typedef std::vector<std::vector<int> > vecvecint;
typedef std::vector<int> vecint;

// Combinations
std::vector<std::vector<int> >
  generate_combinations(int n, int k, std::vector<int>& labels);
std::vector<std::vector<int> >
  generate_combinations(int n, int k);
std::vector<std::vector<int> >
  generate_direct_product_combinations(std::vector<int>& n, std::vector<int>& k);

// Permutations
std::vector<std::vector<int> >
  generate_permutations(int n, std::vector<int>& labels);

// Set partitions
std::vector<std::vector<int> >
  generate_set_partitions(int n);
std::vector<std::vector<int> >
generate_even_set_partitions(int n);
bool next_set_partition(size_t n, std::vector<int>& k, std::vector<int>& m);

// Integer partitions
std::vector<std::vector<int> >
  generate_partitions(int n);
std::vector<std::vector<int> >
  generate_even_partitions(int n);

template<typename T>
std::vector<std::vector<T>> enumeration_elements(const std::vector<std::vector<T>>& sets)
{
    size_t nbins = sets.size();
    std::vector<size_t> bin_offset(1,nbins);
    std::vector<size_t> bin_size(nbins);

    size_t ncomb = 1;
    for (size_t i = 0; i < nbins; i++){
        bin_size[i] = sets[i].size();
        ncomb *= bin_size[i];
    }

    for (size_t i = 1; i < nbins; i++){
        bin_offset[nbins - i - 1] = bin_offset[nbins - i] * sets[nbins - i - 1].size();
    }

    std::vector<std::vector<T>> result(std::vector<T>(nbins),ncomb);

    for (size_t i = 0; i < nbins; i++){
        size_t n = 0;
        size_t maxl = ncomb / (bin_size[i] * bin_offset[i]);
        for (size_t l = 0; l < maxl; l++){
            for (size_t j = 0; j < bin_size[i]; j++){
                for (size_t k = 0; k < bin_offset[i]; k++){
                    result[n][i] = sets[i][j];
                    n++;
                }
            }
        }
    }
    return results;
}

#endif // _wicked_combinatorics_h_
