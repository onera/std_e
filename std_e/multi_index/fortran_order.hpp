#pragma once


#include "std_e/future/contract.hpp"
#include "std_e/multi_index/concept.hpp"
#include "std_e/utils/array_vector_common.hpp"
// TODO RENAME file, test (offsets!)


namespace std_e {


template<class Multi_index_0, class Multi_index_1> FORCE_INLINE constexpr auto
// requires std_e::size<Multi_index_0> == std_e::size<Multi_index_1>
fortran_order_from_dimensions(const Multi_index_0& dimensions, const Multi_index_1& indices) -> int {
  STD_E_ASSERT(dimensions.size()>0);
  STD_E_ASSERT(dimensions.size()==indices.size());
  int rank = dimensions.size();
  int res = indices[0];
  int stride = dimensions[0];
  for (int k=1; k<rank; ++k) {
    res += indices[k] * stride;
    stride *= dimensions[k];
  }
  return res;
}
// case with potential dims.size==0 TODO can it be merged into fortran_order_from_dimensions without loss of performance?
template<class T0, class T1> constexpr auto
fortran_index(const T0& dims, const T1& indices) {
  if (dims.size()==0) return 0;
  else return fortran_order_from_dimensions(dims,indices);
}

template<class Multi_index_0, class Multi_index_1, class Multi_index_2> FORCE_INLINE constexpr auto
// requires std_e::size<Multi_index_0> == std_e::size<Multi_index_1>
// requires std_e::size<Multi_index_0> == std_e::size<Multi_index_2>
fortran_order_from_dimensions(const Multi_index_0& dimensions, const Multi_index_1& offsets, const Multi_index_2& indices) -> int {
  STD_E_ASSERT(dimensions.size()==indices.size());
  STD_E_ASSERT(dimensions.size()==offsets.size());
  int rank = dimensions.size();
  int res = offsets[0]+indices[0];
  int stride = dimensions[0];
  for (int k=1; k<rank; ++k) {
    res += (offsets[k] + indices[k]) * stride;
    stride *= dimensions[k];
  }
  return res;
}


//template<class Multi_index_0, class Multi_index_1> FORCE_INLINE constexpr auto
//// requires std_e::size<Multi_index_0> == std_e::size<Multi_index_1>
//c_order_from_dimensions(const Multi_index_0& dimensions, const Multi_index_1& indices) -> int {
//  STD_E_ASSERT(dimensions.size()==indices.size());
//  int last_index = dimensions.size()-1;
//  int res = indices[last_index];
//  int stride = dimensions[last_index];
//  for (int k=last_index-1; k>=0; --k) {
//    res += indices[k] * stride;
//    stride *= dimensions[k];
//  }
//  return res;
//}
template<class Multi_index_0, class Multi_index_1, class Multi_index_2> FORCE_INLINE constexpr auto
// requires std_e::size<Multi_index_0> == std_e::size<Multi_index_1>
// requires std_e::size<Multi_index_0> == std_e::size<Multi_index_2>
c_order_from_dimensions(const Multi_index_0& dimensions, const Multi_index_1& offsets, const Multi_index_2& indices) -> int {
  STD_E_ASSERT(dimensions.size()==indices.size());
  STD_E_ASSERT(dimensions.size()==offsets.size());
  int last_index = dimensions.size()-1;
  int res = offsets[last_index]+indices[last_index];
  int stride = dimensions[last_index];
  for (int k=last_index-1; k>=0; --k) {
    res += (offsets[k] + indices[k]) * stride;
    stride *= dimensions[k];
  }
  return res;
}


// Note: the stride first dimension stride is always 1, so it is skipped.
// The last stride is the total size
template<class Multi_index> FORCE_INLINE constexpr auto 
fortran_strides_from_extent(const Multi_index& dims) -> Multi_index {
  int rank = dims.size();
  auto strides = make_array_of_size<Multi_index>(rank);
  strides[0] = dims[0];
  for (int i=1; i<rank; ++i) {
    strides[i] = strides[i-1]*dims[i];
  }
  return strides;
}


template<class Multi_index_0, class Multi_index_1> FORCE_INLINE constexpr auto
fortran_order_from_strides(const Multi_index_0& strides, const Multi_index_1& indices) -> int {
  // Precondition:
  //  - The stride of the first dimension is always 1, so it is skipped.
  //  - Thus, if strides = fortran_strides_from_extent(dims), then strides[0] is actually the second stride
  //  - The last stride is the total size
  STD_E_ASSERT(strides.size()==indices.size());
  int res = indices[0]; // * 1   NOTE: this is (from measurements) an IMPORTANT optimization
  for (size_t k=1; k<strides.size(); ++k) {
    res += indices[k] * strides[k-1];
  }
  return res;
}


} // std_e
