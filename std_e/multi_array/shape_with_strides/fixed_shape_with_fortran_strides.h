#pragma once


#include "std_e/multi_array/shape/fixed_shape.hpp"
#include "std_e/multi_index/fortran_order.h"


namespace std_e {


template<int... dims>
struct fixed_shape_with_fortran_strides : public fixed_shape<dims...> {
  using base = fixed_shape<dims...>;
  using index_type = typename base::index_type;
  using multi_index_type = typename base::multi_index_type;

  static constexpr int fixed_rank = base::fixed_rank;
  static constexpr multi_index_type fixed_extent = base::fixed_extent;
  static constexpr multi_index_type fixed_offset = base::fixed_offset;

  static constexpr multi_index_type fixed_strides = fortran_strides_from_extent(extent);

  static FORCE_INLINE constexpr auto
  strides() -> const multi_index_type& {
    return fixed_strides;
  }
};


} // std_e
