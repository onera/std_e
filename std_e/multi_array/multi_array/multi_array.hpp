#pragma once


#include "std_e/base/macros.hpp"
#include "std_e/memory_ressource/concept.hpp"
#include "std_e/multi_array/shape/concept.hpp"
#include "std_e/multi_index/concept.hpp"
#include "std_e/multi_index/multi_index.hpp"
#include "std_e/multi_index/fortran_order.hpp"
#include "std_e/future/contract.hpp"
#include "std_e/multi_array/multi_array/concept.hpp"
#include "std_e/multi_index/cartesian_product.hpp"
#include "std_e/multi_array/shape/fixed_dyn_shape_common.hpp"
#include "std_e/memory_ressource/memory_ressource.hpp"
#include "std_e/future/span.hpp"


namespace std_e {


template<class Memory_ressource, class Multi_array_shape>
class multi_array : private Multi_array_shape
{
  public:
  // type traits
    using base = Multi_array_shape;
    using memory_ressource_type = Memory_ressource;
    using shape_type = Multi_array_shape;
    using index_type = typename shape_type::index_type;

    using value_type = typename memory_ressource_type::value_type;
    using pointer = typename memory_ressource_type::pointer;
    using const_pointer = typename memory_ressource_type::const_pointer;
    using reference = typename memory_ressource_type::reference;
    using const_reference = typename memory_ressource_type::const_reference;

    static constexpr int ct_rank = shape_type::ct_rank;
    using multi_index_type = typename shape_type::multi_index_type;

    static constexpr bool mem_is_owned = memory_is_owned<memory_ressource_type>;

  // constructors {
    FORCE_INLINE constexpr multi_array() = default;

    FORCE_INLINE constexpr
    multi_array(memory_ressource_type mem, shape_type sh)
      : shape_type(std::move(sh))
      , mem(std::move(mem))
    {}
    FORCE_INLINE constexpr
    multi_array(memory_ressource_type mem)
      : multi_array(std::move(mem),{})
    {}

    FORCE_INLINE constexpr multi_array(const multi_array&) = default;
    FORCE_INLINE constexpr multi_array(multi_array&&) = default;
    // assignement: not implemented

    template<class T_ptr0> FORCE_INLINE constexpr // conversion from non-const to const
    multi_array(const multi_array<memory_view<T_ptr0>,shape_type>& ma)
      : shape_type(ma.shape())
      , mem(ma.memory())
    {}
    template<class T_ptr0> FORCE_INLINE constexpr // conversion from non-const to const
    multi_array(multi_array<memory_view<T_ptr0>,shape_type>&& ma)
      : shape_type(std::move(ma.shape()))
      , mem(ma.memory())
    {}

  /// ctor for owning memory {
    template<class T> static constexpr bool is_index_type = std::is_same_v<T,index_type>;
    template<class T> using is_index_type_t = std::bool_constant<is_index_type<T>>;
    template<class... ints,
      std::enable_if_t<std::conjunction_v<is_index_type_t<ints>...>,int> =0
    >
    multi_array(ints... dims)
      : shape_type({dims...})
      , mem(std_e::cartesian_product(multi_index<int,sizeof...(ints)>{dims...}))
    {
      static_assert(ct_rank==dynamic_size || sizeof...(ints)==ct_rank, "Initialization of multi_array with wrong number of dims");
    }
  /// ctor for owning memory }

