#pragma once


namespace std_e {


// is_one_of {
template<class T, class... Ts>
struct is_one_of_impl;

template<class T>
struct is_one_of_impl<T> {
  static constexpr bool value = false;
};
template<class T, class... Ts>
struct is_one_of_impl<T,T,Ts...> {
  static constexpr bool value = true;
};
template<class T, class T0, class... Ts>
struct is_one_of_impl<T,T0,Ts...> {
  static constexpr bool value = is_one_of_impl<T,Ts...>::value;
};

template<class T, class... Ts> constexpr bool is_one_of = is_one_of_impl<T,Ts...>::value;
// is_one_of }


// are_all_of {
template<class T, class... Ts>
struct are_all_of_impl {
  static constexpr bool value = true;
};

template<class T, class T0, class... Ts>
struct are_all_of_impl<T,T0,Ts...> {
  static constexpr bool value = std::is_same_v<T,T0> && are_all_of_impl<T,Ts...>::value;
};

template<class T, class... Ts> constexpr bool are_all_of = are_all_of_impl<T,Ts...>::value;
// are_all_of }


} // std_e
