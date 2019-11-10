#pragma once


#include "std_e/base/macros.h"
#include <utility>


namespace std_e {


template<class unary_op_type, class T> FORCE_INLINE constexpr auto
var_arg_reduction(unary_op_type unary_op, T&& x) {
  return unary_op( std::forward<T>(x) );
}
template<class bin_op_type, class T0, class T1> FORCE_INLINE constexpr auto
var_arg_reduction(bin_op_type bin_op, T0&& x0, T1&& x1) {
  return bin_op( std::forward<T0>(x0) , std::forward<T1>(x1) );
}
template<class bin_op_type, class T0, class T1, class... Ts> FORCE_INLINE constexpr auto
var_arg_reduction(bin_op_type bin_op, T0&& x0, T1&& x1, Ts&&... xs) {
  // note: left-associative
  return var_arg_reduction( bin_op, bin_op(std::forward<T0>(x0),std::forward<T1>(x1)) , std::forward<Ts>(xs)... );
}


template<class op_type>
struct reduction_functor {
  template<class... Ts> FORCE_INLINE constexpr auto
  operator()(Ts&&... xs) const {
    constexpr op_type op = {};
    return var_arg_reduction(op,std::forward<Ts>(xs)...);
  }
};


} // std_e
