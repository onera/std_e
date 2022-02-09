#pragma once


#include <tuple>
#include <type_traits>
#include "std_e/utils/to_string_fwd.hpp"
#include "std_e/base/macros.hpp"
#include "std_e/meta/pack.hpp"


namespace std_e {


// index_of_type_in_tuple {
// https://stackoverflow.com/a/18063608/1583122
template <class T, class Tuple>
struct index_of_type_in_tuple__impl;

template <class T, class... Types>
struct index_of_type_in_tuple__impl<T, std::tuple<T, Types...>> {
  static constexpr int value = 0;
};

template <class T, class U, class... Types>
struct index_of_type_in_tuple__impl<T, std::tuple<U, Types...>> {
  static constexpr int value = 1 + index_of_type_in_tuple__impl<T, std::tuple<Types...>>::value;
};

template <class T, class Tuple>
constexpr int index_of_type_in_tuple = index_of_type_in_tuple__impl<T,Tuple>::value;
// index_of_type_in_tuple }


// tuple_contains_type {
// https://stackoverflow.com/a/25958302/1583122
template <typename T, typename Tuple>
struct tuple_contains_type__impl;

template <typename T>
struct tuple_contains_type__impl<T, std::tuple<>> {
  static constexpr bool value = false;
};

template <typename T, typename... Ts>
struct tuple_contains_type__impl<T, std::tuple<T, Ts...>> {
  static constexpr bool value = true;
};

template <typename T, typename U, typename... Ts>
struct tuple_contains_type__impl<T, std::tuple<U, Ts...>> {
  static constexpr bool value = tuple_contains_type__impl<T, std::tuple<Ts...>>::value;
};

template <typename T, typename Tuple>
constexpr bool tuple_contains_type = tuple_contains_type__impl<T, Tuple>::value;
// tuple_contains_type }


// https://stackoverflow.com/questions/1198260/how-can-you-iterate-over-the-elements-of-an-stdtuple
// shut down GCC 9 erroneous warning because of "if constexpr"
template<int I, class tuple_type, class F> constexpr auto
for_each__impl_tuple([[maybe_unused]] tuple_type& x, [[maybe_unused]] F f) -> void {
  constexpr int sz = std::tuple_size_v<std::decay_t<tuple_type>>;
  if constexpr (I<sz) {
    f(std::get<I>(x));
    for_each__impl_tuple<I+1>(x,f);
  }
}

template<class... Ts, class F> constexpr auto
for_each(std::tuple<Ts...>& x, F f) -> void {
  for_each__impl_tuple<0>(x,f);
}
template<class... Ts, class F> constexpr auto
for_each(const std::tuple<Ts...>& x, F f) -> void {
  for_each__impl_tuple<0>(x,f);
}

// implementations for std::array (follows the tuple protocol)
template<class T, std::size_t N, class F> auto
for_each(std::array<T,N>& x, F f) {
  for_each__impl_tuple<0>(x,f);
}
template<class T, std::size_t N, class F> auto
for_each(const std::array<T,N>& x, F f) {
  for_each__impl_tuple<0>(x,f);
}




template<class... Ts, class T, class bin_op> constexpr auto
accumulate(const std::tuple<Ts...>& x, T& init, bin_op op) -> void {
  auto acc = [&init,op](const auto& val){ init = op(init,val); };
  for_each(x,acc);
}



template<int I, class tuple_type, class Unary_pred, class F>  constexpr auto
find_apply__impl([[maybe_unused]] tuple_type& x, [[maybe_unused]] Unary_pred p, [[maybe_unused]] F f) -> int {
  constexpr int sz = std::tuple_size_v<std::decay_t<tuple_type>>;
  if constexpr (I<sz) {
    if (p(std::get<I>(x))) {
      f(std::get<I>(x));
      return I;
    } else {
      return find_apply__impl<I+1>(x,p,f);
    }
  }
  return sz;
}

template<class... Ts, class Unary_pred, class F> constexpr auto
find_apply(std::tuple<Ts...>& x, Unary_pred p, F f) -> int {
  return find_apply__impl<0>(x,p,f);
}
template<class... Ts, class Unary_pred, class F> constexpr auto
find_apply(const std::tuple<Ts...>& x, Unary_pred p, F f) -> int {
  return find_apply__impl<0>(x,p,f);
}


template<int I, class tuple_type, class F>  constexpr auto
for_each_until__impl_tuple([[maybe_unused]] tuple_type& x,[[maybe_unused]] F f) -> int {
  constexpr int sz = std::tuple_size_v<std::decay_t<tuple_type>>;
  if constexpr (I<sz) {
    if (f(std::get<I>(x))) {
      return I;
    } else {
      return for_each_until__impl_tuple<I+1>(x,f);
    }
  }
  return sz;
}

template<class... Ts, class F> constexpr auto
for_each_until(std::tuple<Ts...>& x, F f) -> decltype(auto) {
  return for_each_until__impl_tuple<0>(x,f);
}
template<class... Ts, class F> constexpr auto
for_each_until(const std::tuple<Ts...>& x, F f) -> decltype(auto) {
  return for_each_until__impl_tuple<0>(x,f);
}

template<class... Ts, size_t... Is> auto
to_string__impl(const std::tuple<Ts...>& x, std::index_sequence<Is...>) -> std::string {
  using std::to_string;
  using std_e::to_string;
  std::vector<std::string> s = {to_string(std::get<Is>(x))...};
  return range_to_string(s,"<","|",">");
}
template<class... Ts> auto
to_string(const std::tuple<Ts...>& x) -> std::string {
  return to_string__impl(x,std::make_index_sequence<sizeof...(Ts)>{});
}




// transform {
template<int I, class tuple_type, class F, class return_type> constexpr auto
transform_impl([[maybe_unused]] tuple_type& x, [[maybe_unused]] F f, [[maybe_unused]] return_type& ret) -> void {
  constexpr int sz = std::tuple_size_v<std::decay_t<tuple_type>>;
  if constexpr (I<sz) {
    std::get<I>(ret) = f(std::get<I>(x));
    transform_impl<I+1>(x,f,ret);
  }
}

template<class T, class... Ts, class F> constexpr auto
transform(std::tuple<T,Ts...>& x, F f) {
  constexpr auto N = 1+sizeof...(Ts);
  using first_return_type = decltype(f(std::declval<T&>()));
  constexpr bool same_return_type = are_all_of<first_return_type, decltype(f(std::declval<Ts&>()))...>;
  if constexpr (same_return_type) {
    if constexpr (std::is_void_v<first_return_type>) {
      return for_each(x,f);
    } else {
      using return_type = std::array<first_return_type,N>;
      return_type ret;
      transform_impl<0>(x,f,ret);
      return ret;
    }
  } else {
    using return_type = std::tuple<first_return_type,decltype(f(std::declval<Ts&>()))...>;
    return_type ret;
    transform_impl<0>(x,f,ret);
    return ret;
  }
}
template<class T, class... Ts, class F> constexpr auto
transform(const std::tuple<T,Ts...>& x, F f) {
  constexpr auto N = 1+sizeof...(Ts);
  using first_return_type = decltype(f(std::declval<const T&>()));
  constexpr bool same_return_type = are_all_of<first_return_type, decltype(f(std::declval<const Ts&>()))...>;
  if constexpr (same_return_type) {
    if constexpr (std::is_void_v<first_return_type>) {
      return for_each(x,f);
    } else {
      using return_type = std::array<first_return_type,N>;
      return_type ret;
      transform_impl<0>(x,f,ret);
      return ret;
    }
  } else {
    using return_type = std::tuple<first_return_type,decltype(f(std::declval<const Ts&>()))...>;
    return_type ret;
    transform_impl<0>(x,f,ret);
    return ret;
  }
}
// transform }


} // std_e
