#pragma once


#include "std_e/multi_array/shape/dyn_shape.hpp"
#include "std_e/multi_index/fortran_order.h"


namespace std_e {


template<class Integer, int N>
class dyn_shape_with_fortran_strides : public dyn_shape<Integer,N> {
  public:
  // type traits
    using base = fixed_shape<dims...>;
    using index_type = typename base::index_type;
    using multi_index_type = typename base::multi_index_type;

  // ctors
    FORCE_INLINE constexpr dyn_shape_with_fortran_strides() = default;

    template<class Multi_index = multi_index_type> FORCE_INLINE constexpr
    dyn_shape_with_fortran_strides(Multi_index ext, Multi_index off)
      : base(std::move(ext),std::move(off))
      , strides_(fortran_strides_from_extent(this->extent()))
    {}

  // accessors
    FORCE_INLINE constexpr auto
    strides() const -> const multi_index_type& {
      return strides_;
    }
    FORCE_INLINE auto
    strides(int i) const -> index_type {
      return strides()[i];
    }
  private:
    multi_index_type strides_;
};


} // std_e
