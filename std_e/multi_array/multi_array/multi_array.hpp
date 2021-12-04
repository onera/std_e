#pragma once


#include "std_e/base/macros.hpp"
#include "std_e/multi_array/shape/concept.hpp"
#include "std_e/multi_index/concept.hpp"
#include "std_e/multi_index/multi_index.hpp"
#include "std_e/multi_index/fortran_order.hpp"
#include "std_e/future/contract.hpp"
#include "std_e/multi_array/multi_array/concept.hpp"
#include "std_e/multi_index/cartesian_product_size.hpp"
#include "std_e/multi_array/shape/fixed_dyn_shape_common.hpp"
#include "std_e/memory_ressource/memory_ressource.hpp"
#include "std_e/future/span.hpp"
#include "std_e/meta/type_traits.hpp"


namespace std_e {


template<class Random_access_range, class Multi_array_shape>
class multi_array : private Multi_array_shape {
  public:
  // type traits
    using base = Multi_array_shape;
    using underlying_range_type = Random_access_range;
    using shape_type = Multi_array_shape;
    using index_type = typename shape_type::index_type;
    using multi_index_type = typename shape_type::multi_index_type;

    using value_type = typename underlying_range_type::value_type;
    using pointer = typename underlying_range_type::pointer;
    using const_pointer = typename underlying_range_type::const_pointer;
    using reference = typename underlying_range_type::reference;
    using const_reference = typename underlying_range_type::const_reference;

    static constexpr int ct_rank = shape_type::ct_rank;
    static constexpr int ct_size = shape_type::ct_size;

    static constexpr bool mem_is_owned = memory_is_owned<underlying_range_type>;

  // constructors {
    FORCE_INLINE constexpr multi_array() = default;
    FORCE_INLINE constexpr multi_array(const multi_array& ) = default;
    FORCE_INLINE constexpr multi_array(      multi_array&&) = default;
    FORCE_INLINE constexpr multi_array& operator=(const multi_array& ) = default;
    FORCE_INLINE constexpr multi_array& operator=(      multi_array&&) = default;

  /// low-level {
    FORCE_INLINE constexpr
    multi_array(underlying_range_type rng, shape_type sh)
      : shape_type(std::move(sh))
      , rng(std::move(rng))
    {}

    template<class... Integers,
      std::enable_if_t<are_integral<Integers...>,int> =0
    >
    multi_array(underlying_range_type rng, Integers... dims)
      : shape_type({dims...})
      , rng(std::move(rng))
    {
      static_assert(ct_rank==dynamic_size || sizeof...(Integers)==ct_rank);
    }

    FORCE_INLINE constexpr
    multi_array(underlying_range_type rng)
      : multi_array(std::move(rng),{})
    {}
  /// low-level }

  /// multi_array-view: conversion from non-const to const
    template<class T0, ptrdiff_t N> FORCE_INLINE constexpr
    multi_array(const multi_array<span<T0,N>,shape_type>& ma)
      : shape_type(ma.shape())
      , rng(ma.underlying_range())
    {}

  /// ctors with dimensions {
    template<class... Integers,
      std::enable_if_t<are_integral<Integers...>,int> =0
    >
    multi_array(Integers... dims)
      : shape_type({dims...})
      , rng(std_e::cartesian_product_size(multi_index<int,sizeof...(Integers)>{dims...}))
    {
      static_assert(ct_rank==dynamic_size || sizeof...(Integers)==ct_rank);
    }

    FORCE_INLINE constexpr
    multi_array(multi_index_type dims)
      : shape_type({std::move(dims)})
      , rng(size())
    {}
    FORCE_INLINE constexpr
    multi_array(value_type* rng, multi_index_type dims)
      : shape_type({std::move(dims)})
      , rng(make_span(rng,size()))
    {}
  /// ctors with dimensions }

