#pragma once


#include <algorithm>
#include "std_e/utils/vector.hpp"
#include "std_e/utils/tuple.hpp"


namespace std_e {


template<class... Ts>
class heterogenous_vector {
  public:
    constexpr
    heterogenous_vector() = default;

    template<class... Ts0> constexpr
    heterogenous_vector(std::vector<Ts0>... xs)
      : _impl({std::move(xs)...})
    {}

    constexpr auto
    impl() -> auto& {
      return _impl;
    }
    constexpr auto
    impl() const -> const auto& {
      return _impl;
    }

    constexpr auto
    size() const -> size_t {
      size_t sz = 0;
      auto accumulate_size = [&sz](const auto& v) { sz += v.size(); };
      for_each(_impl,accumulate_size);
      return sz;
    }
  private:
    std::tuple<std::vector<Ts>...> _impl;
};

template<class... Ts>
using hvector = heterogenous_vector<Ts...>;


template<class T, class... Ts> constexpr auto
get(heterogenous_vector<Ts...>& x) -> std::vector<T>& {
  return std::get<std::vector<T>>(x.impl());
}
template<class T, class... Ts> constexpr auto
get(const heterogenous_vector<Ts...>& x) -> const std::vector<T>& {
  return std::get<std::vector<T>>(x.impl());
}


template<class... Ts, class F> constexpr auto
for_each_vector(heterogenous_vector<Ts...>& hv, F f) -> void {
  for_each(hv.impl(),f);
}
template<class... Ts, class F> constexpr auto
for_each_vector(const heterogenous_vector<Ts...>& hv, F f) -> void {
  for_each(hv.impl(),f);
}

template<class... Ts, class F> constexpr auto
for_each_element(heterogenous_vector<Ts...>& hv, F f) -> void {
  auto f_for_each = [f](auto&& v){ for_each(v,f); };
  for_each_vector(hv,f_for_each);
}
template<class... Ts, class F> constexpr auto
for_each_element(const heterogenous_vector<Ts...>& hv, F f) -> void {
  auto f_for_each = [f](auto&& v){ for_each(v,f); };
  for_each_vector(hv,f_for_each);
}


template<class... Ts, class Unary_pred, class F> constexpr auto
find_apply(const heterogenous_vector<Ts...>& hv, Unary_pred p, F f) -> std::pair<int,int> {
  int pos_in_vec = 0;
  auto f_tuple = [&p,&f,&pos_in_vec](auto&& vec){
    auto it = std::find_if(begin(vec),end(vec),p);
    pos_in_vec = it-begin(vec);
    if (it!=end(vec)) {
      f(*it);
      return true;
    } else {
      return false;
    }
  };

  int pos_in_tuple = for_each_until(hv.impl(),f_tuple);
  return std::make_pair(pos_in_tuple,pos_in_vec);
}


template<class... Ts, class Unary_pred, class F> constexpr auto
for_each_if(const heterogenous_vector<Ts...>& hv, Unary_pred p, F f) -> void {
  auto f_cond = [p,f](auto x){ if (p(x)) f(x); };
  for_each_element(hv,f_cond);
}


} // std_e