  /// ctors from initializer lists {
    // ctor for rank==1, owning memory
    multi_array(std::initializer_list<value_type> l, memory_ressource_type mem)
      : shape_type(make_shape<shape_type>({index_type(l.size())},{0}))
      , mem(std::move(mem))
    {
      static_assert(mem_is_owned,"can't create a view from aggregate initialization (no memory!)");
      STD_E_ASSERT(this->rank()==1);
      index_type i=0;
      for (const value_type& x : l) {
        (*this)(i) = x;
        ++i;
      }
    }
    multi_array(std::initializer_list<value_type> l)
      : multi_array(l,make_array_of_size<memory_ressource_type>(l.size()))
    {}
    // ctor for rank==2, owning memory
    multi_array(std::initializer_list<std::initializer_list<value_type>> ll, memory_ressource_type mem)
      : shape_type(make_shape<shape_type>({index_type(ll.size()),index_type(std::begin(ll)->size())},{0,0}))
      , mem(std::move(mem))
    {
      static_assert(mem_is_owned,"can't create a view from aggregate initialization (no memory!)");
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
    multi_array(std::initializer_list<std::initializer_list<value_type>> ll)
      : multi_array(ll,make_array_of_size<memory_ressource_type>(ll.size() * std::begin(ll)->size()))
    {}
  /// ctors from initializer lists }
  // constructors }


  // low-level
    FORCE_INLINE constexpr auto
    shape() const -> const shape_type& {
      return *this;
    }
    FORCE_INLINE constexpr auto
    shape() -> shape_type& {
      return *this;
    }

    FORCE_INLINE constexpr auto
    memory() const -> const memory_ressource_type& {
      return mem;
    }
    FORCE_INLINE auto
    memory() -> memory_ressource_type& {
      return mem;
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

  // range interface
    FORCE_INLINE auto
    data() -> pointer {
      return mem.data();
    }
    FORCE_INLINE constexpr auto
    data() const -> const_pointer {
      return mem.data();
    }
    FORCE_INLINE auto
    begin() -> pointer {
      return data();
    }
    FORCE_INLINE constexpr auto
    begin() const -> const_pointer {
      return data();
    }
    FORCE_INLINE auto
    end() -> pointer {
      return begin()+size();
    }
    FORCE_INLINE constexpr auto
    end() const -> const_pointer {
      return begin()+size();
    }


  // element access
    template<class... Ts> FORCE_INLINE constexpr auto
    operator()(Ts&&... xs) const -> const_reference {
      return mem[linear_index(std::forward<Ts>(xs)...)];
    }
    template<class... Ts> FORCE_INLINE constexpr auto
    operator()(Ts&&... xs) -> reference {
      return mem[linear_index(std::forward<Ts>(xs)...)];
    }
    FORCE_INLINE constexpr auto
    operator[](index_type i) const -> const_reference {
      // Precondition: rank()==1
      return mem[i + offset(0)];
    }
    FORCE_INLINE constexpr auto
    operator[](index_type i) -> reference {
      // Precondition: rank()==1
      return mem[i + offset(0)];
    }

  private:
// member functions
  /// linear_index {
  // from Multi_index
    template<class Multi_index> FORCE_INLINE constexpr auto
    // requires Multi_index is an array && Multi_index::size()==rank()
    linear_index(const Multi_index& indices) const -> index_type {
      return fortran_order_from_dimensions(extent(),offset(),indices);
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
    memory_ressource_type mem;
};

template<class M00, class M01, class M10, class M11> constexpr auto
operator==(const multi_array<M00,M01>& x, const multi_array<M10,M11>& y) -> bool {
  if (x.size()!=y.size()) return false;
  if (x.size()==0) return true;
  auto mismatch_its = std::mismatch(x.begin(),x.end(),y.begin());
  return mismatch_its.first==x.end();
}
template<class M00, class M01, class M10, class M11> constexpr auto
operator!=(const multi_array<M00,M01>& x, const multi_array<M10,M11>& y) -> bool {
  return !(x==y);
}


// conversion to the view type (useful for function arguments)
/// Warning making a view is NOT free: it copies the shape
template<class M0, class M1> auto
make_view(multi_array<M0,M1>& x) {
  using pointer = typename multi_array<M0,M1>::pointer;
  using mem_view_type = memory_view<pointer>;
  return multi_array{ mem_view_type{x.data()} , x.shape() };
}
template<class M0, class M1> auto
make_view(const multi_array<M0,M1>& x) {
  using const_pointer = typename multi_array<M0,M1>::const_pointer;
  using mem_view_type = memory_view<const_pointer>;
  return multi_array{ mem_view_type{x.data()} , x.shape() };
}

// sub-view of contiguous memory are possible for a fortran-ordered memory
// if the last indices are fixed (i.e. the indices at the right)
template<class M0, class M1, class Multi_index> auto
make_sub_array(multi_array<M0,M1>& x, const Multi_index& right_indices) {
  auto [index_1d,sub_shape] = shape_restriction_start_index_1d(x.shape(),right_indices);
  return multi_array{ memory_view{x.data()+index_1d} , std::move(sub_shape) };
}
template<class M0, class M1, class Multi_index> auto
make_sub_array(const multi_array<M0,M1>& x, const Multi_index& right_indices) {
  auto [index_1d,sub_shape] = shape_restriction_start_index_1d(x.shape(),right_indices);
  return multi_array{ memory_view{x.data()+index_1d} , std::move(sub_shape) };
}

template<class M0, class M1> auto
make_sub_array(multi_array<M0,M1>& x, int i) { // TODO C++20 Integer (complicated without concepts)
  return make_sub_array(x,std_e::multi_index<int,1>{i});
}
template<class M0, class M1> auto
make_sub_array(const multi_array<M0,M1>& x, int i) {
  return make_sub_array(x,std_e::multi_index<int,1>{i});
}


template<class M0, class M1, class Multi_index> auto
make_span(multi_array<M0,M1>& x, const Multi_index& right_indices) {
  auto [index_1d,_] = shape_restriction_start_index_1d(x.shape(),right_indices);
  return make_span( x.data()+index_1d, x.extent(0) );
}
template<class M0, class M1, class Multi_index> auto
make_span(const multi_array<M0,M1>& x, const Multi_index& right_indices) {
  auto [index_1d,_] = shape_restriction_start_index_1d(x.shape(),right_indices);
  return make_span( x.data()+index_1d, x.extent(0) );
}

template<
  class M0, class M1, class I,
  std::enable_if_t< M1::rank()==2 , int > =0
> constexpr auto
column(multi_array<M0,M1>& x, I j) {
  return make_span(x,std_e::multi_index<I,1>{j});
}
template<
  class M0, class M1, class I,
  std::enable_if_t< M1::rank()==2 , int > =0
> constexpr auto
column(const multi_array<M0,M1>& x, I j) {
  return make_span(x,std_e::multi_index<I,1>{j});
}


} // std_e
