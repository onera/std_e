#pragma once


#include "std_e/base/var_arg_reduction.h"
#include "std_e/operation/operation_kind.h"
#include <cmath>


namespace std_e {


// Note: vocabulary
//   - Closure: class with operator()
//   - Functor: object of closure type
template<operation_kind op_k>
struct operation_closure;


/// operation_closure specializations {


/// function {
template<>
struct operation_closure<operation_kind::identity> {
  template<class T> FORCE_INLINE constexpr auto
  operator()(T&& x) const -> T {
    return std::forward<T>(x);
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
  template<class T0, class T1> FORCE_INLINE constexpr auto
  operator()(T0&& x, T1&& y) const {
    return std::forward<T0>(x) + std::forward<T1>(y);
  }
};
template<>
struct operation_closure<operation_kind::minus> {
  template<class T> FORCE_INLINE constexpr auto
  operator()(T&& x) const {
    return -std::forward<T>(x);
  }
  template<class T0, class T1> FORCE_INLINE constexpr auto
  operator()(T0&& x, T1&& y) const {
    return std::forward<T0>(x) - std::forward<T1>(y);
  }
};
template<>
struct operation_closure<operation_kind::multiplies> {
  template<class T0, class T1> FORCE_INLINE constexpr auto
  operator()(T0&& x, T1&& y) const {
    return std::forward<T0>(x) * std::forward<T1>(y);
  }
};
template<>
struct operation_closure<operation_kind::divides> {
  template<class T0, class T1> FORCE_INLINE constexpr auto
  operator()(T0&& x, T1&& y) const {
    return std::forward<T0>(x) / std::forward<T1>(y);
  }
};
/// arithmetics }


/// real {
template<>
struct operation_closure<operation_kind::sqrt> {
  template<class T> FORCE_INLINE constexpr auto
  operator()(T&& x) const {
    using std::sqrt;
    return sqrt( std::forward<T>(x) );
  }
};
template<>
struct operation_closure<operation_kind::abs> {
  template<class T> FORCE_INLINE constexpr auto
  operator()(T&& x) const {
    using std::abs;
    return abs( std::forward<T>(x) );
  }
};
template<>
struct operation_closure<operation_kind::min> {
  template<class T> FORCE_INLINE constexpr auto
  operator()(T&& x) const {
    return std::forward<T>(x);
  }
  template<class T0, class T1> FORCE_INLINE constexpr auto
  operator()(T0&& x, T1&& y) const {
    using std::min;
    return min( std::forward<T0>(x) , std::forward<T1>(y) );
  }
};
template<>
struct operation_closure<operation_kind::max> {
  template<class T> FORCE_INLINE constexpr auto
  operator()(T&& x) const {
    return std::forward<T>(x);
  }
  template<class T0, class T1> FORCE_INLINE constexpr auto
  operator()(T0&& x, T1&& y) const {
    using std::max;
    return max( std::forward<T0>(x) , std::forward<T1>(y) );
  }
};
/// real }

/// matrix/tensor {
template<>
struct operation_closure<operation_kind::pipe> {
  template<class T0, class T1> FORCE_INLINE constexpr auto
  operator()(T0&& x, T1&& y) const {
    return ( std::forward<T0>(x) | std::forward<T1>(y) );
  }
};
template<>
struct operation_closure<operation_kind::tensor_prod> {
  template<class T0, class T1> FORCE_INLINE constexpr auto
  operator()(T0&& x, T1&& y) const {
    return tensor_prod( std::forward<T0>(x) , std::forward<T1>(y) );
  }
};
template<>
struct operation_closure<operation_kind::t> {
  template<class T> FORCE_INLINE constexpr auto
  operator()(T&& x) const {
    return t( std::forward<T>(x) );
  }
};
template<>
struct operation_closure<operation_kind::tr> {
  template<class T> FORCE_INLINE constexpr auto
  operator()(T&& x) const {
    return tr( std::forward<T>(x) );
  }
};
/// matrix/tensor }


/// operation_closure specializations }


// only makes sense for binary operators
template<operation_kind op_k>
using operation_closure_with_extended_arity = reduction_functor<operation_closure<op_k>>;


template<operation_kind op_k>
constexpr auto operation_functor = operation_closure_with_extended_arity<op_k>();


} // std_e
