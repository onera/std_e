#pragma once


#include "std_e/base/var_arg_reduction.hpp"
#include "std_e/operation/operation_kind.hpp"
#include "std_e/future/is_detected.hpp"
#include <cmath>


namespace std_e {

template<operation_kind op_k> constexpr auto
f0 = [](auto&&...){ throw not_implemented_exception("operation_closure for operation " + to_string(op_k)); return any_return_type2{}; };

#define GENERATE_OPERATOR(op_name,symbol) \
  template<> constexpr auto f0<operation_kind::op_name> = [](auto&&... xs) -> decltype( (FWD(xs) symbol ...) ) { return (FWD(xs) symbol ...); };

#define GENERATE_FUNC_OPERATOR(op_name) \
  template<> constexpr auto f0<operation_kind::op_name> = [](auto&&... xs) -> decltype(op_name(FWD(xs)...)) { return op_name(FWD(xs)...); }; \

#define GENERATE_FUNC_STD_OPERATOR(op_name) \
  template<class... Ts> constexpr auto \
  op_name##_impl(Ts&&... xs) -> decltype(op_name(FWD(xs)...)) { \
    return op_name(FWD(xs)...); \
  } \
  template<class... Ts> constexpr auto \
  op_name##_impl(Ts&&... xs) -> decltype(std::op_name(FWD(xs)...)) { \
    return std::op_name(FWD(xs)...); \
  } \
 \
  template<> constexpr auto f0<operation_kind::op_name> = [](auto&&... xs) -> decltype(op_name##_impl(FWD(xs)...)) { return op_name##_impl(FWD(xs)...); };

template<class... Ts> constexpr auto
sqrt_impl(Ts&&... xs) -> decltype(sqrt(FWD(xs)...)) {
  return sqrt(FWD(xs)...);
}
template<class... Ts> constexpr auto
sqrt_impl(Ts&&... xs) -> decltype(std::sqrt(FWD(xs)...)) {
  return std::sqrt(FWD(xs)...);
}
template<> constexpr auto f0<operation_kind::sqrt> = [](auto&&... xs) -> decltype(sqrt_impl(FWD(xs)...)) { return sqrt_impl(FWD(xs)...); };

GENERATE_OPERATOR( plus       , + );
GENERATE_OPERATOR( minus      , - );
GENERATE_OPERATOR( multiplies , * );
GENERATE_OPERATOR( divides    , / );
GENERATE_OPERATOR( pipe       , | );

GENERATE_FUNC_STD_OPERATOR(abs);
//GENERATE_FUNC_STD_OPERATOR(sqrt);
GENERATE_FUNC_STD_OPERATOR(min);
GENERATE_FUNC_STD_OPERATOR(max);

GENERATE_FUNC_OPERATOR(identity);
GENERATE_FUNC_OPERATOR(assign);
GENERATE_FUNC_OPERATOR(gather);

GENERATE_FUNC_OPERATOR(tensor_prod);

GENERATE_FUNC_OPERATOR(t);
GENERATE_FUNC_OPERATOR(tr);

GENERATE_FUNC_OPERATOR(grad);



// Note: vocabulary
//   - Closure: class with operator()
//   - Functor: object of closure type
template<operation_kind op_k>
struct operation_closure {
  template <class... Ts>
  using op_t = decltype(f0<op_k>(std::declval<Ts>()...));
  template <class... Ts>
  static constexpr bool supports_op = is_detected_v<op_t, Ts...>;

  template<class... Ts> FORCE_INLINE constexpr auto
  operator()(Ts&&... xs) const {
    if constexpr (supports_op<Ts...>) {
      return f0<op_k>(FWD(xs)...);
    } else {
      throw not_implemented_exception("unsupported type for "+to_string(op_k));
      return any_return_type{};
    }
  }
};

using TTT = decltype(f0<operation_kind::sqrt>(1.));
static_assert(std::is_same_v<TTT,double>);
static_assert(operation_closure<operation_kind::sqrt>::template supports_op<double>);


template<operation_kind op_k>
constexpr auto basic_operation_functor = operation_closure<op_k>();

// only makes sense for binary operators
template<operation_kind op_k>
using operation_closure_with_extended_arity = reduction_functor<operation_closure<op_k>>;

template<operation_kind op_k>
constexpr auto operation_functor = operation_closure_with_extended_arity<op_k>();

template<operation_kind op_k> constexpr auto
operation(operation_closure_with_extended_arity<op_k>) -> operation_kind {
  return op_k;
}


} // std_e
