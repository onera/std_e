#pragma once


#include "std_e/utils/enum.hpp"
#include "std_e/base/not_implemented_exception.hpp"


namespace std_e {


STD_E_ENUM_CLASS(operation_kind,
// operators, SEE https://en.cppreference.com/w/cpp/utility/functional
  // arithmetic
  plus,
  minus,
  multiplies,
  divides,
  modulus,
  negates,
  // comparison
  equal_to,
  not_equal_to,
  less,
  less_equal,
  greater,
  greater_equal,
  // logical
  logical_and,
  logical_or,
  logical_not,
  // bitwise
  bit_and,
  bit_or,
  bit_xor,
  bit_not,

// std functions, SEE https://en.cppreference.com/w/cpp/numeric/math
  // basic
  abs,
  min,
  max,
  // exponential
  exp,
  log,
  // power
  pow,
  sqrt,
  cbrt,
  hypot,
  // trigo
  sin,
  cos,
  tan,
  asin,
  acos,
  atan,
  atan2,
  // hyperbolic
  sinh,
  cosh,
  tanh,
  asinh,
  acosh,
  atanh,
  // error and gamma
  erf,
  erfc,
  tgamma, // gamma
  lgamma, // log(gamma)


// base function
  identity,
  assign,
  gather,

// matrix/tensor
  tensor_prod,
  t, // transposed
  tr, // trace

// analysis
  grad,
  div,
  rot,
);

constexpr auto
is_operator(operation_kind op) -> bool {
  switch (op) {
    // arithmetic
    case operation_kind::plus:
    case operation_kind::minus:
    case operation_kind::multiplies:
    case operation_kind::divides:
    case operation_kind::modulus:
    case operation_kind::negates:
    // comparison
    case operation_kind::equal_to:
    case operation_kind::not_equal_to:
    case operation_kind::less:
    case operation_kind::less_equal:
    case operation_kind::greater:
    case operation_kind::greater_equal:
    // logical
    case operation_kind::logical_and:
    case operation_kind::logical_or:
    case operation_kind::logical_not:
    // bitwise
    case operation_kind::bit_and:
    case operation_kind::bit_or:
    case operation_kind::bit_xor:
    case operation_kind::bit_not:
      return true;
    default:
      return false;
  }
}

constexpr auto
is_std_function(operation_kind op) -> bool {
  switch (op) {
    // basic
    case operation_kind::abs:
    case operation_kind::min:
    case operation_kind::max:
    // exponential
    case operation_kind::exp:
    case operation_kind::log:
    // power
    case operation_kind::pow:
    case operation_kind::sqrt:
    case operation_kind::cbrt:
    case operation_kind::hypot:
    // trigo
    case operation_kind::sin:
    case operation_kind::cos:
    case operation_kind::tan:
    case operation_kind::asin:
    case operation_kind::acos:
    case operation_kind::atan:
    case operation_kind::atan2:
    // hyperbolic
    case operation_kind::sinh:
    case operation_kind::cosh:
    case operation_kind::tanh:
    case operation_kind::asinh:
    case operation_kind::acosh:
    case operation_kind::atanh:
    // error and gamma
    case operation_kind::erf:
    case operation_kind::erfc:
    case operation_kind::tgamma: // gamma
    case operation_kind::lgamma: // log(gamma)
      return true;
    default:
      return false;
  }
}


inline auto
to_symbol_string(operation_kind x) -> std::string {
  switch (x) {
    case operation_kind::plus: return "+";
    case operation_kind::minus: return "-";
    case operation_kind::multiplies: return "*";
    case operation_kind::divides: return "/";
    case operation_kind::modulus: return "%";
    case operation_kind::negates: return "-";
    // comparison
    case operation_kind::equal_to: return "==";
    case operation_kind::not_equal_to: return "!=";
    case operation_kind::less: return "<";
    case operation_kind::less_equal: return "<=";
    case operation_kind::greater: return ">";
    case operation_kind::greater_equal: return ">=";
    // logical
    case operation_kind::logical_and: return "&&";
    case operation_kind::logical_or: return "||";
    case operation_kind::logical_not: return "!";
    // bitwise
    case operation_kind::bit_and: return "&";
    case operation_kind::bit_or: return "|";
    case operation_kind::bit_xor: return "^";
    case operation_kind::bit_not: return "~";

    case operation_kind::tensor_prod: return "⊗";
    default:
      return to_string(x); // just return the name of the enum
  }
}


} // std_e
