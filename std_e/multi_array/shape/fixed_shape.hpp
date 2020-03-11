#pragma once 


#include "std_e/multi_index/multi_index.hpp"
#include "std_e/utils/index_sequence.hpp"


namespace std_e {


template<int... dims>
struct fixed_shape {
  using index_type = int;
  static constexpr int fixed_rank = sizeof...(dims);

  using multi_index_type = multi_index<index_type,fixed_rank>;
  static constexpr multi_index_type fixed_extent = {dims...};
  static constexpr multi_index_type fixed_offset = {0};

  static FORCE_INLINE constexpr auto
  rank() -> int {
    return fixed_rank;
  }
  static FORCE_INLINE constexpr auto
  extent() -> const multi_index_type& {
    return fixed_extent;
  }
  static FORCE_INLINE constexpr auto
  offset() -> const multi_index_type& {
    return fixed_offset;
  }
  static FORCE_INLINE auto 
  extent(int i) -> index_type {
    return extent()[i];
  }
  static FORCE_INLINE auto 
  offset(int i) -> index_type {
    return offset()[i];
  }
};


template<size_t... dims> constexpr auto
fixed_shape_from_index_seq(std::index_sequence<dims...>) {
  return fixed_shape<dims...>{};
}

template<int right_restriction, int... dims> constexpr auto
make_sub_shape(fixed_shape<dims...>) {
  constexpr int N = sizeof...(dims)-right_restriction;
  auto index_seq = std::index_sequence<dims...>{};
  auto sub_index_seq = take_n<N>(index_seq);
  return fixed_shape_from_index_seq(sub_index_seq);
}


} // std_e
