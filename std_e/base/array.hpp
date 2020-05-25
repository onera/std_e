#pragma once


#include <array>
#include "std_e/concept/array.hpp"
// TODO DEL


namespace std_e {


/**
 *  \brief  std_e::array is just a std::array with an **int** parameter size (instead of a **size**)
 *          
 */
template<class T, int N>
class array : public std::array<T,N>
{};

// std_e::array activations
template<class T, int N>
struct enable_is_array<std_e::array<T,N>> : std::true_type {};
template<class T, int N>
struct enable_is_fixed_size_array<std_e::array<T,N>> : std::true_type {};

}

namespace std {
template<class T, int N>
struct tuple_size<std_e::array<T,N>> {
  static constexpr size_t value = N;
};
}
