#pragma once


#include "std_e/multi_index/multi_index.hpp"


namespace std_e {


template<class Multi_index_0, class Multi_index_1, int rank = rank_of<Multi_index_0>> constexpr auto
increment_multi_index_fortran_order(const Multi_index_0& dims, Multi_index_1& indices) -> int {
  int i=0;
  while (++indices[i]==dims[i]) {
    indices[i]=0;
    ++i;
    if (i==rank) return rank;
  }
  return i;
}
template<class Multi_index_0, class Multi_index_1, class Multi_index_2, int rank = rank_of<Multi_index_0>> constexpr auto
increment_multi_index(const Multi_index_0& dims, Multi_index_1& indices, Multi_index_2& order) -> int {
  int i=0;
  while (++indices[order[i]]==dims[order[i]]) {
    indices[order[i]]=0;
    ++i;
    if (i==rank) return rank;
  }
  return i;
}


} // std_e
