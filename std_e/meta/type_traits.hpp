#pragma once


#include <type_traits>


namespace std_e {


// are_integrals {
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
// are_integrals }

// C/C++ functions decay to function pointers when passed as an arguments
template<class F> constexpr bool is_callable = std::is_function_v<std::remove_pointer_t<F>>;


} // std_e
