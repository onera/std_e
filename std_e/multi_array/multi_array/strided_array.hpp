#pragma once


#include "std_e/multi_array/multi_array/multi_array.hpp"
#include "std_e/data_structure/strided_span.hpp"
#include <iostream>


namespace std_e {


template<class Multi_array_0, class Multi_array_1, class Multi_array_2> constexpr auto
origin_indices(const Multi_array_0& fixed_dim_indices, const Multi_array_1& fixed_indices, const Multi_array_2& indices) {
  constexpr int fixed_rank = rank_of<Multi_array_0>;
  constexpr int rank = rank_of<Multi_array_1>;
  constexpr int origin_rank = fixed_rank + rank;
  using index_type = index_type_of<Multi_array_1>;
  multi_index<index_type,origin_rank> origin_is = {};
  int k0=0;
  int k1=0;
  for (int i=0; i<origin_rank; ++i) {
    if (k0<fixed_rank && i==fixed_dim_indices[k0]) {
      origin_is[i] = fixed_indices[k0];
      ++k0;
    } else {
      origin_is[i] = indices[k1];
      ++k1;
    }
  }
  return origin_is;
}
  

template<class T, class Multi_array_shape, class Multi_array_shape_origin>
class strided_array : private Multi_array_shape {
  public:
  // type traits
    using base = Multi_array_shape;
    using shape_type = Multi_array_shape;
    using origin_shape_type = Multi_array_shape_origin;

    static constexpr int ct_rank = shape_type::ct_rank;
    static constexpr int ct_origin_rank = origin_shape_type::ct_rank;
    static constexpr int ct_fixed_rank = ct_origin_rank-ct_rank;
    using index_type = typename origin_shape_type::index_type;
    using multi_index_type = typename shape_type::multi_index_type;

    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    static constexpr bool mem_is_owned = false;

  // ctor
    template<class Multi_array, class Multi_index> constexpr
    // requires Multi_array::value_type==T
    // requires Multi_array::shape_type==Multi_array_shape,
    // requires rank = Multi_array::rank - rank_of<Multi_index>
    strided_array(Multi_array&& x, multi_index<int,ct_fixed_rank> fixed_dim_is, Multi_index fixed_is)
      : shape_type(make_sub_shape(x.shape(),fixed_dim_is))
      , ptr(x.data())
      , origin_shape(x.shape())
      , fixed_dim_indices(std::move(fixed_dim_is))
      , fixed_indices(std::move(fixed_is))
    {}

  // low-level
    FORCE_INLINE constexpr auto
    shape() const -> const shape_type& {
      return *this;
    }
    FORCE_INLINE constexpr auto
    shape() -> shape_type& {
      return *this;
    }
    FORCE_INLINE auto
    data() -> pointer {
      return ptr;
    }
    FORCE_INLINE constexpr auto
    data() const -> const_pointer {
      return ptr;
    }

  // dimensions
    // Note: there is a trick here:
    // These functions can be static (depending on type of Multi_array_shape).
    // In the case they are, they can be called
    //   - as non-static functions (C++ allows that)
    //   - as static functions (useful if a constexpr value is needed)
    using base::rank;
    using base::extent;
    using base::offset;
    constexpr auto
    size() const -> index_type {
      return cartesian_product(this->extent());
    }

  // element access
    template<class... Ts> FORCE_INLINE constexpr auto
    operator()(Ts&&... xs) const -> const_reference {
      return ptr[linear_index(std::forward<Ts>(xs)...)];
    }
    template<class... Ts> FORCE_INLINE constexpr auto
    operator()(Ts&&... xs) -> reference {
      return ptr[linear_index(std::forward<Ts>(xs)...)];
    }

