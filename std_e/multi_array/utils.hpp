#pragma once


#include "std_e/base/not_implemented_exception.hpp"
#include "std_e/multi_array/shape/dyn_shape.hpp"
#include "std_e/base/memory_view.hpp"
#include "std_e/multi_array/multi_array/multi_array.hpp"


namespace std_e {


template<class Integer, int rank> inline auto
reshape(dyn_shape<Integer,rank>& x, const multi_index<Integer,rank>& dims) {
  using multi_index_type = typename dyn_shape<Integer,rank>::multi_index_type;
  STD_E_ASSERT(x.offset()==make_zero_multi_index<multi_index_type>(x.offset().size()));
  x.extent() = dims;
  x.offset() = make_zero_multi_index<multi_index_type>(dims.size());
}

template<class Memory_ressource> auto
resize_memory(Memory_ressource& x, size_t n) -> void {
  x.resize(n);
}
template<class T_ptr> auto
resize_memory(memory_view<T_ptr>& x, size_t n) -> void {
  // ASSERT same as or less than old size
}

template<class Memory_ressource, class Multi_array_shape> auto
reshape(multi_array<Memory_ressource,Multi_array_shape>& x, const typename Multi_array_shape::multi_index_type& dims) {
  reshape(x.shape(),dims);
  size_t total_size = std_e::cartesian_product(dims);
  resize_memory(x.memory(),total_size);
}


template<class Memory_ressource, class Multi_array_shape> auto
empty(const multi_array<Memory_ressource,Multi_array_shape>& x) -> bool {
  return std_e::cartesian_product(x.extent())==0;
}


template<
  class Memory_ressource, class Multi_array_shape,
  std::enable_if_t< Multi_array_shape::fixed_rank==1 , int > =0
> auto
to_string_1D(const multi_array<Memory_ressource,Multi_array_shape>& x) -> std::string {
  std::string s = "["+std::to_string(x[0]);
  int n = x.extent()[0];
  for (int i=1; i<n; ++i) {
    s += "," + std::to_string(x(i));
  }
  return s + "]";
}
template<
  class Memory_ressource, class Multi_array_shape,
  std::enable_if_t< Multi_array_shape::fixed_rank==2 , int > =0
> auto
to_string_2D(const multi_array<Memory_ressource,Multi_array_shape>& x) -> std::string {
  std::string s = "["+std::to_string(x(0,0));
  int n_i = x.extent()[0];
  int n_j = x.extent()[1];
  for (int j=1; j<n_j; ++j) {
    s += "," + std::to_string(x(0,j));
  }
  for (int i=1; i<n_i; ++i) {
    s += ";" + std::to_string(x(i,0));
    for (int j=1; j<n_j; ++j) {
      s += "," + std::to_string(x(i,j));
    }
  }
  return s + "]";
}

template<
  class Memory_ressource, class Multi_array_shape,
  std::enable_if_t< Multi_array_shape::fixed_rank!=1 , int > =0
> auto
to_string_1D(const multi_array<Memory_ressource,Multi_array_shape>& x) -> std::string {
  throw;
}
template<
  class Memory_ressource, class Multi_array_shape,
  std::enable_if_t< Multi_array_shape::fixed_rank!=2 , int > =0
> auto
to_string_2D(const multi_array<Memory_ressource,Multi_array_shape>& x) -> std::string {
  throw;
}


template<class Memory_ressource, class Multi_array_shape> auto
to_string(const multi_array<Memory_ressource,Multi_array_shape>& x) -> std::string {
  if (empty(x)) {
    return "[]";
  } 
  if (x.rank()==1) {
    return to_string_1D(x);
  }
  if (x.rank()==2) {
    return to_string_2D(x);
  }
  throw not_implemented_exception("to_string(multi_array) implemented only for rank 1 and 2, not for rank "+std::to_string(x.rank()));
}


} // std_e
