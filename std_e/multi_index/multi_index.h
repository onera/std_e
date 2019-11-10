#pragma once


#include "std_e/multi_index/concept.h"
#include "std_e/utils/array.h"
#include <vector>


namespace std_e {


constexpr size_t Dynamic_size = -1;


// multi_index: Multi_index with known compile-time size {
template<class Integer, size_t N> using multi_index = std::array<Integer,N>;
template<class Integer, size_t N> constexpr multi_index<Integer,N> zero_multi_index = default_array<Integer,N>;
// }

// dyn_multi_index: Multi_index with unknown compile-time size {
template<class Integer> using dyn_multi_index = std::vector<Integer>;
// }


// index_type_of Multi_index {
template<class Multi_index>
struct index_type_of__impl; // primary template

template<class Integer, size_t N>
struct index_type_of__impl<multi_index<Integer,N>> {
  using type = Integer;
};

template<class Integer>
struct index_type_of__impl<dyn_multi_index<Integer>> {
  using type = Integer;
};

template<class Multi_index> using index_type_of = typename index_type_of__impl<Multi_index>::type;
// index_type_of Multi_index }


// zero_index {
template<class Multi_index>
struct zero_index__impl;

template<class Integer> 
struct zero_index__impl<dyn_multi_index<Integer>> {
  static constexpr auto
  get(size_t rank) -> dyn_multi_index<Integer> {
    dyn_multi_index<Integer> zero(rank);
    std::fill(begin(zero),end(zero),0);
    return zero;
  }
};

template<class Multi_index> constexpr auto
zero_index(size_t rank) -> Multi_index {
  return zero_index__impl<Multi_index>::get(rank);
}
// zero_index }


// utils {
template<class Multi_index>
struct create_multi_index__impl {
  static constexpr auto
  get(size_t n) {
    return Multi_index(n);
  }
};
template<class Integer, size_t N>
struct create_multi_index__impl<multi_index<Integer,N>> {
  static constexpr auto
  get(size_t n) {
    return multi_index<Integer,N>{}; // no need to create since compile-time size
  }
};
/**
  Generic method to create Multi_index from size
    - if Multi_index==multi_index==array, need to call default ctor
    - if Multi_index!=multi_index==array, need to call ctor with size argument
*/
template<class Multi_index> constexpr auto
create_multi_index(size_t n) -> Multi_index {
  return create_multi_index__impl<Multi_index>::get(n);
}


template<class Integer, size_t... Ns> constexpr auto
concatenate_indices(const multi_index<Integer,Ns>&... is) {
  constexpr size_t sz = (Ns + ...);
  multi_index<Integer,sz> cat_is = {};
  concatenate_in(cat_is,is...);
  return cat_is;
}
// utils }


} // std_e
