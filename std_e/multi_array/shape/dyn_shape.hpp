#pragma once


#include "std_e/multi_index/multi_index.hpp"
#include "std_e/multi_index/cartesian_product_size.hpp"
#include "std_e/utils/array.hpp"
#include "std_e/utils/vector.hpp"
#include "std_e/base/dynamic_size.hpp"


namespace std_e {


template<class Integer, int N>
class dyn_shape_base {
  public:
  // type traits
    using index_type = Integer;
    static constexpr int ct_rank = N; // ct: compile-time
    static constexpr int ct_size = dynamic_size;

    using multi_index_type = multi_index<index_type,ct_rank>;

  // ctors
    FORCE_INLINE constexpr
    dyn_shape_base() = default;

    template<class Multi_index = multi_index_type> FORCE_INLINE constexpr
    dyn_shape_base(Multi_index ext, Multi_index off)
      : extent_(convert_to<multi_index_type>(std::move(ext)))
      , offset_(convert_to<multi_index_type>(std::move(off)))
    {
      STD_E_ASSERT(extent().size()==offset().size());
    }
    template<class Multi_index = multi_index_type> FORCE_INLINE constexpr
    dyn_shape_base(Multi_index ext)
      : extent_(convert_to<multi_index_type>(std::move(ext)))
      , offset_(make_zero_multi_index<multi_index_type>(extent_.size()))
    {}
    FORCE_INLINE constexpr
    dyn_shape_base(std::initializer_list<index_type> ext)
      : extent_(convert_to<multi_index_type>(ext))
      , offset_(make_zero_multi_index<multi_index_type>(extent_.size()))
    {}
    FORCE_INLINE constexpr
    dyn_shape_base(std::initializer_list<index_type> ext, std::initializer_list<index_type> off)
      : extent_(convert_to<multi_index_type>(ext))
      , offset_(convert_to<multi_index_type>(off))
    {}

  // accessors
    FORCE_INLINE constexpr auto
    size() const -> index_type {
      return cartesian_product_size(this->extent());
    }

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
class dyn_shape : public dyn_shape_base<Integer,N> {
  public:
  // ctors
    using base = dyn_shape_base<Integer,N>;
    using base::base;
    FORCE_INLINE constexpr
    dyn_shape()
      : base(zero_multi_index<Integer,N>)
    {}

  // accessors
    static FORCE_INLINE constexpr auto
    rank() -> int {
      return N;
    }
};



template<class Integer>
class dyn_shape<Integer,dynamic_size> : public dyn_shape_base<Integer,dynamic_size> {
  public:
  // ctors
    using base = dyn_shape_base<Integer,dynamic_size>;
    using base::base;

  // accessors
    FORCE_INLINE auto
    rank() const -> int {
      return base::extent().size();
    }
};


template<class Integer> constexpr auto
make_sub_shape(const dyn_shape<Integer,dynamic_size>& shape, int right_restriction) {
  int sub_shape_rank = shape.rank()-right_restriction;
  return dyn_shape<Integer,dynamic_size>(
    make_sub_array(shape.extent(),0,sub_shape_rank),
    make_sub_array(shape.offset(),0,sub_shape_rank)
  );
}
template<int right_restriction, class Integer, int N> constexpr auto
make_sub_shape(const dyn_shape<Integer,N>& shape) {
  if constexpr (N==dynamic_size) {
    return make_sub_shape(shape,right_restriction);
  } else {
    constexpr int sub_shape_rank = N-right_restriction;
    return dyn_shape<Integer,sub_shape_rank>(
      make_sub_array<0,sub_shape_rank>(shape.extent()),
      make_sub_array<0,sub_shape_rank>(shape.offset())
    );
  }
}

template<class Integer, int N, class Multi_index> constexpr auto
make_sub_shape(const dyn_shape<Integer,N>& shape, const Multi_index& fixed_dim_indices) {
  constexpr int fixed_rank = rank_of<Multi_index>;
  constexpr int rank = N - fixed_rank;
  // NOTE: zip | copy_if
  multi_index<Integer,rank> extent = {};
  multi_index<Integer,rank> offset = {};
  int k0 = 0;
  int k1 = 0;
  for (int i=0; i<N; ++i) {
    if (k0<fixed_rank && i==fixed_dim_indices[k0]) {
      ++k0;
    } else {
      extent[k1] = shape.extent(i);
      offset[k1] = shape.offset(i);
      ++k1;
    }
  }
  return dyn_shape<Integer,rank>(extent,offset);
}

// TODO factorize with above
template<class Integer, class Multi_index> constexpr auto
make_sub_shape(const dyn_shape<Integer,dynamic_size>& shape, const Multi_index& fixed_dim_indices) {
  int fixed_rank = fixed_dim_indices.size();
  int rank = shape.rank() - fixed_rank;
  // NOTE: zip | copy_if
  multi_index<Integer> extent(rank);
  multi_index<Integer> offset(rank);
  int k0 = 0;
  int k1 = 0;
  for (int i=0; i<shape.rank(); ++i) {
    if (k0<fixed_rank && i==fixed_dim_indices[k0]) {
      ++k0;
    } else {
      extent[k1] = shape.extent(i);
      offset[k1] = shape.offset(i);
      ++k1;
    }
  }
  return dyn_shape<Integer,dynamic_size>(extent,offset);
}


} // std_e
