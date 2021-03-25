#pragma once

#include "std_e/multi_index/multi_index_range.hpp"
#include "std_e/meta/types.hpp"
#include <tuple>
#include <utility>


namespace std_e {

namespace detail {

// TODO waiting for multi_index_c_order
template<int mir_size, class multi_index_range_type> constexpr auto
generate(const multi_index_range_type& mir) {
  constexpr int sz = mir_size;
  constexpr int rank = 2;
  std::array<multi_index<int,rank>,sz> res = {};
  int i=0;
  for (auto mi : mir) {
    res[i] = mi;
    ++i;
  }
  return res;
}


template<class tuple_types, size_t Is_size, class T>
struct tuple_prod__rank_expansion__impl;

template<class... tuples, size_t Is_size, size_t... Is_rank>
struct tuple_prod__rank_expansion__impl<types<tuples...>,Is_size,std::index_sequence<Is_rank...>> {
  constexpr static auto mir = c_multi_index_range({(int)std::tuple_size_v<tuples>...});
  constexpr static int sz = mir.size();
  constexpr static auto explicit_mir = generate<sz>(mir);
  using type =
    std::tuple<
      std::tuple_element_t<explicit_mir[Is_size][Is_rank],tuples>...
    >;
};


template<class tuple_types, class T>
struct tuple_prod__size_expansion__impl;

template<class tuple_types, size_t... Is_size>
struct tuple_prod__size_expansion__impl<tuple_types,std::index_sequence<Is_size...>> {
  static constexpr int rank = tuple_types::size;
  using rank_index_seq = std::make_index_sequence<rank>;
  using type =
    std::tuple<
      typename tuple_prod__rank_expansion__impl<tuple_types,Is_size,rank_index_seq>::type ...
    >;
};


template<class... tuples>
struct tuple_prod__impl {
  static constexpr int size = (std::tuple_size_v<tuples> * ...);
  using size_index_seq = std::make_index_sequence<size>;
  using type = typename tuple_prod__size_expansion__impl<types<tuples...>,size_index_seq>::type;
};

} // detail

template<class... tuples>
using tuple_types_product = typename detail::tuple_prod__impl<tuples...>::type;


} // std_e
