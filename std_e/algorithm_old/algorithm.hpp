#pragma once


#include <algorithm>
#include "std_e/interval/interval.hpp"


namespace std_e {


template<class I> auto
rotated_position(I index, I first, I n_first, I last) -> I {
  // Returns the new index of a sequence [first,last) having been rotated around n_first
  // Preconditions:
  // - first <= index <= last
  // - first <= n_first <= last
  I first_range_size = n_first-first;
  I second_range_size = last-n_first;
  if (is_in(index,{first,n_first})) {
    return index + second_range_size;
  }
  if (is_in(index,{n_first,last})) {
    return index - first_range_size;
  }
  return index;
}


// TODO permute: see algorithm/permutation
template<class Rand_it0, class Output_it, class Rand_it1, class I> auto
// requires Rand_it0,Rand_it1 are random access iterators
// requires Output_it is an output iterator
permute_copy_n(Rand_it0 first, Output_it d_first, Rand_it1 first_permutation_index, I n) -> Output_it {
  // Write permutation of range [first,first+permutation_indices.size()) at [d_first,d_first+permutation_indices.size())
  // Preconditions:
  //     - [first,first+n) is a valid range
  //     - [d_first,d_first+n) is a valid range
  //     - permutation_indices = [first_permutation_index,first_permutation_index+n) is an index permutation (i.e. sort(permutation_indices) == integer_range(n))
  for (I i=0; i<n; ++i) {
    *d_first = *(first+first_permutation_index[i]);
    ++d_first;
  }
  return d_first;
}

template<class Rand_it, class Output_it, class I> auto
// requires Rand_it is a random access iterator
// requires Output_it is an output iterator
permute_copy(Rand_it first, Output_it d_first, const std::vector<I>& permutation_indices) -> Output_it {
  // Write permutation of range [first,first+permutation_indices.size()) at [d_first,d_first+permutation_indices.size())
  // Preconditions:
  //     - [first,first+permutation_indices.size()) is a valid range
  //     - [d_first,d_first+permutation_indices.size()) is a valid range
  //     - "permutation_indices" is an index permutation (i.e. sort(permutation_indices) == integer_range(permutation_indices.size()))
  return permute_copy_n(first,d_first,permutation_indices.begin(),permutation_indices.size());
}

template<class Rand_it, class I> auto
// requires Rand_it is a random access iterator
permute(Rand_it first, const std::vector<I>& permutation_indices) -> void {
  // Permutation of range [first,first+permutation_indices.size())
  // Preconditions:
  //     - [first,first+permutation_indices.size()) is a valid range
  //     - "permutation_indices" is an index permutation (i.e. sort(permutation_indices) == integer_range(permutation_indices.size()))
  using T = std::remove_reference_t<decltype(*first)>;
  std::vector<T> a_permuted(permutation_indices.size());
  permute_copy(first,a_permuted.begin(),permutation_indices);

  std::copy(a_permuted.begin(),a_permuted.end(),first);
}

template<class C> auto
// requires C is a random access iterable 
inverse_permutation(const C& a) -> C {
  // Precondition: "a" is an index permutation (i.e. sort(a) == integer_range(a.size())
  int sz = a.size();
  C b(sz);
  for (int i=0; i<sz; ++i) {
    b[a[i]] = i;
  }
  return b;
}


} // std_e
