#pragma once

// TODO clean
#include "std_e/multi_array/memory_ressource/memory_view.h"

#include "std_e/multi_array/memory_access/strided_memory_access.h"
#include "std_e/multi_array/memory_access/offset_memory_access.h"
#include "std_e/multi_array/memory_access/slice_memory_access.h"

#include "std_e/multi_array/shape/fixed_shape.h"
#include "std_e/int_seq_from_to_multi_index.h"


namespace std_e {
  

template<class mem_ressource_type, class mem_access_type>
// requires mem_ressource_type is Memory_ressource
// requires mem_access_type is Memory_access
class multi_array 
  : private mem_access_type // logically a member but privately inherited to enable empty base optimization
{
  public:
  // type traits
    using memory_ressource_type = mem_ressource_type;
    using memory_access_type = mem_access_type;

    using value_type = typename memory_ressource_type::value_type;
    using pointer_type = typename memory_ressource_type::pointer_type;
    using const_pointer_type = typename memory_ressource_type::const_pointer_type;
    using reference_type = typename memory_ressource_type::reference_type;
    using const_reference_type = typename memory_ressource_type::const_reference_type;

    static constexpr int rank = mem_access_type::rank;
    static constexpr auto fixed_extent = memory_access_type::fixed_extent;

  // constructors
    FORCE_INLINE constexpr multi_array() = default;

    // TODO op=
    // TODO check order, dims
    template<class other_mem_ressource_type, class other_mem_access_type> FORCE_INLINE constexpr 
    multi_array(const multi_array<other_mem_ressource_type,other_mem_access_type>& old)
    {
      // TODO generalize
      for (int i=0; i<extent()[0] ; ++i) {
        (*this)(i) = old(i);
      }
    }

    FORCE_INLINE constexpr
    multi_array(memory_ressource_type mem, memory_access_type mem_access)
      : mem_access_type(std::move(mem_access))
      , mem(std::move(mem))
    {}


  // low-level
    FORCE_INLINE constexpr auto
    memory_access() const -> const memory_access_type& {
      return mem_access(); 
    }

    FORCE_INLINE constexpr auto
    memory() const -> const memory_ressource_type& {
      return mem; 
    }
    auto
    memory() -> memory_ressource_type& {
      return mem; 
    }


    FORCE_INLINE constexpr auto
    data() const -> const_pointer_type {
      return mem.data();
    }
    auto 
    data() -> pointer_type {
      return mem.data();
    }

  // dimensions
    FORCE_INLINE constexpr auto
    extent() const -> const multi_index<rank>& {
      return mem_access().extent();
    }
    FORCE_INLINE constexpr auto
    origin() const -> const multi_index<rank>& {
      return mem_access().origin();
    }

  // element access
    // by operator()(is)
    template<class... ints> FORCE_INLINE constexpr auto
    // requires ints are integers
    operator()(int i, ints... is) const -> const_reference_type {
      static_assert(1+sizeof...(ints)==rank,"The number of is must be the array rank");
      auto pos = mem_access().complete_position_in_one_step__from_indices(i,is...);
      return mem[pos];
    }
    template<class... ints> FORCE_INLINE constexpr auto
    // requires ints are integers
    operator()(int i, ints... is) -> reference_type {
      static_assert(1+sizeof...(ints)==rank,"The number of is must be the array rank");
      auto pos = mem_access().complete_position_in_one_step__from_indices(i,is...);
      return mem[pos];
    }
    // by operator()(index_array)
    FORCE_INLINE constexpr auto
    operator()(const std_e::multi_index<rank>& is) const -> const_reference_type {
      auto pos = mem_access().complete_position_in_one_step__from_index_array(is);
      return mem[pos];
    }
    FORCE_INLINE constexpr auto
    operator()(const std_e::multi_index<rank>& is) -> reference_type {
      auto pos = mem_access().complete_position_in_one_step__from_index_array(is);
      return mem[pos];
    }
    // by operator()(index_1d)
    FORCE_INLINE constexpr auto 
    operator()(multi_index_1d<rank> index_1d) const -> const_reference_type {
      auto pos = mem_access().complete_position_in_one_step__from_index_1d(index_1d);
      return mem[pos];
    }
    FORCE_INLINE constexpr auto
    operator()(multi_index_1d<rank> index_1d) -> reference_type {
      auto pos = mem_access().complete_position_in_one_step__from_index_1d(index_1d);
      return mem[pos];
    }
  // two-time access TODO DEL ?
    // by operator()(index_1d)
    template<class... ints> FORCE_INLINE constexpr auto
    two_time_access(multi_index_1d<rank-sizeof...(ints)> index_1d, ints... is) const -> const_reference_type {
      auto pos = mem_access().complete_position_in_two_steps__from_indices(index_1d,is...);
      return mem[pos];
    }
    template<class... ints> FORCE_INLINE constexpr auto
    two_time_access(multi_index_1d<rank-sizeof...(ints)> index_1d, ints... is) -> reference_type {
      auto pos = mem_access().complete_position_in_two_steps__from_indices(index_1d,is...);
      return mem[pos];
    }

  public: // TODO
  //private:
    FORCE_INLINE constexpr auto
    mem_access() const -> const mem_access_type& {
      return *this;
    }
    memory_ressource_type mem;
};





template<class base_multi_array_type, class mem_access_type> FORCE_INLINE constexpr auto
// requires base_multi_array_type is Multi_array
// requires mem_access_type is Memory_access
make_multi_array_view(const base_multi_array_type& x, const mem_access_type& mem_access) {
  using base_memory_type = typename base_multi_array_type::memory_ressource_type;
  using memory_view_type = const_memory_view<base_memory_type>;
  memory_view_type mem_view(&x.memory());
  return multi_array<memory_view_type,mem_access_type>(mem_view,mem_access);
}
template<class base_multi_array_type, class mem_access_type> FORCE_INLINE constexpr auto
// requires base_multi_array_type is Multi_array
// requires mem_access_type is Memory_access
make_multi_array_view(base_multi_array_type& x, const mem_access_type& mem_access) {
  using base_memory_type = typename base_multi_array_type::memory_ressource_type;
  using memory_view_type = memory_view<base_memory_type>;
  memory_view_type mem_view(&x.memory());
  return multi_array<memory_view_type,mem_access_type>(mem_view,mem_access);
}




// block access TODO DEL ?
template<class multi_array_type, class shape_type> FORCE_INLINE constexpr auto
// requires multi_array_type is Multi_array
// requires shape_type is Shape
// requires shape_type::rank < multi_array::rank;
block(const multi_array_type& x, const shape_type& r) {
  using memory_access_type = typename multi_array_type::memory_access_type;
  auto block_mem_access = offset_memory_access<memory_access_type,shape_type>(x.memory_access(),r);
  return make_multi_array_view(x,block_mem_access);
}
template<class multi_array_type, class shape_type> FORCE_INLINE constexpr auto
// requires multi_array_type is Multi_array
// requires shape_type is Shape
// requires shape_type::rank < multi_array::rank;
block(multi_array_type& x, const shape_type& r) {
  using memory_access_type = typename multi_array_type::memory_access_type;
  auto block_mem_access = offset_memory_access<memory_access_type,shape_type>(x.memory_access(),r);
  return make_multi_array_view(x,block_mem_access);
}

// block access
template<class multi_range_type, class multi_array_type> FORCE_INLINE constexpr auto
// requires multi_array_type is Multi_array
// requires shape_type is Shape
// requires shape_type::rank < multi_array::rank;
block(const multi_array_type& x) {
  using memory_access_type = typename multi_array_type::memory_access_type;

  constexpr auto order = multi_range_type::order;
  constexpr auto& start = multi_range_type::start;
  constexpr auto& dimension = multi_range_type::dimension;
  using start_int_seq = std_e::create_integer_sequence<order,start>;
  using dimension_int_seq = std_e::create_integer_sequence<order,dimension>;
  using shape_type = fixed_shape<dimension_int_seq,start_int_seq>;

  auto block_mem_access = offset_memory_access<memory_access_type,shape_type>(x.memory_access(),shape_type());
  return make_multi_array_view(x,block_mem_access);
}
template<class multi_range_type, class multi_array_type> FORCE_INLINE constexpr auto
// requires multi_array_type is Multi_array
// requires shape_type is Shape
// requires shape_type::rank < multi_array::rank;
block(multi_array_type& x) {
  using memory_access_type = typename multi_array_type::memory_access_type;

  constexpr auto order = multi_range_type::order;
  constexpr auto& start = multi_range_type::start;
  constexpr auto& dimension = multi_range_type::dimension;
  using start_int_seq = std_e::create_integer_sequence<order,start>;
  using dimension_int_seq = std_e::create_integer_sequence<order,dimension>;
  using shape_type = fixed_shape<dimension_int_seq,start_int_seq>;

  auto block_mem_access = offset_memory_access<memory_access_type,shape_type>(x.memory_access(),shape_type());
  return make_multi_array_view(x,block_mem_access);
}



// sub view // TODO slice
template<class multi_array_type, size_t outer_rank> FORCE_INLINE constexpr auto
// requires multi_array_type is Multi_array
// requires 0 < outer_rank < rank
sub_view(const multi_array_type& x, multi_index_1d<outer_rank> outer_index) {
  constexpr size_t sub_view_rank = multi_array_type::rank - outer_rank;
  using memory_access_type = typename multi_array_type::memory_access_type;
  auto sub_view_mem_access = strided_memory_access<memory_access_type,sub_view_rank>(x.memory_access(),outer_index);
  return make_multi_array_view(x,sub_view_mem_access);
}
template<class multi_array_type, size_t outer_rank> auto
// requires multi_array_type is Multi_array
// requires 0 < outer_rank < rank
sub_view(multi_array_type& x, multi_index_1d<outer_rank> outer_index) {
  constexpr size_t sub_view_rank = multi_array_type::rank - outer_rank;
  using memory_access_type = typename multi_array_type::memory_access_type;
  auto sub_view_mem_access = strided_memory_access<memory_access_type,sub_view_rank>(x.memory_access(),outer_index);
  return make_multi_array_view(x,sub_view_mem_access);
}
// TODO
template<int outer_index, class multi_array_type> FORCE_INLINE constexpr auto
// requires multi_array_type is Multi_array
// requires 0 < outer_rank < rank
sub_view2(const multi_array_type& x) {
  //constexpr auto m_i_1d = multi_index_1d<outer_rank>{outer_index};
  //return sub_view(x,m_i_1d);
  using memory_access_type = typename multi_array_type::memory_access_type;
  auto sub_view_mem_access = strided_memory_access2<memory_access_type,outer_index>(x.memory_access());
  return make_multi_array_view(x,sub_view_mem_access);
}


// TODO overload with for multi-slice (note: should need c++17 template auto)
template<int direction, int index, class multi_array_type> FORCE_INLINE constexpr auto
// requires multi_array_type is Multi_array
// requires 0 < outer_rank < rank
slice(const multi_array_type& x) {
  using memory_access_type = typename multi_array_type::memory_access_type;
  auto slice_mem_access = slice_memory_access<memory_access_type,direction,index>(x.memory_access());
  return make_multi_array_view(x,slice_mem_access);
}

template<int direction, class multi_array_type, class... multi_array_types> FORCE_INLINE constexpr auto
// requires multi_array_types are Multi_array
// requires is_same_t<multi_array_types::memory_access_type...>
// requires 0 < outer_rank < rank
concat_slices(const multi_array_type& x, const multi_array_types&... xs) {
  using memory_access_type = typename multi_array_type::memory_access_type;
  constexpr auto N = 1+sizeof...(multi_array_types);
  auto concat_slices_mem_access = 
    concat_slices_memory_access<memory_access_type,N,direction>(
      x.memory_access(),xs.memory_access()...
    );
  return make_multi_array_view(x,concat_slices_mem_access);
}


} // std_e




