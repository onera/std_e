#pragma once 


#include "std_e/multi_index/multi_index.hpp"


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


} // std_e
