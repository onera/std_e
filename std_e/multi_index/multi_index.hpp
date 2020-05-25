#pragma once


#include "std_e/multi_index/concept.hpp"
#include "std_e/multi_index/cx_multi_index.hpp"
#include "std_e/utils/array.hpp"
#include "std_e/base/dynamic_size.hpp"
#include "std_e/concept/array.hpp"
#include "std_e/future/algorithm.hpp"
#include "std_e/utils/array_vector_common.hpp"


namespace std_e {


// multi_index {

// IMPLEMENTATION DESIGN NOTE: we use inheritance to define multi_index
//   - inheritance should be viewed as an implementation detail:
//       we don't want to manually wrap std::array and std::vector
//   - we don't use type aliases to std::array and std::vector
//       because type aliases are never deduced;
//       hence, they often feel like second-citizen types,
//       in particular regarding overloading
//       (and we will need to define a fair number of multi_index functions)
template<class Int, int rank>
struct multi_index : std_e::array<Int,rank>
{};

template<class Int>
struct multi_index<Int,dynamic_size> : std::vector<Int> {
  using base = std::vector<Int>;
  using base::base; // inherit ctors too
};


/// tuple protocol {
} /* close std_e */
template<class Int, int rank>
struct std::tuple_size<std_e::multi_index<Int,rank>> {
  static constexpr size_t value = rank;
};
/* re-open std_e */ namespace std_e {
/// tuple protocol }

/// enabling/disabling Array concepts {
template<class Int, int N> struct enable_is_array           <multi_index<Int,N           >> : std::true_type  {};

template<class Int, int N> struct enable_is_fixed_size_array<multi_index<Int,N           >> : std::true_type  {};
template<class Int       > struct enable_is_fixed_size_array<multi_index<Int,dynamic_size>> : std::false_type {};
template<class Int       > struct enable_is_dyn_size_array  <multi_index<Int,dynamic_size>> : std::true_type  {};
/// enabling/disabling Array concepts }
// multi_index }


//// is_multi_index {
//template<class T>
//struct is_multi_index__impl {
//  static constexpr bool value = false;
//};
//template<class Int, int rank>
//struct is_multi_index__impl<multi_index<Int,rank>> {
//  static constexpr bool value = true;
//};
//
//template<class T>
//constexpr bool is_multi_index = is_multi_index__impl<T>::value;
//// is_multi_index }

// index_type_of {
template<class Multi_index>
using index_type_of = typename Multi_index::value_type;
// index_type_of }


// rank_of {
template<class Multi_index>
struct rank_of__impl;

template<class Int, int rank>
struct rank_of__impl<std_e::multi_index<Int,rank>> {
  static constexpr int value = rank;
};
template<class Int>
struct rank_of__impl<std_e::multi_index<Int,dynamic_size>> {
  static constexpr int value = dynamic_size;
};
//template<class Int, int rank>
//struct rank_of__impl<std_e::array<Int,rank>> {
//  static constexpr int value = rank;
//};
//template<class Int>
//struct rank_of__impl<std::vector<Int>> {
//  static constexpr int value = dynamic_size;
//};
//template<>
//struct rank_of__impl<std_e::cx_multi_index> {
//  static constexpr int value = dynamic_size;
//};

template<class Multi_index> inline constexpr int rank_of = rank_of__impl<std::decay_t<Multi_index>>::value;
// rank_of }




// zero multi_index {
template<class Int, int N> constexpr multi_index<Int,N> zero_multi_index = default_array<Int,N>;

template<class Multi_index> constexpr auto
make_zero_multi_index(int rank) {
  auto zero = make_array_of_size<Multi_index>(rank);
  std_e::fill(begin(zero),end(zero),0);
  return zero;
}
// zero multi_index }


} // std_e
