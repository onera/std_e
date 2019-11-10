#pragma once


#include "std_e/multi_array/multi_array/multi_array.hpp"
#include <array>
#include "std_e/multi_array/shape/fixed_shape.hpp"
#include "std_e/multi_array/shape/dyn_shape.hpp"
#include "std_e/multi_array/shape/dyn_rank_shape.hpp"
#include "std_e/base/memory_view.hpp"


namespace std_e {


/// fixed_multi_array {
template<int... dims> constexpr size_t cartesian_prod = cartesian_product(std_e::multi_index<int,sizeof...(dims)>{dims...});
template<class T, int... dims> using fixed_multi_array_container = std::array<T,cartesian_prod<dims...>>;

template<class T, int... dims>
using fixed_multi_array = multi_array< fixed_multi_array_container<T,dims...> , fixed_shape<dims...> >;
/// fixed_multi_array }


/// dyn_multi_array {
template<class T, class Integer, size_t rank>
using dyn_multi_array = multi_array< std::vector<T> , dyn_shape<Integer,rank> >;
/// dyn_multi_array }


/// dyn_rank_multi_array {
template<class T, class Integer>
using dyn_rank_multi_array = multi_array< std::vector<T> , dyn_rank_shape<Integer> >;

template<class T, class Integer>
using dyn_rank_multi_array_view = multi_array< std_e::memory_view<T*> , dyn_rank_shape<Integer> >;
/// dyn_multi_array }



} // std_e
