#pragma once


#include "std_e/data_structure/block_range/vblock_iterator.hpp"


#include "std_e/data_structure/jagged_range.hpp"


namespace std_e {


template<class T0, class T1>
using vblock_range = jagged_range<T0,T1,2>;

template<class T0, class T1> constexpr auto
view_as_vblock_range(T0& values, T1& offsets) {
  return vblock_range<T0&,T1&>(values,offsets);
}


} // std_e
