#pragma once


#include "std_e/base/var_arg_reduction.hpp"
#include "std_e/operation/operation_kind.hpp"
#include "std_e/future/is_detected.hpp"
#include <cmath>


namespace std_e {


// Note: vocabulary
//   - Closure: class with operator()
//   - Functor: object of closure type
template<operation_kind op_k>
struct operation_closure {
  template<class T, class... Ts> FORCE_INLINE constexpr auto
  operator()(T&&, Ts&&...) const -> T {
    throw not_implemented_exception("operation_closure for operation " + to_string(op_k));
  }
};


/// operation_closure specializations {


/// function {
template<>
struct operation_closure<operation_kind::identity> {
  template<class T> FORCE_INLINE constexpr auto
  operator()(T&& x) const -> T {
    return std::forward<T>(x);
  }
};
template<>
struct operation_closure<operation_kind::assignment> {
  template <class T0, class T1>
  using op_t = decltype(assign(std::declval<T0>(),std::declval<T1>()));
  template <class T0, class T1>
  static constexpr bool supports_op = is_detected_v<op_t, T0,T1>;

  template<class T0, class T1> FORCE_INLINE constexpr auto
  operator()(T0&& x, T1&& y) const {
    if constexpr (supports_op<T0,T1>) {
      return assign(std::forward<T0>(x) , std::forward<T1>(y));
    } else {
      throw not_implemented_exception("unsupported type for assignment");
      return any_return_type{};
    }
  }
};
template<>
struct operation_closure<operation_kind::gathering> {
  template <class T0, class T1>
  using op_t = decltype(gather(std::declval<T0>(),std::declval<T1>()));
  template <class T0, class T1>
  static constexpr bool supports_op = is_detected_v<op_t, T0,T1>;

  template<class T0, class T1> FORCE_INLINE constexpr auto
  operator()(T0&& x, T1&& y) const {
    if constexpr (supports_op<T0,T1>) {
      return gather(std::forward<T0>(x) , std::forward<T1>(y));
    } else {
      throw not_implemented_exception("unsupported type for gathering");
      return any_return_type{};
    }
  }
};
/// function }


/// arithmetics {
template<>
struct operation_closure<operation_kind::plus> {
  template<class T> FORCE_INLINE constexpr auto
  operator()(T&& x) const {
    return std::forward<T>(x);
  }


  template <class T0, class T1>
  using op_t = decltype(std::declval<T0>()+std::declval<T1>());
  template <class T0, class T1>
  static constexpr bool supports_op = is_detected_v<op_t, T0,T1>;

  template<class T0, class T1> FORCE_INLINE constexpr auto
  operator()(T0&& x, T1&& y) const {
    if constexpr (supports_op<T0,T1>) {
      return std::forward<T0>(x) + std::forward<T1>(y);
    } else {
      throw not_implemented_exception("unsupported type for binary plus");
      return any_return_type{};
    }
  }
};
template<>
struct operation_closure<operation_kind::minus> {
  // unary
  template <class T>
  using unary_op_t = decltype(-std::declval<T>());
  template <class T>
  static constexpr bool supports_unary_op = is_detected_v<unary_op_t, T>;

  template<class T> FORCE_INLINE constexpr auto
  operator()(T&& x) const {
    if constexpr (supports_unary_op<T>) {
      return -std::forward<T>(x);
    } else {
      throw not_implemented_exception("unsupported type for unary minus");
      return any_return_type{};
    }
  }

  // binary
  template <class T0, class T1>
  using bin_op_t = decltype(std::declval<T0>()-std::declval<T1>());
  template <class T0, class T1>
  static constexpr bool supports_bin_op = is_detected_v<bin_op_t, T0,T1>;

  template<class T0, class T1> FORCE_INLINE constexpr auto
  operator()(T0&& x, T1&& y) const {
    if constexpr (supports_bin_op<T0,T1>) {
      return std::forward<T0>(x) - std::forward<T1>(y);
    } else {
      throw not_implemented_exception("unsupported type for binary minus");
      return any_return_type{};
    }
  }
};
template<>
struct operation_closure<operation_kind::multiplies> {
  template <class T0, class T1>
  using op_t = decltype(std::declval<T0>()*std::declval<T1>());
  template <class T0, class T1>
  static constexpr bool supports_op = is_detected_v<op_t, T0,T1>;

  template<class T0, class T1> FORCE_INLINE constexpr auto
  operator()(T0&& x, T1&& y) const {
    if constexpr (supports_op<T0,T1>) {
      return std::forward<T0>(x) * std::forward<T1>(y);
    } else {
      throw not_implemented_exception("unsupported type for multiplies");
      return any_return_type{};
    }
  }
};
template<>
struct operation_closure<operation_kind::divides> {
  template <class T0, class T1>
  using op_t = decltype(std::declval<T0>()/std::declval<T1>());
  template <class T0, class T1>
  static constexpr bool supports_op = is_detected_v<op_t, T0,T1>;

  template<class T0, class T1> FORCE_INLINE constexpr auto
  operator()(T0&& x, T1&& y) const {
    if constexpr (supports_op<T0,T1>) {
      return std::forward<T0>(x) / std::forward<T1>(y);
    } else {
      throw not_implemented_exception("unsupported type for divides");
      return any_return_type{};
    }
  }
};
/// arithmetics }


/// real {
template<>
struct operation_closure<operation_kind::sqrt> {
  template <class T>
  using op_t = decltype(sqrt(std::declval<T>()));
  template <class T>
  using op_std_t = decltype(std::sqrt(std::declval<T>()));
  template <class T>
  static constexpr bool supports_op = is_detected_v<op_t, T> || is_detected_v<op_std_t, T>;

