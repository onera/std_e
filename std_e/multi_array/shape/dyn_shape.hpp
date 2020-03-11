#pragma once


#include "std_e/multi_index/multi_index.hpp"
#include "std_e/utils/array_vector_common.hpp"
#include "std_e/utils/array.hpp"
#include "std_e/utils/vector.hpp"
#include "std_e/base/dynamic_size.hpp"


namespace std_e {


template<class Integer, int N>
class dyn_shape__impl {
  public:
  // type traits
    using index_type = Integer;
    static constexpr int fixed_rank = N;

    using multi_index_type = multi_index<index_type,fixed_rank>;

  // ctors
    FORCE_INLINE constexpr
    dyn_shape__impl() = default;

    template<class Multi_index = multi_index_type> FORCE_INLINE constexpr
    dyn_shape__impl(Multi_index ext, Multi_index off)
      : extent_(convert_to<multi_index_type>(std::move(ext)))
      , offset_(convert_to<multi_index_type>(std::move(off)))
    {
      STD_E_ASSERT(extent().size()==offset().size());
    }
    template<class Multi_index = multi_index_type> FORCE_INLINE constexpr
    dyn_shape__impl(Multi_index ext)
      : extent_(convert_to<multi_index_type>(std::move(ext)))
      , offset_(make_zero_multi_index<multi_index_type>(extent_.size()))
    {}

  // accessors
    FORCE_INLINE constexpr auto
    extent() const -> const multi_index_type& {
      return extent_;
    }
    FORCE_INLINE constexpr auto
    offset() const -> const multi_index_type& {
      return offset_;
    }
    FORCE_INLINE auto
    extent(int i) const -> index_type {
      return extent()[i];
    }
    FORCE_INLINE auto
    offset(int i) const -> index_type {
      return offset()[i];
    }

    FORCE_INLINE constexpr auto
    extent() -> multi_index_type& {
      return extent_;
    }
    FORCE_INLINE constexpr auto
    offset() -> multi_index_type& {
      return offset_;
    }
    FORCE_INLINE auto
    extent(int i) -> index_type& {
      return extent()[i];
    }
    FORCE_INLINE auto
    offset(int i) -> index_type& {
      return offset()[i];
    }
  private:
    multi_index_type extent_;
    multi_index_type offset_;
};


template<class Integer, int N>
class dyn_shape : public dyn_shape__impl<Integer,N> {
  public:
  // ctors
    using base = dyn_shape__impl<Integer,N>;
    using base::base;

  // accessors
    static FORCE_INLINE constexpr auto
    rank() -> int {
      return N;
    }
};



template<class Integer>
class dyn_shape<Integer,dynamic_size> : public dyn_shape__impl<Integer,dynamic_size> {
  public:
  // ctors
    using base = dyn_shape__impl<Integer,dynamic_size>;
    using base::base;

  // accessors
    FORCE_INLINE auto
    rank() const -> int {
      return base::extent().size();
    }
};


template<int right_restriction, class Integer, int N> constexpr auto
make_sub_shape(const dyn_shape<Integer,N>& shape) {
  if constexpr (N==dynamic_size) {
    int sub_shape_rank = N-right_restriction;
    return dyn_shape<Integer,dynamic_size>(
      make_sub_array(shape.extent(),0,sub_shape_rank),
      make_sub_array(shape.offset(),0,sub_shape_rank)
    );
  } else {
    constexpr int sub_shape_rank = N-right_restriction;
    return dyn_shape<Integer,sub_shape_rank>(
      make_sub_array<0,sub_shape_rank>(shape.extent()),
      make_sub_array<0,sub_shape_rank>(shape.offset())
    );
  }
}


} // std_e