  private:
  /// linear_index {
  // from Multi_index
    template<class Multi_index> FORCE_INLINE constexpr auto
    // requires Multi_index is an array && Multi_index::size()==rank()
    linear_index(const Multi_index& indices) const -> index_type {
      auto origin_is = origin_indices(fixed_dim_indices,fixed_indices,indices);
      return fortran_order_from_dimensions(origin_shape.extent(),origin_shape.offset(),origin_is);
    }
  // from indices
    template<class Integer, class... Integers> FORCE_INLINE constexpr auto
    // 1 + sizeof...(Integers)==rank()
    linear_index(Integer i, Integers... is) const -> index_type {
      STD_E_ASSERT(1+sizeof...(Integers)==rank()); // the number of indices must be the array rank
      return linear_index(multi_index_type{i,is...});
    }
    template<class... ints> FORCE_INLINE constexpr auto
    // requires ints are integers && sizeof...(ints)==rank()
    linear_index(int i, ints... is) const -> index_type { // Note: same as above, but forcing first arg
                                                          // to be an integer for unsurprising overload resolution
      STD_E_ASSERT(1+sizeof...(ints)==rank()); // the number of indices must be the array rank
      return linear_index(multi_index_type{i,is...});
    }
    FORCE_INLINE constexpr auto
    // requires ints are integers && sizeof...(ints)==rank()
    linear_index() const -> index_type { // Note: rank 0 case
      return 0;
    }
  /// linear_index }

// data members
    T* ptr;
    origin_shape_type origin_shape;
    multi_index<int,ct_fixed_rank> fixed_dim_indices;
    multi_index_type fixed_indices;
};


// TODO strided array of strided_array
// make_strided_array {
template<
  class Multi_array, class Multi_index_0, class Multi_index_1,
  std::enable_if_t< !std::is_same_v<Multi_index_0,int> , int > =0
> constexpr auto
// requires Multi_array::shape_type is dyn_shape
// requires Multi_array of fixed rank
make_strided_array(Multi_array&& x, Multi_index_0 fixed_dim_indices, Multi_index_1 fixed_is) {
  using T = typename std::decay_t<Multi_array>::value_type;
  using origin_shape_type = typename std::decay_t<Multi_array>::shape_type;
  constexpr int origin_rank = std::decay_t<Multi_array>::rank();
  constexpr int fixed_dims_rank = rank_of<Multi_index_1>;
  constexpr int rank = origin_rank - fixed_dims_rank;
  using index_type = typename origin_shape_type::index_type;
  using shape_type = dyn_shape<index_type,rank>;
  return strided_array<T,shape_type,origin_shape_type>(x,std::move(fixed_dim_indices),std::move(fixed_is));
}
template<class Multi_array, class I> constexpr auto
// requires Multi_array::shape_type is dyn_shape
// requires Multi_array of fixed rank
make_strided_array(Multi_array&& x, int fixed_dim_index, I fixed_index) {
  return make_strided_array(x,multi_index<int,1>{fixed_dim_index},multi_index<I,1>{fixed_index});
}


template<
  int... fixed_dim_is, class Multi_array, class Multi_index,
  std::enable_if_t< !std::is_integral_v<Multi_index> , int > =0
> constexpr auto
// requires Multi_array::shape_type is dyn_shape
// requires Multi_array of fixed rank
make_strided_array(Multi_array&& x, Multi_index fixed_is) {
  constexpr int fixed_rank = sizeof...(fixed_dim_is);
  // Here, just delegate to run-time version
  // TODO compile-time version
  multi_index<int,fixed_rank> fixed_dim_indices = {fixed_dim_is...};
  return make_strided_array(x,fixed_dim_indices,std::move(fixed_is));
}
template<
  int fixed_dim_index, class Multi_array, class I,
  std::enable_if_t< std::is_integral_v<I> , int > =0
> constexpr auto
// requires Multi_array::shape_type is dyn_shape
// requires Multi_array of fixed rank
make_strided_array(Multi_array&& x, I fixed_index) {
  return make_strided_array<fixed_dim_index>(x,multi_index<I,1>{fixed_index});
}
// make_strided_array }


// strided_span {
template<int varying_dim_index, class Multi_array, class Multi_index> constexpr auto
// requires Multi_array::shape_type is dyn_shape
// requires Multi_array of fixed rank
// requires Multi_array::ct_rank = rank_of<Multi_index> + 1
make_strided_span__impl(Multi_array&& x, const Multi_index& fixed_dim_indices) {
  // traits
  using I = index_type_of<Multi_index>;
  constexpr int rank = std::decay_t<Multi_array>::ct_rank;

  // strides
  I sz = x.extent(varying_dim_index);
  auto strides = fortran_strides_from_extent2(x.extent()); // TODO keep fortran-order assuption private to class
  I str_len = strides[varying_dim_index];

  // offset
  multi_index<I,rank> offsets = {};
  for (int i=0;      i<varying_dim_index; ++i) { offsets[i] = fixed_dim_indices[i]; }
  offsets[varying_dim_index] = 0;
  for (int i=varying_dim_index+1; i<rank; ++i) { offsets[i] = fixed_dim_indices[i-1]; }

  I offset = 0;
  for (int i=0; i<rank; ++i) {
    offset += offsets[i] * strides[i];
  }

  // create strided span
  return make_strided_span(x.data()+offset,sz,str_len);
}

template<
  int varying_dim_index, class M0, class M1, class Multi_index,
  std::enable_if_t< !std::is_integral_v<Multi_index> , int > =0
> constexpr auto
// requires Multi_array::shape_type is dyn_shape
// requires Multi_array of fixed rank
// requires Multi_array::ct_rank = rank_of<Multi_index> + 1
make_strided_span(multi_array<M0,M1>& x, const Multi_index& fixed_dim_indices) {
  return make_strided_span__impl<varying_dim_index>(x,fixed_dim_indices);
}
template<
  int varying_dim_index, class M0, class M1, class Multi_index,
  std::enable_if_t< !std::is_integral_v<Multi_index> , int > =0
> constexpr auto
// requires Multi_array::shape_type is dyn_shape
// requires Multi_array of fixed rank
// requires Multi_array::ct_rank = rank_of<Multi_index> + 1
make_strided_span(const multi_array<M0,M1>& x, const Multi_index& fixed_dim_indices) {
  return make_strided_span__impl<varying_dim_index>(x,fixed_dim_indices);
}


template<
  class M0, class M1, class I,
  std::enable_if_t< std::is_integral_v<I> , int > =0
> constexpr auto
// requires Multi_array::shape_type is dyn_shape
// requires Multi_array of fixed rank
make_strided_span(multi_array<M0,M1>& x, I fixed_index) {
  return make_strided_span__impl<1>(x,multi_index<I,1>{fixed_index});
}
template<
  class M0, class M1, class I,
  std::enable_if_t< std::is_integral_v<I> , int > =0
> constexpr auto
// requires Multi_array::shape_type is dyn_shape
// requires Multi_array of fixed rank
make_strided_span(const multi_array<M0,M1>& x, I fixed_index) {
  return make_strided_span__impl<1>(x,multi_index<I,1>{fixed_index});
}
// strided_span }


} // std_e
