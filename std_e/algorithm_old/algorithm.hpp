#pragma once


#include "std_e/Multi_array/Array_view.h"
#include "std_e/index_t.h"
#include <algorithm>


namespace std_e {


inline auto
rotated_position(index_t index, index_t first, index_t n_first, index_t last) -> index_t {
    // Returns the new index of a sequence [first,last) having been rotated around n_first
    // Preconditions:
    // - first <= index <= last
    // - first <= n_first <= last
    index_t first_range_size = n_first-first;
    index_t second_range_size = last-n_first;
    if (is_in_range(index,I_range<index_t>{first,n_first})) {
        return index + second_range_size;
    }
    if (is_in_range(index,I_range<index_t>{n_first,last})) {
        return index - first_range_size;
    }
    return index;
}


template<class Rand_it0, class Rand_it1, class Output_it> auto
// requires Rand_it0,Rand_it1 are random access iterators
// requires Output_it is an output iterator
permute_copy_n(Rand_it0 first, Output_it d_first, Rand_it1 first_permutation_index, index_t n) -> Output_it {
    // Write permutation of range [first,first+permutation_indices.size()) at [d_first,d_first+permutation_indices.size())
    // Preconditions:
    //     - [first,first+n) is a valid range
    //     - [d_first,d_first+n) is a valid range
    //     - permutation_indices = [first_permutation_index,first_permutation_index+n) is an index permutation (i.e. sort(permutation_indices) == integer_range(n))
    for (index_t i=0; i<n; ++i) {
        *d_first = *(first+first_permutation_index[i]);
        ++d_first;
    }
    return d_first;
}

template<class Rand_it, class Output_it> auto
// requires Rand_it is a random access iterator
// requires Output_it is an output iterator
permute_copy(Rand_it first, Output_it d_first, const std::vector<index_t>& permutation_indices) -> Output_it {
    // Write permutation of range [first,first+permutation_indices.size()) at [d_first,d_first+permutation_indices.size())
    // Preconditions:
    //     - [first,first+permutation_indices.size()) is a valid range
    //     - [d_first,d_first+permutation_indices.size()) is a valid range
    //     - "permutation_indices" is an index permutation (i.e. sort(permutation_indices) == integer_range(permutation_indices.size()))
    return permute_copy_n(first,d_first,permutation_indices.begin(),permutation_indices.size());
}

template<class Rand_it> auto
// requires Rand_it is a random access iterator
permute(Rand_it first, const std::vector<index_t>& permutation_indices) -> void {
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
    C b(a.size());
    for (index_t i=0; i<a.size(); ++i) {
        b[a[i]] = i;
    }
    return b;
}


} // std_e