  template<class T> FORCE_INLINE constexpr auto
  operator()(T&& x) const {
    if constexpr (supports_op<T>) {
      using std::sqrt;
      return sqrt( std::forward<T>(x) );
    } else {
      throw not_implemented_exception("unsupported type for sqrt");
      return any_return_type{};
    }
  }
};
template<>
struct operation_closure<operation_kind::abs> {
  template <class T>
  using op_t = decltype(abs(std::declval<T>()));
  template <class T>
  using op_std_t = decltype(std::abs(std::declval<T>()));
  template <class T>
  static constexpr bool supports_op = is_detected_v<op_t, T> || is_detected_v<op_std_t, T>;

  template<class T> FORCE_INLINE constexpr auto
  operator()(T&& x) const {
    if constexpr (supports_op<T>) {
      using std::abs;
      return abs( std::forward<T>(x) );
    } else {
      throw not_implemented_exception("unsupported type for abs");
      return any_return_type{};
    }
  }
};
template<>
struct operation_closure<operation_kind::min> {
  template<class T> FORCE_INLINE constexpr auto
  operator()(T&& x) const {
    return std::forward<T>(x);
  }

  template <class T0, class T1>
  using op_t = decltype(min(std::declval<T0>(),std::declval<T1>()));
  template <class T0, class T1>
  using op_std_t = decltype(std::min(std::declval<T0>(),std::declval<T1>()));
  template <class T0, class T1>
  static constexpr bool supports_op = is_detected_v<op_t, T0,T1> || is_detected_v<op_std_t, T0,T1>;

  template<class T0, class T1> FORCE_INLINE constexpr auto
  operator()(T0&& x, T1&& y) const {
    if constexpr (supports_op<T0,T1>) {
      using std::min;
      return min( std::forward<T0>(x) , std::forward<T1>(y) );
    } else {
      throw not_implemented_exception("unsupported type for min");
      return any_return_type{};
    }
  }
};
template<>
struct operation_closure<operation_kind::max> {
  template<class T> FORCE_INLINE constexpr auto
  operator()(T&& x) const {
    return std::forward<T>(x);
  }

  template <class T0, class T1>
  using op_t = decltype(max(std::declval<T0>(),std::declval<T1>()));
  template <class T0, class T1>
  using op_std_t = decltype(std::max(std::declval<T0>(),std::declval<T1>()));
  template <class T0, class T1>
  static constexpr bool supports_op = is_detected_v<op_t, T0,T1> || is_detected_v<op_std_t, T0,T1>;

  template<class T0, class T1> FORCE_INLINE constexpr auto
  operator()(T0&& x, T1&& y) const {
    if constexpr (supports_op<T0,T1>) {
      using std::max;
      return max( std::forward<T0>(x) , std::forward<T1>(y) );
    } else {
      throw not_implemented_exception("unsupported type for max");
      return any_return_type{};
    }
  }
};
/// real }

/// matrix/tensor {
template<>
struct operation_closure<operation_kind::pipe> {
  template <class T0, class T1>
  using op_t = decltype(std::declval<T0>()|std::declval<T1>());
  template <class T0, class T1>
  static constexpr bool supports_op = is_detected_v<op_t, T0,T1>;

  template<class T0, class T1> FORCE_INLINE constexpr auto
  operator()(T0&& x, T1&& y) const {
    if constexpr (supports_op<T0,T1>) {
      return ( std::forward<T0>(x) | std::forward<T1>(y) );
    } else {
      throw not_implemented_exception("unsupported type for pipe");
      return any_return_type{};
    }
  }
};
template<>
struct operation_closure<operation_kind::tensor_prod> {
  template <class T0, class T1>
  using op_t = decltype(tensor_prod(std::declval<T0>(),std::declval<T1>()));
  template <class T0, class T1>
  static constexpr bool supports_op = is_detected_v<op_t, T0,T1>;

  template<class T0, class T1> FORCE_INLINE constexpr auto
  operator()(T0&& x, T1&& y) const {
    if constexpr (supports_op<T0,T1>) {
      return tensor_prod( std::forward<T0>(x) , std::forward<T1>(y) );
    } else {
      throw not_implemented_exception("unsupported type for tensor_prod");
      return any_return_type{};
    }
  }
};
template<>
struct operation_closure<operation_kind::t> {
  template <class T>
  using op_t = decltype(t(std::declval<T>()));
  template <class T>
  static constexpr bool supports_op = is_detected_v<op_t, T>;

  template<class T> FORCE_INLINE constexpr auto
  operator()(T&& x) const {
    if constexpr (supports_op<T>) {
      return t( std::forward<T>(x) );
    } else {
      throw not_implemented_exception("unsupported type for transpose");
      return any_return_type{};
    }
  }
};
template<>
struct operation_closure<operation_kind::tr> {
  template <class T>
  using op_t = decltype(tr(std::declval<T>()));
  template <class T>
  static constexpr bool supports_op = is_detected_v<op_t, T>;

  template<class T> FORCE_INLINE constexpr auto
  operator()(T&& x) const {
    if constexpr (supports_op<T>) {
      return tr( std::forward<T>(x) );
    } else {
      throw not_implemented_exception("unsupported type for trace");
      return any_return_type{};
    }
  }
};
/// matrix/tensor }


/// operation_closure specializations }


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
