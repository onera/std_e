#pragma once


#include <type_traits>
#include <array>
#include "std_e/base/dynamic_size.hpp"
#include "std_e/future/span.hpp"


namespace std_e {

  
template<class T>
struct compile_time_size_impl {
  static constexpr size_t value = std_e::dynamic_size;
};

template<template<class,size_t> class Array, class T, size_t N>
  requires (std::is_base_of_v<std::array<T,N>,Array<T,N>>)
struct compile_time_size_impl<Array<T,N>> {
  static constexpr size_t value = N;
};
template<template<class,ptrdiff_t> class Span, class T, ptrdiff_t N>
  requires (std::is_base_of_v<std_e::span_base<T,N>,Span<T,N>>)
struct compile_time_size_impl<Span<T,N>> {
  static constexpr size_t value = N;
};

template<class T> constexpr size_t compile_time_size = compile_time_size_impl<T>::value;


} // std_e
