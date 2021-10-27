#pragma once

#include <cstddef>


namespace std_e {


// get_type {
template<int I, class... Ts>
struct get_type__impl;

template<int I, class T, class... Ts>
struct get_type__impl<I,T,Ts...> {
  using type = typename get_type__impl<I-1,Ts...>::type;
};

template<class T>
struct get_type__impl<0,T> {
  using type = T;
};

template<int I, class... Ts> using get_type = typename get_type__impl<I,Ts...>::type;
// get_type }


// types {
/// types is a lightweight equivalent to std::tuple for type computations
template<class... Ts>
struct types {
  static constexpr size_t size = sizeof...(Ts);
  template<int I> using get = get_type<I,Ts...>;
};

template<int I, class Types>
using types_element_t = typename Types::template get<I>;
// types }



}
