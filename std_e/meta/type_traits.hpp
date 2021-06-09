#pragma once


#include <type_traits>


namespace std_e {


template<class... Ts> constexpr bool are_integral = std::conjunction_v< std::is_integral<Ts> ...>;

// C/C++ functions decay to function pointers when passed as an arguments
template<class F> constexpr bool is_callable = std::is_function_v<std::remove_pointer_t<F>>;


} // std_e
