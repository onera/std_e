#pragma once


#include <type_traits>


namespace std_e {


template<class... Ts>
struct are_integrals__impl;

template<>
struct are_integrals__impl<> {
  static constexpr bool value = true;
};

template<class T, class... Ts>
struct are_integrals__impl<T,Ts...> {
  static constexpr bool value = std::is_integral_v<T> && are_integrals__impl<Ts...>::value;
};

template<class... Ts> constexpr bool are_integrals = are_integrals__impl<Ts...>::value;


} // std_e
