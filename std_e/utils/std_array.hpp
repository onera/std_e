#pragma once


#include <array>
#include <algorithm>
#include "std_e/future/algorithm.hpp"
#include "std_e/concept/array.hpp"
#include "std_e/utils/to_string_fwd.hpp"


namespace std_e {


template<size_t N, class T> constexpr auto
constant_array(const T& x) -> std::array<T,N> {
  std::array<T,N> constant_a = {};
  std_e::fill(begin(constant_a),end(constant_a),x);
  return constant_a;
}

template<class T, size_t N> constexpr std::array<T,N> default_array = constant_array<N>(T());

template<size_t N, class T> constexpr auto
default_array_except(int i, const T& x) -> std::array<T,N> {
  auto res = default_array<T,N>;
  res[i] = x;
  return res;
}

template<int start, int sub_size, class T, size_t N> inline auto
make_sub_array(const std::array<T,N>& x) {
  std::array<T,sub_size> sub;
  std::copy_n(begin(x)+start,sub_size,begin(sub));
  return sub;
}


template<class T, size_t N> inline auto
to_string(const std::array<T,N>& x) -> std::string {
  return range_to_string(x);
}


} // std_e
