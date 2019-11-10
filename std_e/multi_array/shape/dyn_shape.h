#pragma once


#include "std_e/multi_index/multi_index.h"


namespace std_e {


template<class Integer, size_t N>
class dyn_shape {
  public:
  // type traits
    using index_type = Integer;
    static constexpr size_t fixed_rank = N;

    using multi_index_type = multi_index<index_type,fixed_rank>;

  // ctors
    FORCE_INLINE constexpr dyn_shape() = default;

    FORCE_INLINE constexpr
    dyn_shape(multi_index_type extent_, multi_index_type offset_)
      : extent_(std::move(extent_))
      , offset_(std::move(offset_))
    {}

  // accessors
    static FORCE_INLINE constexpr auto
    rank() -> size_t {
      return fixed_rank;
    }
    FORCE_INLINE constexpr auto 
    offset() const -> const multi_index_type& {
      return offset_;
    }
    FORCE_INLINE constexpr auto 
    extent() const -> const multi_index_type& {
      return extent_;
    }
  private:
    multi_index_type extent_;
    multi_index_type offset_;
};


} // std_e


