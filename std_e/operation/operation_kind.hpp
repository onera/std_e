#pragma once


#include <string>


namespace std_e {


enum class operation_kind {
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
};


  

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
  }
  return "";
}
/// } to_string


} // std_e
