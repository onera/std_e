#pragma once

#include "std_e/multi_array/multi_array/multi_array.hpp"
#include <array>
#include <vector>
#include "std_e/future/span.hpp"
#include "std_e/multi_array/shape/fixed_shape.hpp"
#include "std_e/multi_array/shape/dyn_shape.hpp"
#include "std_e/base/dynamic_size.hpp"
#include "std_e/multi_index/cartesian_product_size.hpp"


namespace std_e {


// fixed_multi_array {
template<int... dims> constexpr size_t cartesian_prod = cartesian_product_size(std_e::multi_index<int,sizeof...(dims)>{dims...});
template<class T, int... dims> using fixed_multi_array_container = std::array<T,cartesian_prod<dims...>>;

template<class T, int... dims>
using fixed_multi_array = multi_array< fixed_multi_array_container<T,dims...> , fixed_shape<dims...>>;
// fixed_multi_array }


// dyn_multi_array {
using default_index_type = int; // int is sometimes faster. Arrays with size > 2^31 are rare
                                // (because when arrays are this big, they better be distributed)

template<class T, int rank, class Integer = default_index_type>
using dyn_multi_array = multi_array< std::vector<T> , dyn_shape<Integer,rank>>;

template<class T, int rank, class Integer = default_index_type>
using dyn_multi_array_view = multi_array< std_e::span<T,dynamic_size> , dyn_shape<Integer,rank>>;
// dyn_multi_array }


// traits {
template<class T>
struct is_multi_array_impl {
  static constexpr bool value = false;
};
template<class M0, class M1>
struct is_multi_array_impl<std_e::multi_array<M0,M1>> {
  static constexpr bool value = true;
};
template<class T> constexpr bool is_multi_array = is_multi_array_impl<T>::value;
template<class T> constexpr bool is_1d_array = is_array<T> && !is_multi_array<T>;
// traits }


} // std_e
