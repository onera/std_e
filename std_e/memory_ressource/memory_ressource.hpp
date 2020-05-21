#pragma once


#include "std_e/memory_ressource/concept.hpp"
#include "std_e/base/memory_view.hpp"


namespace std_e {


template<class Memory_ressource>
struct memory_is_owned__impl {
  static constexpr bool value = true; // by default, memory is owned by the ressource (std::array, std::vector...)
};
template<class T>
struct memory_is_owned__impl<memory_view<T*>> {
  static constexpr bool value = false;
};


template<class Memory_ressource> constexpr bool memory_is_owned = memory_is_owned__impl<Memory_ressource>::value;


} // std_e
