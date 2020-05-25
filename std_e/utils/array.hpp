#pragma once


#include "std_e/base/array.hpp"
#include <algorithm>
#include "std_e/future/algorithm.hpp"
#include "std_e/concept/array.hpp"
#include "std_e/utils/to_string_fwd.hpp"


// TODO rename fixed_size_array.hpp
namespace std_e {


template<int N, class T> constexpr auto
constant_array(const T& x) -> std_e::array<T,N> {
  std_e::array<T,N> constant_a = {};
  std_e::fill(begin(constant_a),end(constant_a),x);
  return constant_a;
}

template<class T, int N> constexpr std_e::array<T,N> default_array = constant_array<N>(T());

template<int N, class T> constexpr auto
default_array_except(int i, const T& x) -> std_e::array<T,N> {
  auto res = default_array<T,N>;
  res[i] = x;
  return res;
}

template<int start, int sub_size, class T, int N> inline auto
make_sub_array(const std_e::array<T,N>& x) {
  std_e::array<T,sub_size> sub;
  std::copy_n(begin(x)+start,sub_size,begin(sub));
  return sub;
}


// same_array_type_except_size {
template<auto N_new, template<class,auto> class Fixed_size_array_template, class T, auto N> constexpr auto
make_same_array_type_except_size(Fixed_size_array_template<T,N>) {
  return Fixed_size_array_template<T,N_new>{};
}
template<class Fixed_size_array, auto N_new>
using same_array_type_except_size = decltype(make_same_array_type_except_size<N_new>(Fixed_size_array{}));
// same_array_type_except_size }


template<class T, size_t N> inline auto
to_string(const std::array<T,N>& x) -> std::string {
  return range_to_string(x);
}
template<class T, int N> inline auto
to_string(const std_e::array<T,N>& x) -> std::string {
  return range_to_string(x);
}


} // std_e
