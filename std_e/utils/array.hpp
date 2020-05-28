#pragma once


#include "std_e/concept/array.hpp"
#include <tuple> // just for tuple_size<Array>... (which has, btw, nothing to do with std::tuple)
#include "std_e/future/contract.hpp"
#include "std_e/base/macros.hpp"
#include <algorithm>
#include "std_e/future/constexpr_vector.hpp"


// The idea of these functions is to provide a common interface
// between std::array and std::vector (and more generally, to any Array class)
//  - common creation from a size
//  - copy/fwd from one to the other


namespace std_e {


// same_array_type_except_size {
template<auto N_new, template<class,auto> class Fixed_size_array_template, class T, auto N> constexpr auto
make_same_array_type_except_size(Fixed_size_array_template<T,N>) {
  return Fixed_size_array_template<T,N_new>{};
}
template<class Fixed_size_array, auto N_new>
using same_array_type_except_size = decltype(make_same_array_type_except_size<N_new>(Fixed_size_array{}));
// same_array_type_except_size }


// make_array_of_size {
template<class Array, class Enable = void>
struct make_array_of_size__impl;

template<class T>
struct make_array_of_size__impl< T , std::enable_if_t<is_fixed_size_array<T>> > {
  static constexpr auto
  func(int sz) -> T {
    constexpr int array_sz = std::tuple_size_v<T>;
    STD_E_ASSERT(sz==array_sz);
    return {};
  }
};

template<class T>
struct make_array_of_size__impl< T , std::enable_if_t<is_dyn_size_array<T>> > {
  template<class Integer> static constexpr auto
  func(Integer sz) -> T {
    return T(sz);
  }
};

template<class Array, class Integer> constexpr auto
make_array_of_size(Integer sz) -> Array {
  return make_array_of_size__impl<Array>::func(sz);
}
// make_array_of_size }


// concatenated_array {
template<class Enable, class... Arrays>
struct concatenated_array__impl;


template<class Array, class... Arrays>
struct concatenated_array__impl<
  std::enable_if_t<is_fixed_size_array<Array>>,
  Array,Arrays... 
>
{
  static constexpr int sum_sizes = std::tuple_size_v<Array> + (std::tuple_size_v<Arrays> + ...);
  using type = same_array_type_except_size<Array,sum_sizes>;
};

template<class Array, class... Arrays>
struct concatenated_array__impl<
  std::enable_if_t<is_dyn_size_array<Array>>,
  Array,Arrays... 
>
{
  using type = Array;
};

template<class... Arrays> using
concatenated_array = typename concatenated_array__impl<void,Arrays...>::type;
// concatenated_array }




// convert_to {
/// From array "x" of type Array1, get an array "y" of type Array0
///   - if Array0==Array1, forward x
///   - if Array0!=Array1, copy x into y
template<
  class Array0, class Array1,
  std::enable_if_t< std::is_same<Array0,Array1>::value , int > =0
> constexpr auto
convert_to(Array1&& x) 
 -> Array1&&
{
  return FWD(x);
}
template<
  class Array0, class Array1,
  std::enable_if_t< not std::is_same<Array0,Array1>::value , int > =0
> constexpr auto
convert_to(Array1&& x) 
 -> Array0
{
  size_t n = x.size();
  Array0 y = make_array_of_size<Array0>(n);;
  std::copy_n(begin(x),n,begin(y));
  return y;
}
template<class Array, class T> constexpr auto
convert_to(std::initializer_list<T> x) -> Array {
  size_t n = x.size();
  Array y = make_array_of_size<Array>(n);;
  std::copy_n(begin(x),n,begin(y));
  return y;
}
// convert_to }


template<class Array, class T> constexpr auto
// requires Array::value_type == T
constant_array(const T& x) -> Array {
  Array constant_a = {};
  std_e::fill(begin(constant_a),end(constant_a),x);
  return constant_a;
}

template<class Array> constexpr Array default_array = constant_array<Array>(typename Array::value_type());

template<class Array, class T> constexpr auto
default_array_except(int i, const T& x) -> Array {
  auto res = default_array<Array>;
  res[i] = x;
  return res;
}


} // std_e
