#pragma once

#include <algorithm>
#include "std_e/utils/tuple.hpp"

namespace std_e {

template<template<class> class Range_template, class... Ts>
class hrange {
  public:
    /** @name type traits */
    ///@{
    using ranges_tuple = std::tuple<Range_template<Ts>...>;
    ///@}

    /** @name ctors */
    ///@{
    constexpr
    hrange() = default;

    template<class... Ranges> constexpr
    // requires Ts0 == Ts
    hrange(Ranges&&... xs)
      : _impl(std::make_tuple(FWD(xs)...))
    {}
    ///@}

    /** @name access to underlying tuple */
    ///@{
    constexpr auto impl()       ->       ranges_tuple& { return _impl; }
    constexpr auto impl() const -> const ranges_tuple& { return _impl; }
    ///@}

    /** @name low-level access */
    ///@{
    /// number of types
    static constexpr auto
    hsize() -> int {
      return sizeof...(Ts);
    }
    /// total number of elements
    constexpr auto
    size() const -> size_t {
      size_t sz = 0;
      auto accumulate_size = [&sz](const auto& v) { sz += v.size(); };
      for_each(_impl,accumulate_size);
      return sz;
    }
    ///@}

    /** @name vector-like interface */
    ///@{
    template<class T> auto
    // requires T is one of the Ts
    push_back(const T& elt) {
      std::get<Range_template<T>>(_impl).push_back(elt);
    }
    ///@}
  private:
    ranges_tuple _impl;
};


/** @name tuple-like get */
///@{
template<class T, template<class> class RT, class... Ts> constexpr auto
get(hrange<RT,Ts...>& x) -> RT<T>& {
  return std::get<RT<T>>(x.impl());
}
template<class T, template<class> class RT, class... Ts> constexpr auto
get(const hrange<RT,Ts...>& x) -> const RT<T>& {
  return std::get<RT<T>>(x.impl());
}

template<size_t I, template<class> class RT, class... Ts> constexpr auto
get(hrange<RT,Ts...>& x) -> auto& {
  return std::get<I>(x.impl());
}
template<size_t I, template<class> class RT, class... Ts> constexpr auto
get(const hrange<RT,Ts...>& x) -> const auto& {
  return std::get<I>(x.impl());
}
///@}

/// @name tuple protocol
///@{
} // std_e
namespace std {
  template<template<class> class RT, class... Ts>
  struct tuple_size<std_e::hrange<RT,Ts...>> {
    using ranges_tuple = typename std_e::hrange<RT,Ts...>::ranges_tuple;
    static constexpr size_t value = tuple_size_v<ranges_tuple>;
  };
  template<size_t I, template<class> class RT, class... Ts>
  struct tuple_element<I, std_e::hrange<RT,Ts...>> {
    using ranges_tuple = typename std_e::hrange<RT,Ts...>::ranges_tuple;
    using type = tuple_element_t<I, ranges_tuple>;
  };
} // std
namespace std_e {

template <class F, template<class> class RT, class... Ts> constexpr auto
apply(F&& f, hrange<RT,Ts...>& x) -> decltype(auto) {
  return std::apply(f, x.impl());
}
template <class F, template<class> class RT, class... Ts> constexpr auto
apply(F&& f, const hrange<RT,Ts...>& x) -> decltype(auto) {
  return std::apply(f, x.impl());
}
///@}

/// @name algorithms
///@{
template<class... Ts, template<class> class RT, class F> constexpr auto
for_each_range(hrange<RT,Ts...>& hr, F f) -> void {
  for_each(hr.impl(),f);
}
template<class... Ts, template<class> class RT, class F> constexpr auto
for_each_range(const hrange<RT,Ts...>& hr, F f) -> void {
  for_each(hr.impl(),f);
}

template<class... Ts, template<class> class RT, class F> constexpr auto
for_each_element(hrange<RT,Ts...>& hr, F f) -> void {
  auto f_for_each = [f](auto& v){ for_each(v,f); };
  for_each_vector(hr,f_for_each);
}
template<class... Ts, template<class> class RT, class F> constexpr auto
for_each_element(const hrange<RT,Ts...>& hr, F f) -> void {
  auto f_for_each = [f](auto& v){ for_each(v,f); };
  for_each_vector(hr,f_for_each);
}

// also defining this function for vector, for genericity purposes // TODO define for any Range
template<class T, class F> constexpr auto
for_each_element(std::vector<T>& v, F f) -> void {
  std::for_each(begin(v),end(v),f);
}
template<class T, class F> constexpr auto
for_each_element(const std::vector<T>& v, F f) -> void {
  std::for_each(begin(v),end(v),f);
}
///@}


namespace detail {
template<class hrange_type, class Unary_pred, class F> constexpr auto
find_apply(hrange_type& hr, Unary_pred p, F f) -> std::pair<int,int> {
  int index_in_range = 0;
  auto f_tuple = [&p,&f,&index_in_range](auto& rng){
    auto it = std::find_if(begin(rng),end(rng),p);
    index_in_range = it-begin(rng);
    if (it!=end(rng)) {
      f(*it);
      return true;
    } else {
      return false;
    }
  };

  int index_in_types = for_each_until(hr.impl(),f_tuple);
  return std::make_pair(index_in_types,index_in_range);
}
} // detail


template<template<class> class RT, class... Ts, class Unary_pred, class F> constexpr auto
find_apply(hrange<RT,Ts...>& hr, Unary_pred p, F f) -> std::pair<int,int> {
  return detail::find_apply(hr,p,f);
}
template<template<class> class RT, class... Ts, class Unary_pred, class F> constexpr auto
find_apply(const hrange<RT,Ts...>& hr, Unary_pred p, F f) -> std::pair<int,int> {
  return detail::find_apply(hr,p,f);
}

template<template<class> class RT, class... Ts, class Unary_pred> constexpr auto
find_position(const hrange<RT,Ts...>& hr, Unary_pred p) -> std::pair<int,int> {
  auto no_op = [](auto&&){};
  return detail::find_apply(hr,p,no_op);
}

template<template<class> class RT, class... Ts, class Unary_pred, class F> constexpr auto
apply_first(hrange<RT,Ts...>& hr, Unary_pred p, F f) -> bool {
  auto [index_in_types,_] = find_apply(hr,p,f);
  return index_in_types != hr.hsize();
}
template<template<class> class RT, class... Ts, class Unary_pred, class F> constexpr auto
apply_first(const hrange<RT,Ts...>& hr, Unary_pred p, F f) -> bool {
  auto [index_in_types,_] = find_apply(hr,p,f);
  return index_in_types != hr.hsize();
}


template<template<class> class RT, class... Ts, class Unary_pred, class F> constexpr auto
for_each_element_if(hrange<RT,Ts...>& hr, Unary_pred p, F f) -> void {
  auto f_cond = [p,f](auto&& x){ if (p(x)) f(x); };
  for_each_element(hr,f_cond);
}
template<template<class> class RT, class... Ts, class Unary_pred, class F> constexpr auto
for_each_element_if(const hrange<RT,Ts...>& hr, Unary_pred p, F f) -> void {
  auto f_cond = [p,f](auto&& x){ if (p(x)) f(x); };
  for_each_element(hr,f_cond);
}


} // std_e

