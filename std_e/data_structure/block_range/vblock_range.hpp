#pragma once


#include "std_e/data_structure/block_range/vblock_iterator.hpp"


#include "std_e/data_structure/jagged_range.hpp"


namespace std_e {


template<class T, class I>
using vblock_range = jagged_range<T,I,2>;

template<class Rng, class Int_range> constexpr auto
view_as_vblock_range(Rng& values, Int_range& offsets) {
  return vblock_range<Rng&,Int_range&>(values,offsets);
}
template<class T, class I> constexpr auto
view_as_vblock_range2(span<T> values, span<I> offsets) { // TODO proper overload with is_view
  return vblock_range<span<T>,span<I>>(values,offsets);
}


} // std_e
