#pragma once


#include <algorithm>
#include "std_e/meta/compile_time_size.hpp"


namespace std_e {


// Note: With GCC 11, the handwritten version for N==3 is ~50% faster than the general one below
//       In fact, this is true "most of the time" (the results are stable, but seemingly unrelated changes affect the timings)
//       See benchmark in __perf_test/rotate_fixed_size
template<class Range, class It> auto
_rotate_optim_3(Range& x, It mid) -> void {
  int i = mid-x.data();
  auto x_rotated_0 = x[i];
  auto x_rotated_1 = x[(i+1)%3];
  auto x_rotated_2 = x[(i+2)%3];
  x[0] = x_rotated_0;
  x[1] = x_rotated_1;
  x[2] = x_rotated_2;
}


// Note: With GCC 11, for N==4, on the contrary, we are ~3 times slower!
//       (maybe because contrary to N==3, %4 can be replaced by a shift)
template<class Range, class It> auto
_rotate_optim_4(Range& x, It mid) -> void {
  int i = mid-x.data();
  auto x_rotated_0 = x[i];
  auto x_rotated_1 = x[(i+1)%4];
  auto x_rotated_2 = x[(i+2)%4];
  auto x_rotated_3 = x[(i+3)%4];
  x[0] = x_rotated_0;
  x[1] = x_rotated_1;
  x[2] = x_rotated_2;
  x[3] = x_rotated_3;
}

// Note: the performance comparisons are weird
//       until we understand them, use the general algorithm
template<class Range, class It> auto
rotate_optim(Range& x, It mid) -> void {
  //if constexpr (compile_time_size<Range> == 3) {
  //  _rotate_optim_3(x,mid);
  //} else if constexpr (compile_time_size<Range> == 4) {
  //  _rotate_optim_4(x,mid);
  //} else {
    std::ranges::rotate(x,mid);
  //}
}


template<class Range> auto
rotate_min_first(Range& x) -> void {
  auto min_pos = std::ranges::min_element(x);
  rotate_optim(x,min_pos);
}


} // std_e
