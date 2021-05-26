#pragma once


#include <utility>


namespace std_e {


template<class Integer, Integer... Is> constexpr auto
get_value_at(Integer i, std::integer_sequence<Integer, Is...>) -> Integer {
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


template<class Int0, class Int1> constexpr auto
position_in_seq(Int0 /*i*/, std::integer_sequence<Int1>) -> int {
    return 0;
}
template<class Int0, class Int1, Int1 I, Int1... Is> constexpr auto
position_in_seq(Int0 i, std::integer_sequence<Int1,I,Is...>) -> int {
    return (i==I) ? 0 : 1 + position_in_seq(i,std::integer_sequence<Int1,Is...>());
}

template<class Int0, class Int1, Int1... Is> constexpr auto
contains(Int0 i, std::integer_sequence<Int1,Is...> idx_seq) -> bool {
    return position_in_seq(i,idx_seq) != sizeof...(Is);
}



} // std_e
