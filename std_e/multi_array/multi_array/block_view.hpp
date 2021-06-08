#pragma once


#include "std_e/interval/multi_interval.hpp"
#include "std_e/multi_array/multi_array/multi_array.hpp"
#include "std_e/meta/meta.hpp"
#include <type_traits>


namespace std_e {

template<class Multi_array_type>
class block_view {
  public:
  // type traits
    static constexpr bool mem_is_owned = false;
    static constexpr int ct_rank = std::decay_t<Multi_array_type>::ct_rank;
    using multi_index_type = typename std::decay_t<Multi_array_type>::multi_index_type;
    using index_type = typename std::decay_t<Multi_array_type>::index_type;

    using value_type = add_other_type_constness<typename std::decay_t<Multi_array_type>::value_type,std::remove_reference_t<Multi_array_type>>;
    using T = value_type;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

  // ctors
    template<class Multi_array_type_0>
    block_view(Multi_array_type_0&& x, multi_index_type offset, multi_index_type dims)
      : origin_ma(FWD(x))
      , total_offset(std::move(offset))
      , dims(std::move(dims))
    {
      for (int i=0; i<rank(); ++i) {
        total_offset[i] += origin_ma.offset(i);
      }
    }

    template<class Multi_array_type_0, class I0>
    block_view(Multi_array_type_0&& x, const multi_interval<I0>& sub_interval)
      : block_view(FWD(x),sub_interval.first(),length(sub_interval))
    {}

  // dimensions
    // Note: there is a trick here:
    // These functions can be static (depending on type of Multi_array_shape).
    // In the case they are, they can be called
    //   - as non-static functions (C++ allows that)
    //   - as static functions (useful if a constexpr value is needed)
    constexpr auto
    rank() const -> int {
      return origin_ma.rank();
    }
    constexpr auto
    offset() const -> const multi_index_type& {
      return origin_ma.offset();
    }
    constexpr auto
    offset(int i) const -> index_type {
      return origin_ma.offset(i);
    }
    constexpr auto
    extent() const -> const multi_index_type& {
      return dims;
    }
    constexpr auto
    extent(int i) const -> index_type {
      return dims[i];
    }
    constexpr auto
    size() const -> index_type {
      return cartesian_product_size(extent());
    }

  // element access
    template<class... Ts> FORCE_INLINE constexpr auto
    operator()(Ts&&... xs) const -> const_reference {
      //static_assert(sizeof...(Ts)==rank());
      return origin_ma.data()[linear_index(std::forward<Ts>(xs)...)];
    }
    template<class... Ts> FORCE_INLINE constexpr auto
    operator()(Ts&&... xs) -> reference {
      //static_assert(sizeof...(Ts)==rank());
      return origin_ma.data()[linear_index(std::forward<Ts>(xs)...)];
    }

  private:
  /// linear_index {
    /// from Multi_index
    template<
      class MI
      #if __cplusplus <= 201703L
      , std::enable_if_t< is_multi_index<MI> , int > =0
      #endif
    >
      #if __cplusplus > 201703L
        requires Multi_index<MI> && (MI::ct_rank==ct_rank)
      #endif
    FORCE_INLINE constexpr auto
    linear_index(const MI& indices) const -> index_type {
      return fortran_order_from_dimensions(origin_ma.extent(),total_offset,indices);
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
      //STD_E_ASSERT(sizeof...(Integers)==rank());
      return linear_index(multi_index_type{is...});
    }
    FORCE_INLINE constexpr auto
    linear_index() const -> index_type { // Note: rank==0 case
      return 0;
    }
  /// linear_index }

// data members
    remove_rvalue_reference<Multi_array_type> origin_ma;
    multi_index_type total_offset;
    multi_index_type dims;
};


template<class Multi_array_type, class multi_index_type = typename std::decay_t<Multi_array_type>::multi_index_type> auto
make_block_view(Multi_array_type&& x, multi_index_type offset, multi_index_type dims) {
  return block_view<Multi_array_type&&>(FWD(x),offset,dims);
}
template<class Multi_array_type, class I0 = int> auto
make_block_view(Multi_array_type&& x, const multi_interval<I0>& sub_interval) {
  return block_view<Multi_array_type&&>(FWD(x),sub_interval);
}

} // std_e
