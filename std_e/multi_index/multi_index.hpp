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
template<class Integer, int rank>
struct multi_index__impl {
  using type = std_e::array<Integer,rank>;
};
template<class Integer>
struct multi_index__impl<Integer,dynamic_size> {
  using type = std::vector<Integer>;
};

template<class Integer, int rank>
using multi_index = typename multi_index__impl<Integer,rank>::type;

//template<class Integer, int rank>
//struct multi_index : std_e::array<Integer,rank>
//{};
//
//template<class Integer>
//struct multi_index<Integer,dynamic_size> : std::vector<Integer>
//{};
// multi_index }


//// is_multi_index {
//template<class T>
//struct is_multi_index__impl {
//  static constexpr bool value = false;
//};
//template<class Integer, int rank>
//struct is_multi_index__impl<multi_index<Integer,rank>> {
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

//template<class Integer, int rank>
//struct rank_of__impl<std_e::multi_index<Integer,rank>> {
//  static constexpr int value = rank;
//};
//template<class Integer>
//struct rank_of__impl<std_e::multi_index<Integer,dynamic_size>> {
//  static constexpr int value = dynamic_size;
//};
template<class Integer, int rank>
struct rank_of__impl<std_e::array<Integer,rank>> {
  static constexpr int value = rank;
};
template<class Integer>
struct rank_of__impl<std::vector<Integer>> {
  static constexpr int value = dynamic_size;
};
template<>
struct rank_of__impl<std_e::cx_multi_index> {
  static constexpr int value = dynamic_size;
};

template<class Multi_index> inline constexpr int rank_of = rank_of__impl<std::decay_t<Multi_index>>::value;
// rank_of }




// zero multi_index {
template<class Integer, int N> constexpr multi_index<Integer,N> zero_multi_index = default_array<Integer,N>;

//// TODO move
//template<class T, int ct_size>
//struct make_array_of_size__impl<std_e::multi_index<T,ct_size>> {
//  static constexpr auto
//  func(size_t sz) -> std_e::multi_index<T,ct_size> {
//    STD_E_ASSERT(sz==ct_size);
//    return {};
//  }
//};
//template<class T>
//struct make_array_of_size__impl<std_e::multi_index<T,dynamic_size>> {
//  static constexpr auto
//  func(size_t sz) -> std_e::multi_index<T,dynamic_size> {
//    return std_e::multi_index<T,dynamic_size>(sz);
//  }
//};
//
//template<class T, int... ct_sizes>
//struct concatenated_array__impl<std_e::multi_index<T,ct_sizes>...> {
//  static constexpr int sum_ct_sizes = (ct_sizes + ...);
//  using type = std_e::multi_index<T,sum_ct_sizes>;
//};
//
//template<class T, class... Ts>
//struct concatenated_array__impl<std_e::multi_index<T,dynamic_size>,std_e::multi_index<Ts,dynamic_size>...> {
//  using type = std_e::multi_index<T,dynamic_size>;
//};
//// END TODO

template<class Multi_index> constexpr auto
make_zero_multi_index(int rank) {
  auto zero = make_array_of_size<Multi_index>(rank);
  std_e::fill(begin(zero),end(zero),0);
  return zero;
}
// zero multi_index }


} // std_e
