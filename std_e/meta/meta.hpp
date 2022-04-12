#pragma once


#include <functional>
#include "std_e/future/type_traits.hpp"
#include "std_e/meta/types.hpp"
#include "std_e/base/macros.hpp"


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


// replace_by_pointer_if_ref {
namespace detail {
  template<class T>
  struct replace_by_pointer_if_ref_impl {
    using type = T;
  };
  template<class T>
  struct replace_by_pointer_if_ref_impl<T&> {
    using type = T*;
  };
}
template<class T>
using replace_by_pointer_if_ref = typename detail::replace_by_pointer_if_ref_impl<T>::type;
// replace_by_pointer_if_ref }


// from https://quuxplusone.github.io/blog/2019/02/06/arrow-proxy/
template<class Reference>
struct arrow_proxy {
  Reference r;
  Reference *operator->() {
    return &r;
  }
};


// element_type {

template<class C>
struct element_type__impl {
  using type = typename C::value_type;
};

template<class T, ptrdiff_t N> class span_base; // Fwd decl
template<class T, ptrdiff_t N> class span; // Fwd decl
template<class T, ptrdiff_t N> class span_ref; // Fwd decl
template<class T, ptrdiff_t N>
struct element_type__impl<span_base<T,N>> {
  using type = T;
};
template<class T, ptrdiff_t N>
struct element_type__impl<span<T,N>> {
  using type = T;
};
template<class T, ptrdiff_t N>
struct element_type__impl<span_ref<T,N>> {
  using type = T;
};

template<class C>
using element_type = typename element_type__impl<std_e::remove_cvref_t<C>>::type;
// element_type }


namespace detail {
  template<class F, class Tuple, size_t... Is> constexpr auto
  apply_move_impl(F&& f, Tuple&& t, std::index_sequence<Is...>) -> decltype(auto) {
    return std::invoke(FWD(f), std::move(std::get<Is>(t))...);
  }
}

template<class F, class Tuple> constexpr auto
apply_move(F&& f, Tuple&& t) -> decltype(auto) {
  constexpr auto N = std::tuple_size_v<std::remove_reference_t<Tuple>>;
  return detail::apply_move_impl(FWD(f), FWD(t), std::make_index_sequence<N>{});
}


namespace detail {
  template<class F, class Tuple, size_t... Is> constexpr auto
  apply_forward_impl(F&& f, Tuple&& t, std::index_sequence<Is...>) -> decltype(auto) {
    //return std::invoke(std::forward<F>(f), std::forward<std::tuple_element_t<Is,std::remove_reference_t<Tuple>>>(std::get<Is>(t))...);
    return f(std::forward<std::tuple_element_t<Is,std::remove_reference_t<Tuple>>>(std::get<Is>(t))...);
  }
}

// apply f with t elements as arguments
// elements are moved, unless they are lvalue references
template<class F, class Tuple> constexpr auto
apply_forward(F&& f, Tuple&& t) -> decltype(auto) {
  constexpr auto N = std::tuple_size_v<std::remove_reference_t<Tuple>>;
  return detail::apply_forward_impl(FWD(f), FWD(t), std::make_index_sequence<N>{});
}


namespace detail {
  template<class F, class Tuple, size_t... Is, class Args_types> constexpr auto
  apply_forward_as_impl(F&& f, Tuple&& t, std::index_sequence<Is...>, Args_types) -> decltype(auto) {
    return std::invoke(FWD(f), std::forward<types_element_t<Is,Args_types>>(std::get<Is>(t))...);
  }
}

// apply f with t elements as arguments
// elements are moved, unless their corresponding type in "Args..." is an lvalue reference
// TODO test
template<class... Args, class F, class Tuple> constexpr auto
apply_forward_as(F&& f, Tuple&& t) -> decltype(auto) {
  static_assert(sizeof...(Args)==std::tuple_size_v<std::decay_t<Tuple>>);
  constexpr auto N = std::tuple_size_v<std::remove_reference_t<Tuple>>;
  return detail::apply_forward_as_impl(FWD(f), FWD(t), std::make_index_sequence<N>{}, std_e::types<Args...>{});
}


template <class... Ts> constexpr auto
make_non_temp_tuple(Ts&&... xs) -> std::tuple<remove_rvalue_reference<Ts>...> {
    return {FWD(xs)...};
}


} // std_e
