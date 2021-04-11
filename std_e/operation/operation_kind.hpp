#pragma once


#include "std_e/utils/enum.hpp"
#include "std_e/base/not_implemented_exception.hpp"


namespace std_e {


STD_E_ENUM_CLASS(operation_kind,
// function
  identity,
  assignment,
  gathering,

// arithmetics
  plus,
  minus,
  multiplies,
  divides,

// real
  abs,
  sqrt,
  min,
  max,

// matrix/tensor
  pipe,
  tensor_prod,
  t,
  tr,

// analysis
  grad
);

constexpr auto
is_unary(operation_kind op) -> bool {
  switch (op) {
    case operation_kind::identity:

    case operation_kind::plus:
    case operation_kind::minus:

    case operation_kind::abs:
    case operation_kind::sqrt:
    case operation_kind::min:
    case operation_kind::max:

    case operation_kind::t:
    case operation_kind::tr:

    case operation_kind::grad:
      return true;
    default:
      return false;
  }
}
constexpr auto
is_binary(operation_kind op) -> bool {
  switch (op) {
    case operation_kind::assignment:

    case operation_kind::plus:
    case operation_kind::minus:
    case operation_kind::multiplies:
    case operation_kind::divides:

    case operation_kind::min:
    case operation_kind::max:

    case operation_kind::pipe:
    case operation_kind::tensor_prod:
      return true;
    default:
      return false;
  }
}

constexpr auto
needs_computation(operation_kind op) -> bool {
  switch (op) {
    // the result of these operations is immediate
    case operation_kind::identity:
    case operation_kind::gathering:
    case operation_kind::t:
      return false;

    // the result of those need some computation
    case operation_kind::assignment: // assignment means copying memory, so it is not immediate

    case operation_kind::plus:
    case operation_kind::minus:
    case operation_kind::multiplies:
    case operation_kind::divides:

    case operation_kind::abs:
    case operation_kind::sqrt:
    case operation_kind::min:
    case operation_kind::max:

    case operation_kind::pipe:
    case operation_kind::tensor_prod:
    case operation_kind::tr:
      return true;

    case operation_kind::grad:
    default:
      throw not_implemented_exception{"needs_computation: unknown operation "+to_string(op)};
  }
}


// to_string {
inline auto
to_symbol_string(operation_kind x) -> std::string {
  switch (x) {
    case operation_kind::identity: return "id";
    case operation_kind::assignment: return "=";
    case operation_kind::gathering: return "gathering";

    case operation_kind::plus: return "+";
    case operation_kind::minus: return "-";
    case operation_kind::multiplies: return "*";
    case operation_kind::divides: return "/";

    case operation_kind::abs: return "abs";
    case operation_kind::sqrt: return "sqrt";
    case operation_kind::min: return "min";
    case operation_kind::max: return "max";

    case operation_kind::pipe: return "|";
    case operation_kind::tensor_prod: return "⊗";
    case operation_kind::t: return "t";
    case operation_kind::tr: return "tr";

    case operation_kind::grad: return "grad";
  }
  return "";
}
// to_string }


} // std_e
