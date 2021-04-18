#pragma once


#include "std_e/multi_array/multi_array/multi_array.hpp"
#include "std_e/utils/meta.hpp"


namespace std_e {

template<class Multi_array_type>
class block_view {
  public:
  // type traits
    static constexpr bool mem_is_owned = false;
    using multi_index_type = typename Multi_array_type::multi_index_type;
    using index_type = typename Multi_array_type::index_type;

    using value_type = add_other_type_constness<typename Multi_array_type::value_type,Multi_array_type>;
    using T = value_type;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

  // ctor
    block_view(Multi_array_type& x, multi_index_type offset, multi_index_type dims)
      : ma_ptr(&x)
      , total_offset(std::move(offset))
      , dims(std::move(dims))
    {
      for (int i=0; i<rank(); ++i) {
        total_offset[i] += ma_ptr->offset(i);
      }
    }

  // dimensions
    // Note: there is a trick here:
    // These functions can be static (depending on type of Multi_array_shape).
    // In the case they are, they can be called
    //   - as non-static functions (C++ allows that)
    //   - as static functions (useful if a constexpr value is needed)
    constexpr auto
    rank() const -> int {
      return ma_ptr->rank();
    }
    constexpr auto
    offset() const -> const multi_index_type& {
      return ma_ptr->offset();
    }
    constexpr auto
    offset(int i) const -> index_type {
      return ma_ptr->offset(i);
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
      return ma_ptr->data()[linear_index(std::forward<Ts>(xs)...)];
    }
    template<class... Ts> FORCE_INLINE constexpr auto
    operator()(Ts&&... xs) -> reference {
      //static_assert(sizeof...(Ts)==rank());
      return ma_ptr->data()[linear_index(std::forward<Ts>(xs)...)];
    }

  private:
  /// linear_index {
    /// from Multi_index
    template<
      class Multi_index
      #if __cplusplus <= 201703L
      , std::enable_if_t< is_multi_index<Multi_index> , int > =0
      #endif
    >
      #if __cplusplus > 201703L
        requires Multi_index2<Multi_index>
      #endif
    FORCE_INLINE constexpr auto
    // requires Multi_index::size()==rank()
    linear_index(const Multi_index& indices) const -> index_type {
      return fortran_order_from_dimensions(ma_ptr->extent(),total_offset,indices);
    }
    /// from indices
    template<
      class... Integers
      #if __cplusplus <= 201703L
      , std::enable_if_t< std::conjunction_v< std::bool_constant<std::is_integral_v<Integers>> ... > , int > =0
      #endif
    >
      #if __cplusplus > 201703L
        requires std::conjunction_v< std::bool_constant<std::is_integral_v<Integers>>...> FORCE_INLINE constexpr auto
      #endif
    // requires sizeof...(Integers)==rank()
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
    Multi_array_type* ma_ptr;
    multi_index_type total_offset;
    multi_index_type dims;
};


template<class Multi_array_type, class multi_index_type = typename Multi_array_type::multi_index_type> auto
make_block_view(Multi_array_type& x, multi_index_type offset, multi_index_type dims) {
  return block_view<Multi_array_type>(x,offset,dims);
}

} // std_e
