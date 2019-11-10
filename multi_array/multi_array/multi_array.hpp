#pragma once


#include "std_e/base/macros.hpp"
#include "std_e/memory_ressource/concept.hpp"
#include "std_e/multi_array/shape/concept.hpp"
#include "std_e/multi_index/concept.hpp"
#include "std_e/multi_index/multi_index.hpp"
#include "std_e/multi_index/fortran_order.hpp"
#include "std_e/future/contract.hpp"


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

    static constexpr size_t fixed_rank = shape_type::fixed_rank;
    using multi_index_type = typename shape_type::multi_index_type;

  // constructors
    FORCE_INLINE constexpr multi_array() = default;

    FORCE_INLINE constexpr
    multi_array(memory_ressource_type mem, shape_type sh)
      : shape_type(std::move(sh))
      , mem(std::move(mem))
    {}

    // Ctor for rank==1, owning memory
    multi_array(std::initializer_list<value_type> l) 
      : shape_type{{index_type(l.size())},{0}}
      , mem(l.size())
    {
      index_type i=0;
      for (const value_type& x : l) {
        (*this)(i) = x;
        ++i;
      }
    }
    // Ctor for rank==2, owning memory
    multi_array(std::initializer_list<std::initializer_list<value_type>> ll)
      : shape_type{{index_type(ll.size()),index_type(begin(ll)->size())},{0,0}}
      , mem(ll.size()*begin(ll)->size())
    {
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


    FORCE_INLINE constexpr auto
    data() const -> const_pointer {
      return mem.data();
    }
    FORCE_INLINE auto 
    data() -> pointer {
      return mem.data();
    }

  // dimensions
    // Note: there is a trick here:
    // These functions can be static (depending on type of Multi_array_shape).
    // In this case, they can be called
    //   - as non-static functions (C++ allows that)
    //   - as static functions if a constexpr value is needed
    using base::rank;
    using base::extent;
    using base::offset;

  // element access
    template<class... Ts> FORCE_INLINE constexpr auto
    operator()(Ts&&... xs) const -> const_reference {
      return mem[linear_index(std::forward<Ts>(xs)...)];
    }
    template<class... Ts> FORCE_INLINE constexpr auto
    operator()(Ts&&... xs) -> reference {
      return mem[linear_index(std::forward<Ts>(xs)...)];
    }

  public:
// member functions
  /// linear index {
  // from Multi_index
    template<class Multi_index> FORCE_INLINE constexpr auto 
    // requires Multi_index is an array && Multi_index::size()==rank()
    linear_index(const Multi_index& indices) const -> index_type {
      return fortran_order_from_dimensions(extent(),offset(),indices);
    }
  // from indices
    template<class... ints> FORCE_INLINE constexpr auto
    // requires ints are integers && sizeof...(ints)==rank()
    linear_index(index_type i, ints... is) const -> index_type {
      std_e::ASSERT(1+sizeof...(ints)==rank()); // the number of indices must be the array rank
      return linear_index(multi_index_type{i,is...});
    }
    template<class... ints> FORCE_INLINE constexpr auto
    // requires ints are integers && sizeof...(ints)==rank()
    linear_index(int i, ints... is) const -> index_type {
      std_e::ASSERT(1+sizeof...(ints)==rank()); // the number of indices must be the array rank
      return linear_index(multi_index_type{i,is...});
    }
  /// linear index }

// data members
    memory_ressource_type mem;
};


} // std_e




