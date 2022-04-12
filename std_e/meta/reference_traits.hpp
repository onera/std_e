#pragma once


#include "std_e/future/type_traits.hpp"


namespace std_e {


// Customization point:
//   by default, we remove the const& qualifiers
//   but the user is allowed to specialize `reference_value` for his own proxy reference type
template<class T>
struct reference_value {
  using type = std_e::remove_cvref_t<T>;
};


template<class T> using reference_value_t = typename reference_value<T>::type;


} // std_e
