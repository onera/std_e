#pragma once


#include <array>
#include "std_e/future/algorithm.h"
#include "std_e/base/index_t.h"


namespace std_e {


template<size_t N, class T> constexpr auto
constant_array(const T& x) -> std::array<T,N> {
  std::array<T,N> constant_a = {};
  std_e::fill(begin(constant_a),end(constant_a),x);
  return constant_a;
}

template<class T, size_t N> constexpr std::array<T,N> default_array = constant_array<N>(T());

template<size_t N, class T> constexpr auto
default_array_except(index_t i, const T& x) -> std::array<T,N> {
  auto res = default_array<T,N>;
  res[i] = x;
  return res;
}


} // std_e
