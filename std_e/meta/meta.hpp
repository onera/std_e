#pragma once


#include <functional>


namespace std_e {


template<class T>
struct is_reference_wrapper__impl {
  static constexpr bool value = false;
};

template<class T>
struct is_reference_wrapper__impl<std::reference_wrapper<T>> {
  static constexpr bool value = true;
};

template<class T>
constexpr bool is_reference_wrapper = is_reference_wrapper__impl<std::decay_t<T>>::value;


// add_other_type_constness {
template<class T, class T_other>
struct add_other_type_constness__impl {
  using type = T;
};
template<class T, class T_other>
struct add_other_type_constness__impl<T,const T_other> {
  using type = const T;
};


template<class T, class T_other>
using add_other_type_constness = typename add_other_type_constness__impl<T,T_other>::type;
// add_other_type_constness }


// remove_rvalue_reference {
template<class T>
struct remove_rvalue_reference__impl {
  using type = T;
};
template<class T>
struct remove_rvalue_reference__impl<T&&> {
  using type = T;
};
template<class T>
struct remove_rvalue_reference__impl<const T&&> {
  using type = const T;
};

template<class T>
using remove_rvalue_reference = typename remove_rvalue_reference__impl<T>::type;
// remove_rvalue_reference }


// from https://quuxplusone.github.io/blog/2019/02/06/arrow-proxy/
template<class Reference>
struct arrow_proxy {
  Reference r;
  Reference *operator->() {
    return &r;
  }
};


namespace detail {
  template <class F, class Tuple, std::size_t... Is> constexpr auto
  apply_move_impl(F&& f, Tuple&& t, std::index_sequence<Is...>) -> decltype(auto) {
    return std::invoke(std::forward<F>(f), std::move(std::get<Is>(t))...);
  }
}  // namespace detail

template <class F, class Tuple> constexpr auto
apply_move(F&& f, Tuple&& t) -> decltype(auto) {
  constexpr auto N = std::tuple_size_v<std::remove_reference_t<Tuple>>;
  return detail::apply_move_impl(std::forward<F>(f), std::forward<Tuple>(t), std::make_index_sequence<N>{});
}


} // std_e
