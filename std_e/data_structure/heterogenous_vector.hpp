#pragma once

#include <algorithm>
#include "std_e/utils/vector.hpp"
#include "std_e/utils/tuple.hpp"

namespace std_e {

/**
 *  \brief  hvector is a specific structure to store vector of multiple type
 */
template<class... Ts>
class hvector {
  public:
    //! \brief TODO
    using impl_type = std::tuple<std::vector<Ts>...>;

    //! Default constructor
    constexpr
    hvector() = default;

    //! Construtor from variadic template
    template <class... Ts0> constexpr
    // requires Ts0 == Ts
    hvector(std::vector<Ts0>... xs)
      : _impl({std::move(xs)...}){}

    //! \brief low-level access
    constexpr auto
    impl() -> impl_type& {
      return _impl;
    }

    //! \brief low-level access
    constexpr auto
    impl() const -> const impl_type& {
      return _impl;
    }

    //! Retun the total size : number of type in hvector
    static constexpr auto
    hsize() -> size_t {
      return sizeof...(Ts);
    }

    //! Return The current size of hvector : sum(vector.size())
    constexpr auto
    size() const -> size_t {
      size_t sz = 0;
      auto accumulate_size = [&sz](const auto& v) { sz += v.size(); };
      for_each(_impl,accumulate_size);
      return sz;
    }

    //! vector-like interface : requires T is one of the Ts...
    template<class T> auto
    push_back(const T& elt) {
      std::get<std::vector<T>>(_impl).push_back(elt);
    }
  private:
    impl_type _impl;    /*!< The current implementation  */
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


/*!
 *  \brief Get vector in current hvector from type in tuple
 */
template<class T, class... Ts> constexpr auto
get(hvector<Ts...>& x) -> std::vector<T>& {
  return std::get<std::vector<T>>(x.impl());
}

/*!
 *  \brief Same as const
 */
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

// for each element {
/*!
 *  \brief  Apply function for each element in the vector and for all type contains by hvector
 *
 *  Example :
 *  \code
 *     std_e::hvector<int,double> hv = { vector{1,2,3,4} , vector{3.14,2.7} };
 *     double total = 0;
 *     auto f = [&sz](auto& val){ total += val; };
 *     for_each_element(hv,f);
 *     assert(val == 1+2+3+4+3.14+2.7);
 *  \endcode
 */
template<class... Ts, class F> constexpr auto
for_each_element(hvector<Ts...>& hv, F f) -> void {
  auto f_for_each = [f](auto&& v){ for_each(v,f); };
  for_each_vector(hv,f_for_each);
}

/*!
 *  \brief Same but const
 */
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


template<class hvector_type, class Unary_pred, class F> constexpr auto
find_apply__impl(hvector_type&& hv, Unary_pred p, F f) -> std::pair<int,int> {
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
template<class hvector_type, class Unary_pred, class F> constexpr auto
find_apply__impl2(hvector_type&& hv, Unary_pred p, F f) -> std::pair<int,int> {
  int pos_in_vec = 0;
  auto p_tuple = [&p,&pos_in_vec](auto&& vec){
    auto it = std::find_if(begin(vec),end(vec),p);
    pos_in_vec = it-begin(vec);
    if (it!=end(vec)) {
      return true;
    } else {
      return false;
    }
  };
  auto f_tuple = [&f,&pos_in_vec](auto&& vec){
    f(vec[pos_in_vec]);
  };

  int pos_in_tuple = find_apply(hv.impl(),p_tuple,f_tuple);
  return std::make_pair(pos_in_tuple,pos_in_vec);
}


/*!
 *  \brief  TODO
 */
template<class... Ts, class Unary_pred, class F> constexpr auto
find_apply(hvector<Ts...>& hv, Unary_pred p, F f) -> std::pair<int,int> {
  return find_apply__impl(hv,p,f);
}

/*!
 *  \brief  TODO
 */
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

