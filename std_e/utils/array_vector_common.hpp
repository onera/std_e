#pragma once


#include "std_e/future/contract.hpp"
#include "std_e/base/macros.hpp"
#include <algorithm>
#include <vector>
#include "std_e/base/array.hpp"
#include "std_e/future/constexpr_vector.hpp"
#include "std_e/concept/array.hpp"


// The idea of these function is to provide a common interface
// between std::array and std::vector (and more generally, to any Array class)
//  - common creation from a size
//  - copy/fwd from one to the other


namespace std_e {


// make_array_of_size {
template<class Array, class Enable = void>
struct make_array_of_size__impl;

template<class T>
struct make_array_of_size__impl< T , std::enable_if_t<is_fixed_size_array<T>> > {
  static constexpr auto
  func(int sz) -> T {
    constexpr int array_sz = std::tuple_size_v<T>;
    STD_E_ASSERT(sz==array_sz);
    return {};
  }
};

template<class T>
struct make_array_of_size__impl< T , std::enable_if_t<is_dyn_size_array<T>> > {
  template<class Integer> static constexpr auto
  func(Integer sz) -> T {
    return T(sz);
  }
};

template<class Array, class Integer> constexpr auto
make_array_of_size(Integer sz) -> Array {
  return make_array_of_size__impl<Array>::func(sz);
}
// make_array_of_size }


// concatenated_array {
template<class Enable, class... Arrays>
struct concatenated_array__impl;


//template<template<class T, auto sz> class Fixed_size_array_template, auto new_sz>
//struct Same_array_type_but_different_size {
//  using type = Fixed_size_array_template<T
//};


template<class T, int... ct_sizes>
struct concatenated_array__impl< std::enable_if_t<is_fixed_size_array<std_e::array<T,3>>> , std_e::array<T,ct_sizes>... > {
  static constexpr int sum_ct_sizes = (ct_sizes + ...);
  using type = std_e::array<T,sum_ct_sizes>;
};

//template<class Array, class... Arrays>
//struct concatenated_array__impl<
//  std::enable_if_t<is_fixed_size_array<Array>>,
//  Array,Arrays... 
//>
//{
//  using type = Array;
//};

template<class Array, class... Arrays>
struct concatenated_array__impl<
  std::enable_if_t<is_dyn_size_array<Array>>,
  Array,Arrays... 
>
{
  using type = Array;
};

template<class... Arrays> using
concatenated_array = typename concatenated_array__impl<void,Arrays...>::type;
// concatenated_array }




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
template<class Array, class T> constexpr auto
convert_to(std::initializer_list<T> x) -> Array {
  size_t n = x.size();
  Array y = make_array_of_size<Array>(n);;
  std::copy_n(begin(x),n,begin(y));
  return y;
}
// convert_to }


} // std_e
