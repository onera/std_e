#pragma once


#include "std_e/base/macros.hpp"
#include "std_e/memory_ressource/concept.hpp"
#include "std_e/multi_array/multi_array/concept.hpp"
#include "std_e/multi_index/multi_index.hpp"


namespace std_e {

/**
  concept Permutation
    permute(Multi_index) const -> Multi_index
*/

struct reversing_permutation {
  template<class Multi_index> constexpr auto
  permute(const Multi_index& is) const -> Multi_index {
    Multi_index inv_is(is.size());
    std::reverse_copy(begin(is),end(is),begin(inv_is));
    return inv_is;
  }
};

template<class Multi_array_shape, class Permutation> constexpr auto
permuted_shape(const Multi_array_shape& shape, const Permutation& perm) -> Multi_array_shape {
  return
    Multi_array_shape(
      perm.permute(shape.extent()),
      perm.permute(shape.offset())
    );
}


template<class T_ptr, class Multi_array_shape, class Permutation>
class multi_array_permuted_view : private Multi_array_shape, private Permutation
{
  public:
  // type traits
    using shape_type = Multi_array_shape;
    using permutation_type = Permutation;

    using index_type = typename shape_type::index_type;
    using multi_index_type = typename shape_type::multi_index_type;

    using value_type = std::remove_pointer_t<T_ptr>;
    using pointer = T_ptr;
    using const_pointer = const T_ptr;
    using reference = value_type&;
    using const_reference = const value_type&;

    static constexpr size_t fixed_rank = shape_type::fixed_rank;

  // constructors
    FORCE_INLINE constexpr multi_array_permuted_view() = default;

    FORCE_INLINE constexpr
    multi_array_permuted_view(T_ptr ptr, const shape_type& sh, permutation_type perm)
      : shape_type(permuted_shape(sh,perm))
      , permutation_type(std::move(perm))
      , source_shape(&sh)
      , ptr(ptr)
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

  // dimensions
    // Note: there is a trick here:
    // These functions can be static (depending on type of Multi_array_shape).
    // In the case they are, they can be called
    //   - as non-static functions (C++ allows that)
    //   - as static functions (useful if a constexpr value is needed)
    using shape_type::rank;
    using shape_type::extent;
    using shape_type::offset;

  // element access
    template<class... Ts> FORCE_INLINE constexpr auto
    operator()(Ts&&... xs) const -> const_reference {
      return ptr[fortran_linear_index(std::forward<Ts>(xs)...)];
    }
    template<class... Ts> FORCE_INLINE constexpr auto
    operator()(Ts&&... xs) -> reference {
      return ptr[fortran_linear_index(std::forward<Ts>(xs)...)];
    }

  public:
  // member functions
    using permutation_type::permute;

  /// fortran_linear_index { // TODO factorize with multi_array
  // from Multi_index
    template<class Multi_index> FORCE_INLINE constexpr auto
    // requires Multi_index is an array && Multi_index::size()==rank()
    fortran_linear_index(const Multi_index& indices) const -> index_type {
      auto source_indices = permute(indices);
      return fortran_order_from_dimensions(source_shape->extent(),source_shape->offset(),source_indices);
    }
  // from indices
    template<class... ints> FORCE_INLINE constexpr auto
    // requires ints are integers && sizeof...(ints)==rank()
    fortran_linear_index(index_type i, ints... is) const -> index_type {
      STD_E_ASSERT(1+sizeof...(ints)==rank()); // the number of indices must be the array rank
      return fortran_linear_index(multi_index_type{i,is...});
    }
    template<class... ints> FORCE_INLINE constexpr auto
    // requires ints are integers && sizeof...(ints)==rank()
    fortran_linear_index(int i, ints... is) const -> index_type { // Note: same as above, but forcing first arg
                                                                  // to be an integer for unsurprising overload resolution
      STD_E_ASSERT(1+sizeof...(ints)==rank()); // the number of indices must be the array rank
      return fortran_linear_index(multi_index_type{i,is...});
    }
  /// fortran_linear_index }
  // data member
    const shape_type* source_shape;
    T_ptr ptr;
};


template<class Multi_array> constexpr auto
reversed_indices_view(const Multi_array& ma) {
  using T_ptr = typename Multi_array::const_pointer;
  using shape_type = typename Multi_array::shape_type;
  return multi_array_permuted_view<T_ptr,shape_type,reversing_permutation>(ma.data(),ma.shape(),{});
}


} // std_e




