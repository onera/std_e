#pragma once


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

} // std_e