  /// ctors from initializer lists {
    //// ctors for owning memory
    // NOTE:
    //   the ctor is templated by T with T required to by of type value_type
    //   its seems that using a non-templated version using T=value_type would be simpler and do the same
    //   however, with the template, multi_array[I4,2]{{0},{1}} will work and choose the 2D ctor
    //   whereas without the template, the compiler will say that the 1D and 1D ctors are ambigous
    template<class T>
      // requires std::is_same_v<T,value_type>
    multi_array(std::initializer_list<T>&& l)
      : multi_array(l,make_array_of_size<underlying_range_type>(l.size()))
    {}
    template<class T>
      // requires std::is_same_v<T,value_type>
    multi_array(std::initializer_list<std::initializer_list<T>>&& ll)
      : multi_array(ll,make_array_of_size<underlying_range_type>(ll.size() * std::begin(ll)->size()))
    {}

    //// ctors for non-owning memory
    multi_array(std::initializer_list<value_type>&& l, value_type* ptr)
      : multi_array(l,span<value_type>(ptr,l.size()))
    {}
    multi_array(std::initializer_list<std::initializer_list<value_type>>&& ll, value_type* ptr)
      : multi_array(ll,span<value_type>(ptr,ll.size() * std::begin(ll)->size()))
    {}
  /// ctors from initializer lists }
  // constructors }

  // shape interface
    FORCE_INLINE constexpr auto shape() const -> const shape_type& { return *this; }
    FORCE_INLINE constexpr auto shape()       ->       shape_type& { return *this; }

    // Note: there is a trick here:
    // These functions can be static (depending on type of Multi_array_shape).
    // In the case they are, they can be called
    //   - as non-static functions (C++ allows that)
    //   - as static functions (useful if a constexpr value is needed)
    using base::rank;
    using base::extent;
    using base::offset;
    using base::size;

  // contiguous range interface
    FORCE_INLINE constexpr auto underlying_range() const -> const underlying_range_type& { return rng; }
    FORCE_INLINE constexpr auto underlying_range()       ->       underlying_range_type& { return rng; }

    FORCE_INLINE constexpr auto data() const  -> const_pointer { return rng.data(); }
    FORCE_INLINE constexpr auto data()        ->       pointer { return rng.data(); }

    FORCE_INLINE constexpr auto begin() const -> const_pointer { return data();        }
    FORCE_INLINE constexpr auto begin()       ->       pointer { return data();        }
    FORCE_INLINE constexpr auto end()   const -> const_pointer { return data()+size(); }
    FORCE_INLINE constexpr auto end()         ->       pointer { return data()+size(); }

    FORCE_INLINE constexpr auto operator[](index_type i) const -> const_reference { return rng[i]; }
    FORCE_INLINE constexpr auto operator[](index_type i)       ->       reference { return rng[i]; }

  // element access
    template<class... Ts> FORCE_INLINE constexpr auto
    operator()(Ts&&... xs) const -> const_reference {
      return rng[linear_index(std::forward<Ts>(xs)...)];
    }
    template<class... Ts> FORCE_INLINE constexpr auto
    operator()(Ts&&... xs) -> reference {
      return rng[linear_index(std::forward<Ts>(xs)...)];
    }

  private:
  // member functions
    // linear_index {
    /// from Multi_index
    template<
      class MI
      #if __cplusplus <= 201703L
      , std::enable_if_t< is_multi_index<MI> , int > =0
      #endif
    >
      #if __cplusplus > 201703L
        requires Multi_index<MI>
      #endif
    FORCE_INLINE constexpr auto
    linear_index(const MI& indices) const -> index_type {
      STD_E_ASSERT((int)indices.size()==rank());
      return fortran_order_from_dimensions(extent(),offset(),indices);
    }
    /// from indices
    template<
      class... Integers
      #if __cplusplus <= 201703L
      , std::enable_if_t< are_integral<Integers...> , int > =0
      #endif
    >
      #if __cplusplus > 201703L
        requires are_integral<Integers...>
      #endif
    FORCE_INLINE constexpr auto
    linear_index(Integers... is) const -> index_type {
      STD_E_ASSERT(sizeof...(Integers)==rank());
      return linear_index(multi_index_type{is...});
    }
    FORCE_INLINE constexpr auto
    linear_index() const -> index_type { // Note: rank 0 case // TODO why fortran_order_from_dimensions does not return 0?
      return 0;
    }
    // linear_index }

