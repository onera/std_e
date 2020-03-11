#pragma once


#include <utility>


namespace std_e {


template<class Integer, Integer... Is> constexpr auto 
get_value_at(std::size_t i, std::integer_sequence<Integer, Is...>) -> Integer {
  constexpr Integer arr[] = {Is...};
  return arr[i];
}


template<std::size_t... Is, std::size_t... first_Is> constexpr auto
take_n__impl(std::index_sequence<Is...> int_seq, std::index_sequence<first_Is...>) {
  return std::index_sequence<get_value_at(first_Is,int_seq)...>{};
}

template<std::size_t N, std::size_t... Is> constexpr auto
take_n(std::index_sequence<Is...> int_seq) {
  return take_n__impl(int_seq,std::make_index_sequence<N>{});
}


} // std_e
