#pragma once


#include <array>
#include <vector>
#include "std_e/future/span.hpp"
#include "std_e/future/constexpr_vector.hpp"
#include "std_e/memory_ressource/concept.hpp"
#include "std_e/base/memory_view.hpp"


namespace std_e {


/*
  concept Memory_ressource_provider:
    static constexpr bool owns_memory
    exists template<class T> memory_ressource_for<T,Memory_ressource> -> Memory_ressource with Memory_ressource::value_type==T
*/


/// owned memory {
template<size_t N>
struct fixed_size_owned_memory_ressource {
  static constexpr bool owns_memory = true;
  template<class T> using type = std::array<T,N>;
};
struct dyn_size_owned_memory_ressource {
  static constexpr bool owns_memory = true;
  template<class T> using type = std::vector<T>;
};
template<size_t N>
struct constexpr_dyn_size_owned_memory_ressource {
  static constexpr bool owns_memory = true;
  static constexpr size_t max_size = N;
  template<class T> using type = std_e::constexpr_vector<T,N>;
};
/// owned memory }


/// memory view {
struct external_memory_ressource {
  static constexpr bool owns_memory = false;
  template<class T> using type = std_e::memory_view<T*>;
};
template<size_t N>
struct constexpr_dyn_size_external_memory_ressource {
  static constexpr bool owns_memory = false;
  static constexpr size_t max_size = N; // remember max_size if asked...
  template<class T> using type = std_e::span<T,std_e::dynamic_size>; // ...but no need to use internally
};
struct dyn_size_external_memory_ressource {
  static constexpr bool owns_memory = false;
  template<class T> using type = std_e::span<T,std_e::dynamic_size>;
};
/// memory view }

/// const memory view {
struct const_external_memory_ressource {
  static constexpr bool owns_memory = false;
  template<class T> using type = std_e::memory_view<const T*>;
};
template<size_t N>
struct constexpr_const_dyn_size_external_memory_ressource {
  static constexpr bool owns_memory = false;
  static constexpr size_t max_size = N; // remember max_size if asked...
  template<class T> using type = std_e::memory_view<const T*>; // ...but no need to use internally
};
struct const_dyn_size_external_memory_ressource {
  static constexpr bool owns_memory = false;
  template<class T> using type = std_e::span<const T,std_e::dynamic_size>;
};
/// const memory view }


template<class T, class memory_ressource_type> using memory_ressource_for = typename memory_ressource_type::template type<T>;
/// memory_ressouce }


} // std_e
