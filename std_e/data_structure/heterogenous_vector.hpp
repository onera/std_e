#pragma once

#include <algorithm>
#include "std_e/utils/vector.hpp"
#include "std_e/utils/tuple.hpp"

namespace std_e {

/**
 *  \brief hvector<Ts...> is a wrapper around tuple<vector<Ts...>>
 *  with convenient accessors and algorithms.
 */
template<class... Ts>
class hvector {
  public:
  // type traits
    using impl_type = std::tuple<std::vector<Ts>...>;

  // ctors
    constexpr
    hvector() = default;

    template <class... Ts0> constexpr
    // requires Ts0 == Ts
    hvector(std::vector<Ts0>... xs)
      : _impl({std::move(xs)...}){}

  // low-level access
    constexpr auto impl()       ->       impl_type& { return _impl; }
    constexpr auto impl() const -> const impl_type& { return _impl; }

  // size
    //! \brief Heterogenous size: number of different types
    static constexpr auto
    hsize() -> int {
      return sizeof...(Ts);
    }

    //! \brief Total size: sum(vector.size())
    constexpr auto
    size() const -> size_t {
      size_t sz = 0;
      auto accumulate_size = [&sz](const auto& v) { sz += v.size(); };
      for_each(_impl,accumulate_size);
      return sz;
    }

  // vector-like interface
    template<class T> auto
    // requires T is one of the Ts
    push_back(const T& elt) {
      std::get<std::vector<T>>(_impl).push_back(elt);
    }
  private:
    impl_type _impl;
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

template<size_t I, class... Ts> constexpr auto
get(hvector<Ts...>& x) -> auto& {
  return std::get<I>(x.impl());
}
template<size_t I, class... Ts> constexpr auto
get(const hvector<Ts...>& x) -> const auto& {
  return std::get<I>(x.impl());
}

// tuple protocol {
} // std_e
namespace std {
  template<class... Ts>
  struct tuple_size<std_e::hvector<Ts...>> {
    using impl_type = typename std_e::hvector<Ts...>::impl_type;
    static constexpr size_t value = tuple_size_v<impl_type>;
  };
  template<size_t I, class... Ts>
  struct tuple_element<I, std_e::hvector<Ts...>> {
    using impl_type = typename std_e::hvector<Ts...>::impl_type;
    using type = tuple_element_t<I, impl_type>;
  };
} // std

// TODO DEL or RENAME (not clear if we apply to vectors or elts in vectors
namespace std_e {
template <class F, class... Ts> constexpr auto
apply(F&& f, hvector<Ts...>& x) -> decltype(auto) {
  return std::apply(f, x.impl());
}
template <class F, class... Ts> constexpr auto
apply(F&& f, const hvector<Ts...>& x) -> decltype(auto) {
  return std::apply(f, x.impl());
}
// tuple protocol }

template<class... Ts, class F> constexpr auto
for_each_vector(hvector<Ts...>& hv, F f) -> void {
  for_each(hv.impl(),f);
}
template<class... Ts, class F> constexpr auto
for_each_vector(const hvector<Ts...>& hv, F f) -> void {
  for_each(hv.impl(),f);
}

template<class... Ts, class F> constexpr auto
for_each_element(hvector<Ts...>& hv, F f) -> void {
  auto f_for_each = [f](auto& v){ for_each(v,f); };
  for_each_vector(hv,f_for_each);
}
template<class... Ts, class F> constexpr auto
for_each_element(const hvector<Ts...>& hv, F f) -> void {
  auto f_for_each = [f](auto& v){ for_each(v,f); };
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


template<class hvector_type, class Unary_pred, class F> constexpr auto
find_apply__impl(hvector_type& hv, Unary_pred p, F f) -> std::pair<int,int> {
  int index_in_vector = 0;
  auto f_tuple = [&p,&f,&index_in_vector](auto& vec){
    auto it = std::find_if(begin(vec),end(vec),p);
    index_in_vector = it-begin(vec);
    if (it!=end(vec)) {
      f(*it);
      return true;
    } else {
      return false;
    }
  };

  int index_in_types = for_each_until(hv.impl(),f_tuple);
  return std::make_pair(index_in_types,index_in_vector);
}
// TODO choose between find_apply__impl and find_apply__impl2
template<class hvector_type, class Unary_pred, class F> constexpr auto
find_apply__impl2(hvector_type& hv, Unary_pred p, F f) -> std::pair<int,int> {
  int index_in_vector = 0;
  auto p_tuple = [&p,&index_in_vector](auto& vec){
    auto it = std::find_if(begin(vec),end(vec),p);
    index_in_vector = it-begin(vec);
    if (it!=end(vec)) {
      return true;
    } else {
      return false;
    }
  };
  auto f_tuple = [&f,&index_in_vector](auto& vec){
    f(vec[index_in_vector]);
  };

  int index_in_types = find_apply(hv.impl(),p_tuple,f_tuple);
  return std::make_pair(index_in_types,index_in_vector);
}


template<class... Ts, class Unary_pred, class F> constexpr auto
find_apply(hvector<Ts...>& hv, Unary_pred p, F f) -> std::pair<int,int> {
  return find_apply__impl(hv,p,f);
}
template<class... Ts, class Unary_pred, class F> constexpr auto
find_apply(const hvector<Ts...>& hv, Unary_pred p, F f) -> std::pair<int,int> {
  return find_apply__impl(hv,p,f);
}

template<class... Ts, class Unary_pred> constexpr auto
find_position(const hvector<Ts...>& hv, Unary_pred p) -> std::pair<int,int> {
  auto no_op = [](auto&&){};
  return find_apply__impl(hv,p,no_op);
}

template<class... Ts, class Unary_pred, class F> constexpr auto
apply_first(hvector<Ts...>& hv, Unary_pred p, F f) -> bool {
  auto [index_in_types,_] = find_apply(hv,p,f);
  return index_in_types != hv.hsize();
}
template<class... Ts, class Unary_pred, class F> constexpr auto
apply_first(const hvector<Ts...>& hv, Unary_pred p, F f) -> bool {
  auto [index_in_types,_] = find_apply(hv,p,f);
  return index_in_types != hv.hsize();
}


template<class... Ts, class Unary_pred, class F> constexpr auto
for_each_element_if(hvector<Ts...>& hv, Unary_pred p, F f) -> void {
  auto f_cond = [p,f](auto&& x){ if (p(x)) f(x); };
  for_each_element(hv,f_cond);
}
template<class... Ts, class Unary_pred, class F> constexpr auto
for_each_element_if(const hvector<Ts...>& hv, Unary_pred p, F f) -> void {
  auto f_cond = [p,f](auto&& x){ if (p(x)) f(x); };
  for_each_element(hv,f_cond);
}

// TODO REMOVE
template<class... Ts, class Unary_pred, class F> [[deprecated("use for_each_element_if instead")]] constexpr auto
for_each_if(hvector<Ts...>& hv, Unary_pred p, F f) -> void {
  auto f_cond = [p,f](auto&& x){ if (p(x)) f(x); };
  for_each_element(hv,f_cond);
}
template<class... Ts, class Unary_pred, class F> [[deprecated("use for_each_element_if instead")]] constexpr auto
for_each_if(const hvector<Ts...>& hv, Unary_pred p, F f) -> void {
  auto f_cond = [p,f](auto&& x){ if (p(x)) f(x); };
  for_each_element(hv,f_cond);
}


} // std_e

