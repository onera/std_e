#pragma once


namespace std_e {


// type_with_other_constness {
template<class T, class T_other>
struct type_with_other_constness__impl {
  using type = T;
};
template<class T, class T_other>
struct type_with_other_constness__impl<T,const T_other> {
  using type = const T;
};


template<class T, class T_other>
using type_with_other_constness = typename type_with_other_constness__impl<T,T_other>::type;
// type_with_other_constness }


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


} // std_e
