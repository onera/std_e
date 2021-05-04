#pragma once


#include "std_e/base/var_arg_reduction.hpp"
#include "std_e/operation/operation_kind.hpp"
#include "std_e/future/is_detected.hpp"
#include "std_e/base/lift.hpp"
#include <cmath>


namespace std_e {

// operation_kind / associated function name {

/// Associate each operation_kind to a function name
//
/// The association of operation_kind "op_k" to it name
/// is done through the templated lambdas "operation_overload_set<op_k>" and "operation_std_overload_set<op_k>"
//
/// What is complicated here is that we want
///   - to be SFINAE friendly, that is, when op_k is not implemented for a type T,
///     then "operation_std_overload_set<op_k>" will result in substition failure
///     (i.e. the fact that op_k is not implemented can be **detected**
///   - to work for
///       - operators (+,-...)
///       - overload sets (using argument-dependent lookup)
///       - std:: functions (so that it works for build-in types)


/// Default implementations {
template<operation_kind op_k> constexpr auto
operation_overload_set = [](auto&& x, auto&&... xs) -> never_enable<decltype(x)> {};

template<operation_kind op_k> constexpr auto
operation_std_overload_set = operation_overload_set<op_k>;
/// Default implementations }


/// Boilerplate macros {
// The SFINAE friendliness is done through RETURNS
#define GENERATE_UNARY_OPERATOR_OVERLOAD_SET(op_name,symbol) \
  template<> constexpr auto operation_overload_set<operation_kind::op_name> = [](auto&& x) RETURNS( symbol FWD(x) );

#define GENERATE_BINARY_OPERATOR_OVERLOAD_SET(op_name,symbol) \
  template<> constexpr auto operation_overload_set<operation_kind::op_name> = [](auto&&... xs) RETURNS( (FWD(xs) symbol ...) );

#define GENERATE_FUNCTION_OVERLOAD_SET(op_name) \
  template<> constexpr auto operation_overload_set<operation_kind::op_name> = [](auto&&... xs) RETURNS( op_name(FWD(xs)...) );

#define GENERATE_FUNCTION_STD_OVERLOAD_SET(op_name) \
  GENERATE_FUNCTION_OVERLOAD_SET(op_name) \
  template<> constexpr auto operation_std_overload_set<operation_kind::op_name> = [](auto&&... xs) RETURNS( ::std::op_name(FWD(xs)...) );

// We need to reimplement std::min and std::max with SFINAE-friendlyness
// (they are special because they are templated, unlike e.g. std::sqrt, ...
template<class T> constexpr auto
min(const T& a, const T& b) -> std::enable_if_t<std::is_same_v<decltype(b<a),bool>,const T&> {
  return (b < a) ? b : a;
}
template<class T> constexpr auto
max(const T& a, const T& b) -> std::enable_if_t<std::is_same_v<decltype(a<b),bool>,const T&> {
  return (a < b) ? b : a;
}
#define GENERATE_FUNCTION_STD_E_OVERLOAD_SET(op_name) \
  GENERATE_FUNCTION_OVERLOAD_SET(op_name) \
  template<> constexpr auto operation_std_overload_set<operation_kind::op_name> = [](auto&&... xs) RETURNS( ::std_e::op_name(FWD(xs)...) );
/// Boilerplate macros }


/// The actual associations {
//// operators {
///// arithmetic
GENERATE_BINARY_OPERATOR_OVERLOAD_SET( plus          , +  );
GENERATE_BINARY_OPERATOR_OVERLOAD_SET( minus         , -  );
GENERATE_BINARY_OPERATOR_OVERLOAD_SET( multiplies    , *  );
GENERATE_BINARY_OPERATOR_OVERLOAD_SET( divides       , /  );
GENERATE_BINARY_OPERATOR_OVERLOAD_SET( modulus       , %  );
GENERATE_BINARY_OPERATOR_OVERLOAD_SET( negates       , -  );
///// comparison
GENERATE_BINARY_OPERATOR_OVERLOAD_SET( equal_to      , == );
GENERATE_BINARY_OPERATOR_OVERLOAD_SET( not_equal_to  , != );
GENERATE_BINARY_OPERATOR_OVERLOAD_SET( less          , <  );
GENERATE_BINARY_OPERATOR_OVERLOAD_SET( less_equal    , <= );
GENERATE_BINARY_OPERATOR_OVERLOAD_SET( greater       , >  );
GENERATE_BINARY_OPERATOR_OVERLOAD_SET( greater_equal , >= );
///// logical
GENERATE_BINARY_OPERATOR_OVERLOAD_SET( logical_and   , && );
GENERATE_BINARY_OPERATOR_OVERLOAD_SET( logical_or    , || );
GENERATE_UNARY_OPERATOR_OVERLOAD_SET ( logical_not   , !  );
///// bitwise
GENERATE_BINARY_OPERATOR_OVERLOAD_SET( bit_and       , &  );
GENERATE_BINARY_OPERATOR_OVERLOAD_SET( bit_or        , |  );
GENERATE_BINARY_OPERATOR_OVERLOAD_SET( bit_xor       , ^  );
GENERATE_UNARY_OPERATOR_OVERLOAD_SET ( bit_not       , ~  );
//// operators }

//// std functions {
///// basic
GENERATE_FUNCTION_STD_OVERLOAD_SET  ( abs  );
GENERATE_FUNCTION_STD_E_OVERLOAD_SET( min  );
GENERATE_FUNCTION_STD_E_OVERLOAD_SET( max  );
///// exponential
GENERATE_FUNCTION_STD_OVERLOAD_SET( exp    );
GENERATE_FUNCTION_STD_OVERLOAD_SET( log    );
///// power
GENERATE_FUNCTION_STD_OVERLOAD_SET( pow    );
GENERATE_FUNCTION_STD_OVERLOAD_SET( sqrt   );
GENERATE_FUNCTION_STD_OVERLOAD_SET( cbrt   );
GENERATE_FUNCTION_STD_OVERLOAD_SET( hypot  );
///// trigo
GENERATE_FUNCTION_STD_OVERLOAD_SET( sin    );
GENERATE_FUNCTION_STD_OVERLOAD_SET( cos    );
GENERATE_FUNCTION_STD_OVERLOAD_SET( tan    );
GENERATE_FUNCTION_STD_OVERLOAD_SET( asin   );
GENERATE_FUNCTION_STD_OVERLOAD_SET( acos   );
GENERATE_FUNCTION_STD_OVERLOAD_SET( atan   );
GENERATE_FUNCTION_STD_OVERLOAD_SET( atan2  );
///// hyperbolic
GENERATE_FUNCTION_STD_OVERLOAD_SET( sinh   );
GENERATE_FUNCTION_STD_OVERLOAD_SET( cosh   );
GENERATE_FUNCTION_STD_OVERLOAD_SET( tanh   );
GENERATE_FUNCTION_STD_OVERLOAD_SET( asinh  );
GENERATE_FUNCTION_STD_OVERLOAD_SET( acosh  );
GENERATE_FUNCTION_STD_OVERLOAD_SET( atanh  );
///// error and gamma
GENERATE_FUNCTION_STD_OVERLOAD_SET( erf    );
GENERATE_FUNCTION_STD_OVERLOAD_SET( erfc   );
GENERATE_FUNCTION_STD_OVERLOAD_SET( tgamma );
GENERATE_FUNCTION_STD_OVERLOAD_SET( lgamma );
//// std functions }

//// base function
GENERATE_FUNCTION_OVERLOAD_SET( identity );
GENERATE_FUNCTION_OVERLOAD_SET( assign   );
GENERATE_FUNCTION_OVERLOAD_SET( gather   );

//// matrix/tensor
GENERATE_FUNCTION_OVERLOAD_SET( tensor_prod );
GENERATE_FUNCTION_OVERLOAD_SET( t );
GENERATE_FUNCTION_OVERLOAD_SET( tr );

//// analysis
GENERATE_FUNCTION_OVERLOAD_SET( grad );
GENERATE_FUNCTION_OVERLOAD_SET( div  );
GENERATE_FUNCTION_OVERLOAD_SET( rot  );
/// The actual associations }

// operation_kind / associated function name }




// Provide a default if the function has not been implemented {
// Note: vocabulary
//   - Closure: type with operator()
//   - Functor: object of closure type
template<operation_kind op_k>
struct operation_closure {
  template <class... Ts>
  using op_t = decltype(operation_overload_set<op_k>(std::declval<Ts>()...));
  template <class... Ts>
  static constexpr bool supports_op = is_detected_v<op_t, Ts...>;

  template <class... Ts>
  using std_op_t = decltype(operation_std_overload_set<op_k>(std::declval<Ts>()...));
  template <class... Ts>
  static constexpr bool supports_std_op = is_detected_v<std_op_t, Ts...>;

  template<class... Ts> FORCE_INLINE constexpr auto
  operator()(Ts&&... xs) const {
    if constexpr (supports_op<Ts...>) {
      return operation_overload_set<op_k>(FWD(xs)...);
    } else if constexpr (supports_std_op<Ts...>) {
      return operation_std_overload_set<op_k>(FWD(xs)...);
    } else {
      throw not_implemented_exception("in function " + std::string(__PRETTY_FUNCTION__)+": unsupported type for "+to_string(op_k));
      return any_return_type{};
    }
  }
};
// Provide a default if the function has not been implented }


template<operation_kind op_k>
constexpr auto operation_functor = operation_closure<op_k>();


template<operation_kind op_k> constexpr auto
operation(operation_closure<op_k>) -> operation_kind {
  return op_k;
}


} // std_e
