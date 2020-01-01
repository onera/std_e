#pragma once


#include <algorithm>
#include "std_e/utils/vector.hpp"
#include "std_e/utils/tuple.hpp"


namespace std_e {


template<class... Ts>
class hvector {
  public:
    constexpr
    hvector() = default;

    template<class... Ts0> constexpr
    hvector(std::vector<Ts0>... xs)
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

// TODO Test
//template<class... UTypes>
//hvector(std::vector<UTypes>...) -> hvector<UTypes...>;
template<class T0>
hvector(std::vector<T0>) -> hvector<T0>;
template<class T0, class T1>
hvector(std::vector<T0>,std::vector<T1>) -> hvector<T0,T1>;
template<class T0, class T1, class T2>
hvector(std::vector<T0>,std::vector<T1>,std::vector<T2>) -> hvector<T0,T1,T2>;
template<class T0, class T1, class T2, class T3>
hvector(std::vector<T0>,std::vector<T1>,std::vector<T2>,std::vector<T3>) -> hvector<T0,T1,T2,T3>;


template<class T, class... Ts> constexpr auto
get(hvector<Ts...>& x) -> std::vector<T>& {
  return std::get<std::vector<T>>(x.impl());
}
template<class T, class... Ts> constexpr auto
get(const hvector<Ts...>& x) -> const std::vector<T>& {
  return std::get<std::vector<T>>(x.impl());
}


template<class... Ts, class F> constexpr auto
for_each_vector(hvector<Ts...>& hv, F f) -> void {
  for_each(hv.impl(),f);
}
template<class... Ts, class F> constexpr auto
for_each_vector(const hvector<Ts...>& hv, F f) -> void {
  for_each(hv.impl(),f);
}

// for each element {
template<class... Ts, class F> constexpr auto
for_each_element(hvector<Ts...>& hv, F f) -> void {
  auto f_for_each = [f](auto&& v){ for_each(v,f); };
  for_each_vector(hv,f_for_each);
}
template<class... Ts, class F> constexpr auto
for_each_element(const hvector<Ts...>& hv, F f) -> void {
  auto f_for_each = [f](auto&& v){ for_each(v,f); };
  for_each_vector(hv,f_for_each);
}

// also defining this function for vector, for genericity purposes
template<class T, class F> constexpr auto
for_each_element(std::vector<T>& v, F f) -> void {
  std::for_each(begin(v),end(v),f);
}
template<class T, class F> constexpr auto
for_each_element(const std::vector<T>& v, F f) -> void {
  std::for_each(begin(v),end(v),f);
}
// for each element }


template<class... Ts, class Unary_pred, class F> constexpr auto
find_apply(const hvector<Ts...>& hv, Unary_pred p, F f) -> std::pair<int,int> {
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
for_each_if(const hvector<Ts...>& hv, Unary_pred p, F f) -> void {
  auto f_cond = [p,f](auto x){ if (p(x)) f(x); };
  for_each_element(hv,f_cond);
}


} // std_e
