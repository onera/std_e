#pragma once

#include "std_e/operation/operation_functor.hpp"
#include <utility>

namespace std_e {

class wrong_n_arg : public msg_exception {
  using msg_exception::msg_exception;
};

template<class T>
class dyn_op_functor {
  private:
  // one arg
    template<operation_kind op_k> static auto
    apply_1arg(const T& x) -> T {
      if constexpr (is_unary(op_k)) {
        return operation_functor<op_k>(x);
      } else {
        throw wrong_n_arg("operation " + to_string(op_k) + " is not unary");
      }
    }

    using func_1arg = T(*)(const T&);

    template<size_t... Is> static constexpr auto
    gen_array_1arg_from_op_kinds(std::index_sequence<Is...>) -> std::array<func_1arg,sizeof...(Is)> {
      return {apply_1arg<static_cast<operation_kind>(Is)> ...};
    }

    static constexpr auto func_1arg_table = gen_array_1arg_from_op_kinds(std::make_index_sequence<enum_size<operation_kind>>{});

  // two args
    template<operation_kind op_k> static auto
    apply_2args(const T& x, const T& y) -> T {
      if constexpr (is_binary(op_k)) {
        return operation_functor<op_k>(x,y);
      } else {
        throw wrong_n_arg("operation " + to_string(op_k) + " is not binary");
      }
    }

    using func_2args = T(*)(const T&, const T&);

    template<size_t... Is> static constexpr auto
    gen_array_2args_from_op_kinds(std::index_sequence<Is...>) -> std::array<func_2args,sizeof...(Is)> {
      return {apply_2args<static_cast<operation_kind>(Is)> ...};
    }

    static constexpr auto func_2args_table = gen_array_2args_from_op_kinds(std::make_index_sequence<enum_size<operation_kind>>{});
  public:
    static auto
    apply(operation_kind op_k, const T& x) -> T {
      return func_1arg_table[static_cast<int>(op_k)](x);
    }
    static auto
    apply(operation_kind op_k, const T& x, const T& y) -> T {
      return func_2args_table[static_cast<int>(op_k)](x,y);
    }
};

template<class T> auto
apply_operation(operation_kind op, const T& x) -> T {
  return dyn_op_functor<T>::apply(op,x);
}
template<class T> auto
apply_operation(operation_kind op, const T& x, const T& y) -> T {
  return dyn_op_functor<T>::apply(op,x,y);
}
template<class Range> auto
apply_operation_to_args(operation_kind op, const Range& x) {
  if (x.size()==1) {
    return apply_operation(op,x[0]);
  }
  if (x.size()==2) {
    return apply_operation(op,x[0],x[1]);
  }
  throw not_implemented_exception("apply_operation_to_args: implemented only for 1 and 2 args");
}

} // std_e
