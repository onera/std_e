#pragma once


#include "std_e/multi_array/shape/fixed_shape.hpp"
#include "std_e/multi_array/shape/dyn_shape.hpp"
#include "std_e/utils/concatenate.hpp"


// When calling a Shape ctor in a generic context,
// we provide all the dyn_shape arguments.
// make_shape provides a uniform interface for fixed_/dyn_shape:
// It always takes the args and
//  - forwards them for the dyn_shape ctor
//  - removes them for the fixed_shape ctor


namespace std_e {


// make_shape {
template<class Shape>
struct make_shape__impl;


template<int... dims>
struct make_shape__impl<fixed_shape<dims...>> {
  template<class Multi_index> FORCE_INLINE static constexpr auto
  func(Multi_index&& ext, Multi_index&&) -> fixed_shape<dims...> {
    STD_E_ASSERT(ext==fixed_shape<dims...>::fixed_extent);
    return {};
  }
};

template<class Integer, int N>
struct make_shape__impl<dyn_shape<Integer,N>> {
  template<class Multi_index> FORCE_INLINE static constexpr auto
  func(Multi_index&& ext, Multi_index&& off) -> dyn_shape<Integer,N> {
    return dyn_shape<Integer,N>(FWD(ext),FWD(off));
  }
};

template<class Shape, class Multi_index = multi_index<typename Shape::index_type,Shape::ct_rank>> constexpr auto
make_shape(Multi_index&& ext, Multi_index&& off) -> Shape {
  return make_shape__impl<std::decay_t<Shape>>::func(FWD(ext),FWD(off));
}
// make_shape }


template<class Multi_array_shape, class Multi_index> auto
shape_restriction_start_index_1d(const Multi_array_shape& x, const Multi_index& right_indices) {
  using index_type = index_type_of<Multi_index>;

  constexpr int rank = Multi_array_shape::rank();
  static_assert(rank != dynamic_size);
  constexpr int restriction_rank = rank_of<Multi_index>;
  constexpr int sub_rank = rank - restriction_rank;

  auto sub_view_start_index = concatenate(multi_index<index_type,sub_rank>{0},right_indices);
  auto index_1d = fortran_order_from_dimensions(x.extent(),x.offset(),sub_view_start_index);

  auto sub_shape = make_sub_shape<restriction_rank>(x);
  return std::make_pair(index_1d,std::move(sub_shape));
}


} // std_e
