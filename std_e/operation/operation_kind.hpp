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
needs_computation(operation_kind op) -> bool {
  // the result of these operations is immediate
  if (op==operation_kind::identity || op==operation_kind::t || op==operation_kind::gathering) {
    return false;
  }
  // the result of those need some computation
  if (op==operation_kind::plus       || op==operation_kind::minus
   || op==operation_kind::multiplies || op==operation_kind::divides
   || op==operation_kind::abs        || op==operation_kind::sqrt
   || op==operation_kind::min        || op==operation_kind::max
   || op==operation_kind::pipe       || op==operation_kind::tensor_prod || op==operation_kind::tr
   || op==operation_kind::assignment) {
    return true;
  } else {
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
