#pragma once


#include <tuple>

// https://codereview.stackexchange.com/questions/193420/apply-a-function-to-each-element-of-a-tuple-map-a-tuple
template <typename Fn, typename tuple_type, std::size_t... Ns>
auto tuple_transform_impl(Fn&& fn, tuple_type&& args, std::index_sequence<Ns...>) {
  if constexpr (sizeof...(Ns) == 0){
    return std::tuple<>(); // empty tuple
  } else if constexpr (std::is_same_v<decltype(fn(std::get<0>(args))), void>) {
    (fn(std::get<Ns>(args)),...); // no return value expected
    return;
  } else if constexpr (std::is_lvalue_reference_v<decltype(fn(std::get<0>(args)))>) {
    // then dispatch lvalue, rvalue ref, temporary
    return std::tie(fn(std::get<Ns>(args))...);
  } else if constexpr (std::is_rvalue_reference_v<decltype(fn(std::get<0>(args)))>) {
    return std::forward_as_tuple(fn(std::get<Ns>(args))...);
  } else {
    return std::tuple(fn(std::get<Ns>(args))...);
  }
}

template <typename Fn, typename... Ts>
auto tuple_transform(Fn&& fn, const std::tuple<Ts...>& tuple) {
  return tuple_transform_impl(std::forward<Fn>(fn), tuple,
                              std::make_index_sequence<sizeof...(Ts)>());
}

template <typename Fn, typename... Ts>
auto tuple_transform(Fn&& fn, std::tuple<Ts...>& tuple) {
  return tuple_transform_impl(std::forward<Fn>(fn), tuple,
                              std::make_index_sequence<sizeof...(Ts)>());
}
