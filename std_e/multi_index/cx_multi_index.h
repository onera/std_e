#pragma once


#include "std_e/multi_index/concept.h"
#include "std_e/future/constexpr_vector.h"
#include "std_e/utils/concatenate.h"


namespace std_e {


// Max number of dimensions for multi-dimensional arrays
// Max tensor order
constexpr size_t Max_size_of_multi_index = 8;


/// cx_multi_index: Multi_index with known compile-time size and indices {
using cx_multi_index = constexpr_vector<int,Max_size_of_multi_index>;
/// cx_multi_index }


/// index_type_of Multi_index {
template<class Multi_index>
struct index_type_of__impl; // primary template

template<>
struct index_type_of__impl<cx_multi_index> {
  using type = int;
};

template<class Multi_index> using index_type_of = typename index_type_of__impl<Multi_index>::type;
/// index_type_of Multi_index }


/// zero_index {
template<class Multi_index>
struct zero_index__impl;

template<> 
struct zero_index__impl<cx_multi_index> {
  static constexpr auto
  get(size_t rank) -> cx_multi_index {
    return cx_multi_index(rank); // already zero-initialized
  }
};

// TODO already defined
//template<class Multi_index> constexpr auto
//zero_index(size_t rank) -> Multi_index {
//  return zero_index__impl<Multi_index>::get(rank);
//}
/// zero_index }


/// utils {
template<class... Multi_index> constexpr auto
concatenate_indices(const Multi_index&... is) {
  size_t sz = (is.size() + ...);
  cx_multi_index cat_is(sz);
  concatenate_in(cat_is,is...);
  return cat_is;
}
/// utils }


} // std_e
