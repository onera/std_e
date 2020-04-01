#pragma once

//template<class multi_array_type, class shape_type> FORCE_INLINE constexpr auto
//// requires multi_array_type is Multi_array
//// requires shape_type is Shape
//// requires shape_type::rank < strided_view::rank;
//block(const multi_array_type& x, const shape_type& r) {
//  using memory_access_type = typename multi_array_type::memory_access_type;
//  auto block_mem_access = offset_memory_access<memory_access_type,shape_type>(x.memory_access(),r);
//  return make_multi_array_view(x,block_mem_access);
//}
//template<class multi_range_type, class multi_array_type> FORCE_INLINE constexpr auto
//// requires multi_array_type is Multi_array
//// requires shape_type is Shape
//// requires shape_type::rank < strided_view::rank;
//block(const multi_array_type& x) {
//  using memory_access_type = typename multi_array_type::memory_access_type;
//
//  constexpr auto order = multi_range_type::order;
//  constexpr auto& start = multi_range_type::start;
//  constexpr auto& dimension = multi_range_type::dimension;
//  using start_int_seq = std_e::create_integer_sequence<order,start>;
//  using dimension_int_seq = std_e::create_integer_sequence<order,dimension>;
//  using shape_type = fixed_shape<dimension_int_seq,start_int_seq>;
//
//  auto block_mem_access = offset_memory_access<memory_access_type,shape_type>(x.memory_access(),shape_type());
//  return make_multi_array_view(x,block_mem_access);
//}
