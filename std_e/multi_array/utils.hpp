#pragma once


#include "std_e/base/not_implemented_exception.hpp"
#include "std_e/multi_array/shape/dyn_shape.hpp"
#include "std_e/future/span.hpp"
#include "std_e/multi_array/multi_array/multi_array.hpp"
#include "std_e/multi_index/multi_index_range.hpp"


namespace std_e {


template<class Integer, int rank> inline auto
reshape(dyn_shape<Integer,rank>& x, const multi_index<Integer,rank>& dims) {
  using multi_index_type = typename dyn_shape<Integer,rank>::multi_index_type;
  STD_E_ASSERT(x.offset()==make_zero_multi_index<multi_index_type>(x.offset().size()));
  x.extent() = dims;
  x.offset() = make_zero_multi_index<multi_index_type>(dims.size());
}

template<class Dynamic_container, class Integer> auto
resize_memory(Dynamic_container& x, Integer n) -> void {
  x.resize(n);
}
template<class T, ptrdiff_t N, class Integer> auto
resize_memory(span<T,N>& x, Integer n) -> void {
  STD_E_ASSERT((Integer)x.size()==n);
}

template<class R, class Shape> auto
cons_reshape(multi_array<R,Shape>& x, const typename Shape::multi_index_type& dims) {
  auto n = std_e::cartesian_product_size(dims);
  STD_E_ASSERT(x.size()==n);
  reshape(x.shape(),dims);
}
template<class R, class Shape> auto
reshape(multi_array<R,Shape>& x, const typename Shape::multi_index_type& dims) {
  reshape(x.shape(),dims);
  auto total_size = std_e::cartesian_product_size(dims);
  resize_memory(x.underlying_range(),total_size);
}


template<class R, class Shape> auto
is_empty(const multi_array<R,Shape>& x) -> bool {
  return x.size()==0;
}


template<class R, class Shape> auto
to_string(const multi_array<R,Shape>& x) -> std::string {
  if (cartesian_product_size(x.extent())==0) {
    return "[]";
  }

  using std::to_string;
  if constexpr (Shape::ct_rank==0 || Shape::ct_rank==dynamic_size) {
    if (x.rank()==0) {
      return "["+to_string(x())+"]";
    }
  }
  if constexpr (Shape::ct_rank==1 || Shape::ct_rank==dynamic_size) {
    if (x.rank()==1) {
      std::string s = "["+to_string(x(0));
      int n = x.extent(0);
      for (int i=1; i<n; ++i) {
        s += "," + std::to_string(x(i));
      }
      return s + "]";
    }
  }
  if constexpr (Shape::ct_rank==2 || Shape::ct_rank==dynamic_size) {
    if (x.rank()==2) {
      std::string s = "[["+to_string(x(0,0));
      int n_i = x.extent(0);
      int n_j = x.extent(1);
      for (int j=1; j<n_j; ++j) {
        s += "," + std::to_string(x(0,j));
      }
      s += "]";
      for (int i=1; i<n_i; ++i) {
        s += ",[" + std::to_string(x(i,0));
        for (int j=1; j<n_j; ++j) {
          s += "," + std::to_string(x(i,j));
        }
        s += "]";
      }
      return s + "]";
    }
  }

  // rank > 2
  std::string s;
  for (const auto& is : fortran_multi_index_range(x.extent())) {
    s += to_string(is) + " => " + to_string(x(is)) + "\n";
  }
  return s;
}


} // std_e