    // initialization list ctors {
    multi_array(std::initializer_list<value_type> l, underlying_range_type rng)
      : shape_type(make_shape<shape_type>({index_type(l.size())},{0}))
      , rng(std::move(rng))
    {
      STD_E_ASSERT(this->rank()==1);
      index_type i=0;
      for (const value_type& x : l) {
        (*this)(i) = x;
        ++i;
      }
    }
    multi_array(std::initializer_list<std::initializer_list<value_type>> ll, underlying_range_type rng)
      : shape_type(make_shape<shape_type>({index_type(ll.size()),index_type(std::begin(ll)->size())},{0,0}))
      , rng(std::move(rng))
    {
      STD_E_ASSERT(this->rank()==2);
      index_type i=0;
      for (const auto& l : ll) {
        index_type j=0;
        for (const value_type& x : l) {
          (*this)(i,j) = x;
          ++j;
        }
        ++i;
      }
    }
    // initialization list ctors }

  // data members
    underlying_range_type rng;
};

template<class M00, class M01, class M10, class M11> constexpr auto
operator==(const multi_array<M00,M01>& x, const multi_array<M10,M11>& y) -> bool {
  if (x.extent()!=y.extent()) return false;
  return x.underlying_range() == y.underlying_range();
}
template<class M00, class M01, class M10, class M11> constexpr auto
operator!=(const multi_array<M00,M01>& x, const multi_array<M10,M11>& y) -> bool {
  return !(x==y);
}
template<class M00, class M01, class M10, class M11> constexpr auto
same_data(const multi_array<M00,M01>& x, const multi_array<M10,M11>& y) -> bool {
  return
      x.extent()==y.extent()
   && x.data()==y.data();
}
template<class M0, class M1> FORCE_INLINE constexpr auto begin(const multi_array<M0,M1>& x) { return x.begin(); }
template<class M0, class M1> FORCE_INLINE constexpr auto begin(      multi_array<M0,M1>& x) { return x.begin(); }
template<class M0, class M1> FORCE_INLINE constexpr auto end  (const multi_array<M0,M1>& x) { return x.end()  ; }
template<class M0, class M1> FORCE_INLINE constexpr auto end  (      multi_array<M0,M1>& x) { return x.end()  ; }


// conversion to the view type (useful for function arguments)
/// Warning making a view is NOT free: it copies the shape
template<class M0, class M1> auto
make_view(multi_array<M0,M1>& x) {
  using value_type = typename multi_array<M0,M1>::value_type;
  constexpr int ct_size = multi_array<M0,M1>::ct_size;
  using mem_view_type = span<value_type,ct_size>;
  return multi_array{ mem_view_type{x.data(),x.size()} , x.shape() };
}
template<class M0, class M1> auto
make_view(const multi_array<M0,M1>& x) {
  using value_type = typename multi_array<M0,M1>::value_type;
  constexpr int ct_size = multi_array<M0,M1>::ct_size;
  using mem_view_type = span<const value_type,ct_size>;
  return multi_array{ mem_view_type{x.data(),x.size()} , x.shape() };
}

// sub views of contiguous memory {
// sub-view of contiguous memory are possible for a fortran-ordered memory
// if the last indices are fixed (i.e. the indices at the right)
template<
  class M0, class M1, class Multi_index,
  std::enable_if_t< is_multi_index<Multi_index> , int > =0
> auto
make_sub_array(multi_array<M0,M1>& x, const Multi_index& right_indices) {
  auto [index_1d,sub_shape] = shape_restriction_start_index_1d(x.shape(),right_indices);
  auto sub_sz = sub_shape.size();
  return multi_array{ make_span(x.data()+index_1d,sub_sz) , std::move(sub_shape) };
}
template<
  class M0, class M1, class Multi_index,
  std::enable_if_t< is_multi_index<Multi_index> , int > =0
> auto
make_sub_array(const multi_array<M0,M1>& x, const Multi_index& right_indices) {
  auto [index_1d,sub_shape] = shape_restriction_start_index_1d(x.shape(),right_indices);
  auto sub_sz = sub_shape.size();
  return multi_array{ make_span(x.data()+index_1d,sub_sz) , std::move(sub_shape) };
}
// overloads matching initialization lists {
template<
  class M0, class M1, int fixed_rank,
  class index_type = typename M1::index_type
> auto
make_sub_array(multi_array<M0,M1>& x, const index_type(&right_indices)[fixed_rank]) {
  multi_index<index_type,fixed_rank> right_is = {};  std::copy_n(right_indices,fixed_rank,begin(right_is));
  return make_sub_array(x,right_is);
}
template<
  class M0, class M1, int fixed_rank,
  class index_type = typename M1::index_type
> auto
make_sub_array(const multi_array<M0,M1>& x, const index_type(&right_indices)[fixed_rank]) {
  multi_index<index_type,fixed_rank> right_is = {};  std::copy_n(right_indices,fixed_rank,begin(right_is));
  return make_sub_array(x,right_is);
}
// overloads matching initialization lists }

template<
  class M0, class M1,
  class index_type = typename M1::index_type,
  int rank = M1::rank()
> auto
make_span(multi_array<M0,M1>& x, const multi_index<index_type,rank-1>& right_indices) {
  auto [index_1d,_] = shape_restriction_start_index_1d(x.shape(),right_indices);
  return make_span( x.data()+index_1d, x.extent(0) );
}
template<
  class M0, class M1,
  class index_type = typename M1::index_type,
  int rank = M1::rank()
> auto
make_span(const multi_array<M0,M1>& x, const multi_index<index_type,rank-1>& right_indices) {
  auto [index_1d,_] = shape_restriction_start_index_1d(x.shape(),right_indices);
  return make_span( x.data()+index_1d, x.extent(0) );
}

/// special case of fixing only one index {
template<
  class M0, class M1, class I,
  std::enable_if_t< std::is_integral_v<I> , int > =0
> auto
make_sub_array(multi_array<M0,M1>& x, I i) {
  return make_sub_array(x,std_e::multi_index<int,1>{i});
}
template<
  class M0, class M1, class I,
  std::enable_if_t< std::is_integral_v<I> , int > =0
> auto
make_sub_array(const multi_array<M0,M1>& x, I i) {
  return make_sub_array(x,std_e::multi_index<int,1>{i});
}
template<
  class M0, class M1, class I,
  std::enable_if_t< M1::rank()==2 , int > =0,
  std::enable_if_t< std::is_integral_v<I> , int > =0
> constexpr auto
make_span(multi_array<M0,M1>& x, I j) {
  return make_span(x,std_e::multi_index<I,1>{j});
}
template<
  class M0, class M1, class I,
  std::enable_if_t< M1::rank()==2 , int > =0,
  std::enable_if_t< std::is_integral_v<I> , int > =0
> constexpr auto
make_span(const multi_array<M0,M1>& x, I j) {
  return make_span(x,std_e::multi_index<I,1>{j});
}
/// special case of fixing only one index }


/// special case of 2D arrays {
template<
  class M0, class M1,
  std::enable_if_t< M1::rank()==2 , int > =0
> constexpr auto
column(multi_array<M0,M1>& x, typename multi_array<M0,M1>::index_type j) {
  return make_span(x,j);
}
template<
  class M0, class M1,
  std::enable_if_t< M1::rank()==2 , int > =0
> constexpr auto
column(const multi_array<M0,M1>& x, typename multi_array<M0,M1>::index_type j) {
  return make_span(x,j);
}
/// special case of 2D arrays }
// sub views of contiguous memory }


} // std_e
