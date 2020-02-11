#pragma once


#include "std_e/future/contract.hpp"
#include "std_e/base/macros.hpp"
#include <algorithm>
#include <vector>
#include <array>
#include "std_e/future/constexpr_vector.hpp"


// The idea of these function is to provide a common interface
// between std::array and std::vector (and more generally, to any Array class)
//  - common creation from a size
//  - copy/fwd from one to the other


namespace std_e {


// make_array_of_size {
template<class Array>
struct make_array_of_size__impl;


template<class T, size_t ct_size>
struct make_array_of_size__impl<std::array<T,ct_size>> {
  static constexpr auto
  func(size_t sz) -> std::array<T,ct_size> {
    STD_E_ASSERT(sz==ct_size);
    return {};
  }
};

template<class T>
struct make_array_of_size__impl<std::vector<T>> {
  static constexpr auto
  func(size_t sz) -> std::vector<T> {
    return std::vector<T>(sz);
  }
};

template<class T, size_t max_size>
struct make_array_of_size__impl<std_e::constexpr_vector<T,max_size>> {
  static constexpr auto
  func(size_t sz) -> std_e::constexpr_vector<T,max_size> {
    return std_e::constexpr_vector<T,max_size>(sz);
  }
};

template<class Array> constexpr auto
make_array_of_size(size_t sz) -> Array {
  return make_array_of_size__impl<std::decay_t<Array>>::func(sz);
}
// make_array_of_size }


// concatenated_array {
template<class... Arrays>
struct concatenated_array__impl;


template<class T, size_t... ct_sizes>
struct concatenated_array__impl<std::array<T,ct_sizes>...> {
  static constexpr size_t sum_ct_sizes = (ct_sizes + ...);
  using type = std::array<T,sum_ct_sizes>;
};

template<class T, class... Ts>
struct concatenated_array__impl<std::vector<T>,std::vector<Ts>...> {
  using type = std::vector<T>;
};

template<class T, class... Ts, size_t max_size>
struct concatenated_array__impl<std_e::constexpr_vector<T,max_size>,std_e::constexpr_vector<Ts,max_size>...> {
  using type = std_e::constexpr_vector<T,max_size>;
};

template<class... Arrays> using
concatenated_array = typename concatenated_array__impl<Arrays...>::type;
// make_array_of_size }




// convert_to {
/// From array "x" of type Array1, get an array "y" of type Array0
///   - if Array0==Array1, forward x
///   - if Array0!=Array1, copy x into y
template<
  class Array0, class Array1,
  std::enable_if_t< std::is_same<Array0,Array1>::value , int > =0
> constexpr auto
convert_to(Array1&& x) 
 -> Array1&&
{
  return FWD(x);
}
template<
  class Array0, class Array1,
  std::enable_if_t< not std::is_same<Array0,Array1>::value , int > =0
> constexpr auto
convert_to(Array1&& x) 
 -> Array0
{
  size_t n = x.size();
  Array0 y = make_array_of_size<Array0>(n);;
  std::copy_n(begin(x),n,begin(y));
  return y;
}
// convert_to }


} // std_e
