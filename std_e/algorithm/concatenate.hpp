#pragma once


#include <algorithm>
#include "std_e/multi_index/multi_index_range.hpp"


namespace std_e {


template<class Fwd_it, class Output_it> constexpr auto
// requires Fwd_it::operator* return Fwd_it with value_type==Output_it::value_type
concatenate(Fwd_it first, Fwd_it last, Output_it d_first) -> Output_it {
  while (first!=last) {
    d_first = std::copy(begin(*first),end(*first),d_first);
    ++first;
  }
  return d_first;
}

template<class input_ranges_type, class output_range_type> constexpr auto
// requires input_ranges_type is a Range of Ranges
// requires output_range is a Range
concatenate(const input_ranges_type& in_rs, output_range_type& out_r) -> void {
  std_e::concatenate(begin(in_rs), end(in_rs), begin(out_r));
}


template<class Multi_arrays, class Multi_array> constexpr auto
// requires Multi_arrays is Range of Multi_array
concatenate_on_axis(int axis, const Multi_arrays& in_arrays, Multi_array& out_array) -> void {
  // Preconditions:
  //  - all in_arrays are of the same range
  //  - all in_arrays are of the same extent, except extent(axis)
  //  - cat array is 
  //      - of the sum of in_arrays extent(axis)
  //      - of the extent of in_arrays for all other axes
  int offset = 0;
  for (auto& in_array : in_arrays) {
    auto gen = fortran_multi_index_range(in_array.extent());
    int sz = gen.size();
    for (int i=0; i<sz; ++i, ++gen) {
      const auto& indices = *gen;

      auto cat_indices = indices;
      cat_indices[axis] += offset;

      out_array(cat_indices) = in_array(indices);
    }
    offset += in_array.extent(axis);
  }
}


} // std_e
