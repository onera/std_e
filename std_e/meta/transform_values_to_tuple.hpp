#pragma once


#include <array>
#include <tuple>
#include "std_e/meta/compile_time_size.hpp"


namespace std_e {


template<auto value_array, template<auto> class value_to_type_transformation, size_t... Is> constexpr auto
transform_values_to_tuple_impl_idx_seq(std::index_sequence<Is...>) {
  return std::tuple< value_to_type_transformation<value_array[Is]>... >{};
}

template<auto value_array, template<auto> class value_to_type_transformation> constexpr auto
transform_values_to_tuple_impl() {
  constexpr size_t N = compile_time_size<std::decay_t<decltype(value_array)>>;
  return transform_values_to_tuple_impl_idx_seq<value_array,value_to_type_transformation>(std::make_index_sequence<N>{});
}

template<auto value_array, template<auto> class value_to_type_transformation>
using transform_values_to_tuple = decltype(transform_values_to_tuple_impl<value_array,value_to_type_transformation>());


} // std_e
