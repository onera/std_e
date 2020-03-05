#pragma once

#include "std_e/algorithm/algorithm.hpp"


namespace std_e {


template<class I> auto
permute_id(I& id, const std::vector<index_t>& permutation, index_t offset) -> void {
    // Precondition: offset <= id < offset + permutation.size()
    index_t old_index = id - offset;
    index_t new_index = permutation[old_index];
    id = new_index + offset;
}

template<class I> auto
permute_id_if_in_range(I& id, const std::vector<index_t>& permutation, I_range<index_t> range) -> void {
    // Precondition: size(range) == permutation.size()
    if ( is_in_range(id,range) ) {
        permute_id(id,permutation,range.first);
    }
}


template<class I_iterable> auto
// requires I_iterable is an integer iterable (i.e. begin(), end())
update_ids_after_permutation(I_iterable&& ids, const std::vector<index_t>& permutation, index_t offset) {
    // Preconditions:
    //   - permutation is an index permutation (i.e. sort(permutation) == integer_range(permutation.size()))
    //   - for id in ids, offset <= id < offset + permutation.size()
    auto inv_perm = inverse_permutation(permutation);
    for (auto& id : ids) {
        permute_id(id,inv_perm,offset);
    }
}
template<class I_iterable> auto
// requires I_iterable is an integer iterable (i.e. begin(), end())
update_ids_in_range_after_permutation(I_iterable&& ids, const std::vector<index_t>& permutation, I_range<index_t> range) -> void {
    // Preconditions:
    //   - permutation is an index permutation (i.e. sort(permutation) == integer_range(permutation.size()))
    //   - size(range) == permutation.size()
    auto inv_perm = inverse_permutation(permutation);
    for (auto& id : ids) {
        permute_id_if_in_range(id,inv_perm,range);
    }
}


} // std_e
