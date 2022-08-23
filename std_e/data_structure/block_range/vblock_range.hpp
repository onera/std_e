#pragma once


#include "std_e/data_structure/block_range/vblock_iterator.hpp"


#include "std_e/data_structure/jagged_range.hpp"


namespace std_e {


template<class T, class I>
using vblock_range = jagged_range<T,I,2>;

template<class T, class I>
using vblock_vector = jagged_vector<T,2,I>;

template<class Rng, class Int_range> constexpr auto
view_as_vblock_range(Rng& values, Int_range& offsets) {
  return vblock_range<Rng&,Int_range&>(values,offsets);
}
template<class T, class I> constexpr auto
view_as_vblock_range2(span<T> values, span<I> offsets) { // TODO proper overload with is_view
  return vblock_range<span<T>,span<I>>(values,offsets);
}

template<class TR, class IR> constexpr auto
vblock_vector_with_same_size(const vblock_range<TR,IR>& x) {
  using T = typename std::remove_cvref_t<TR>::value_type;
  using I = typename std::remove_cvref_t<IR>::value_type;

  return vblock_vector<T,I>(x.size(), x.total_size());
}


} // std_e
