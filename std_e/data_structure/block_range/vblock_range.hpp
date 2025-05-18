#pragma once


#include "std_e/data_structure/block_range/vblock_iterator.hpp"


#include "std_e/data_structure/jagged_range.hpp"
#include "std_e/future/dynarray.hpp"


namespace std_e {


template<class T, class I>
using vblock_range = jagged_range<T,I,2>;

template<class T, class I>
using vblock_vector = jagged_vector<T,2,I>;

template<class T, class I, class A = mallocator, int rank=2>
using vblock_array = jagged_range<std_e::dynarray<T,A>,std_e::dynarray<I,A>,rank>;

template<class T, class I>
using vblock_span = vblock_range<span<T>,span<I>>;

template<class Rng, class Int_range> constexpr auto
view_as_vblock_range(Rng& values, Int_range& offsets) {
  //return vblock_range<Rng&,Int_range&>(values,offsets);
  using T = typename std::remove_cvref_t<Rng>::value_type;
  using I = typename std::remove_cvref_t<Int_range>::value_type;
  return vblock_span<T,I>(make_span(values),make_span(offsets));
}
template<class Rng, class Int_range> constexpr auto
view_as_vblock_range(const Rng& values, const Int_range& offsets) {
  //return vblock_range<Rng&,Int_range&>(values,offsets);
  using T = typename std::remove_cvref_t<Rng>::value_type;
  using I = typename std::remove_cvref_t<Int_range>::value_type;
  return vblock_span<const T,const I>(make_span(values),make_span(offsets));
}
template<class T, class I> constexpr auto
view_as_vblock_range(span<T> values, span<I> offsets) { // TODO proper overload with is_view
  return vblock_span<T,I>(values,offsets);
}
template<class T, class I> constexpr auto
view_as_vblock_range2(span<T> values, span<I> offsets) { // TODO proper overload with is_view
  return vblock_span<T,I>(values,offsets);
}

template<class TR, class IR> constexpr auto
vblock_vector_with_same_size(const vblock_range<TR,IR>& x) {
  using T = typename std::remove_cvref_t<TR>::value_type;
  using I = typename std::remove_cvref_t<IR>::value_type;

  return vblock_vector<T,I>(x.size(), x.total_size());
}


} // std_e
