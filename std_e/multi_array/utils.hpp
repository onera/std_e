#pragma once


#include "std_e/base/not_implemented_exception.hpp"
#include "std_e/multi_array/shape/dyn_shape.hpp"
#include "std_e/future/span.hpp"
#include "std_e/multi_array/multi_array/multi_array.hpp"
#include "std_e/multi_index/multi_index_range.hpp"


namespace std_e {


template<class Integer, int rank> inline auto
reshape(dyn_shape<Integer,rank>& x, const multi_index<Integer,rank>& dims) {
  using multi_index_type = typename dyn_shape<Integer,rank>::multi_index_type;
  STD_E_ASSERT(x.offset()==make_zero_multi_index<multi_index_type>(x.offset().size()));
  x.extent() = dims;
  x.offset() = make_zero_multi_index<multi_index_type>(dims.size());
}

template<class Dynamic_container, class Integer> auto
resize_memory(Dynamic_container& x, Integer n) -> void {
  x.resize(n);
}
template<class T, ptrdiff_t N, class Integer> auto
resize_memory(span<T,N>& x, Integer n) -> void {
  STD_E_ASSERT((Integer)x.size()>=n);
}

template<class R, class Shape> auto
cons_reshape(multi_array<R,Shape>& x, const typename Shape::multi_index_type& dims) {
  auto n = std_e::cartesian_product_size(dims);
  STD_E_ASSERT(x.size()==n);
  reshape(x.shape(),dims);
}
template<class R, class Shape> auto
reshape(multi_array<R,Shape>& x, const typename Shape::multi_index_type& dims) {
  reshape(x.shape(),dims);
  auto total_size = std_e::cartesian_product_size(dims);
  resize_memory(x.underlying_range(),total_size);
}


template<class R, class Shape> auto
is_empty(const multi_array<R,Shape>& x) -> bool {
  return x.size()==0;
}


} // std_e
