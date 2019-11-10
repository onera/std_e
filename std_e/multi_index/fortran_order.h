#pragma once


#include "std_e/future/contract.h"
#include "std_e/multi_index/concept.h"
#include "std_e/multi_index/multi_index.h"


namespace std_e {


template<class Multi_index_0, class Multi_index_1 = Multi_index_0> FORCE_INLINE constexpr auto
fortran_order_from_dimensions(const Multi_index_0& dimensions, const Multi_index_1& indices) -> int {
  std_e::ASSERT(dimensions.size()==indices.size());
  int res = indices[0];
  int stride = dimensions[0];
  for (size_t k=1; k<dimensions.size(); ++k) {
    res += indices[k] * stride;
    stride *= dimensions[k];
  }
  return res;
}
template<class Multi_index_0, class Multi_index_1, class Multi_index_2 = Multi_index_0> FORCE_INLINE constexpr auto
fortran_order_from_dimensions(const Multi_index_0& dimensions, const Multi_index_1& offsets, const Multi_index_2& indices) -> int {
  std_e::ASSERT(dimensions.size()==indices.size());
  std_e::ASSERT(dimensions.size()==offsets.size());
  int res = offsets[0] + indices[0];
  int stride = dimensions[0];
  for (size_t k=1; k<dimensions.size(); ++k) {
    res += (offsets[k] + indices[k]) * stride;
    stride *= dimensions[k];
  }
  return res;
}


// Note: 
//  - The stride of the first dimension is always 1, so it is skipped.
//  - Thus, if strides = fortran_strides_from_extent(dims), then strides[0] is actually the second stride
//  - The last stride is the total size
template<class Multi_index> FORCE_INLINE constexpr auto 
fortran_strides_from_extent(const Multi_index& dims) -> Multi_index {
  auto strides = create_multi_index<Multi_index>(dims.size());
  strides[0] = dims[0];
  for (size_t i=1; i<dims.size(); ++i) {
    strides[i] = strides[i-1]*dims[i];
  }
  return strides;
}


template<class Multi_index_0, class Multi_index_1 = Multi_index_0> FORCE_INLINE constexpr auto
fortran_order_from_strides(const Multi_index_0& strides, const Multi_index_1& indices) -> int {
  // Precondition:
  //  - The stride of the first dimension is always 1, so it is skipped.
  //  - Thus, if strides = fortran_strides_from_extent(dims), then strides[0] is actually the second stride
  //  - The last stride is the total size
  std_e::ASSERT(strides.size()==indices.size());
  int res = indices[0]; // * 1   NOTE: this is (from measurements) an IMPORTANT optimization
  for (size_t k=1; k<strides.size(); ++k) {
    res += indices[k] * strides[k-1];
  }
  return res;
}


} // std_e
