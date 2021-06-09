#include "std_e/operation/operation_kind.hpp"


namespace std_e {


auto
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
