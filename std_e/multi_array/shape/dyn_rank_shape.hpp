#pragma once


#include "std_e/multi_index/multi_index.h"


namespace std_e {


template<class Integer>
class dyn_rank_shape {
  public:
  // traits
    using index_type = Integer;
    static constexpr size_t fixed_rank = Dynamic_size;
    using multi_index_type = dyn_multi_index<index_type>;

  // ctors
    FORCE_INLINE dyn_rank_shape() = default;

    FORCE_INLINE 
    dyn_rank_shape(multi_index_type extent_, multi_index_type offset_)
      : extent_(std::move(extent_))
      , offset_(std::move(offset_))
    {
      std_e::ASSERT(extent().size()==offset().size());
    }

    FORCE_INLINE 
    dyn_rank_shape(multi_index_type extent_)
      : extent_(std::move(extent_))
      , offset_(zero_dyn_multi_index<Integer>(rank()))
    {}

  // accessors
    FORCE_INLINE auto
    rank() const -> size_t {
      return extent_.size();
    }
    FORCE_INLINE auto 
    offset() const -> const multi_index_type& {
      return offset_;
    }
    FORCE_INLINE auto 
    offset() -> multi_index_type& {
      return offset_;
    }
    FORCE_INLINE auto 
    extent() const -> const multi_index_type& {
      return extent_;
    }
    FORCE_INLINE auto 
    extent() -> multi_index_type& {
      return extent_;
    }
  private:
    multi_index_type extent_;
    multi_index_type offset_;
};


} // std_e


