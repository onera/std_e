#pragma once


#include "std_e/multi_index/concept.hpp"
#include "std_e/future/constexpr_vector.hpp"
#include "std_e/utils/concatenate.hpp"
#include "std_e/utils/array.hpp"


namespace std_e {


// Max number of dimensions for multi-dimensional arrays
// Max tensor order
constexpr size_t Max_size_of_multi_index = 8;


/// cx_multi_index: Multi_index with known compile-time size and indices {
using cx_multi_index = constexpr_vector<int,Max_size_of_multi_index>;
/// cx_multi_index }


/// utils {
template<class... Multi_index> constexpr auto
concatenate_indices(const Multi_index&... is) {
  size_t sz = (is.size() + ...);
  auto cat_is = make_array_of_size<concatenated_array<Multi_index...>>(sz);
  concatenate_in(cat_is,is...);
  return cat_is;
}
template<class... Multi_index> constexpr auto
concatenate_into_cx_multi_index(const Multi_index&... is) {
  size_t sz = (is.size() + ...);
  auto cat_is = cx_multi_index(sz);
  concatenate_in(cat_is,is...);
  return cat_is;
}
/// utils }


} // std_e
