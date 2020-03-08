#pragma once


namespace std_e {


template<class Multi_index_0, class Multi_index_1> constexpr auto
increment_multi_index_fortran_order(const Multi_index_0& dims, Multi_index_1& indices) -> int {
  constexpr int rank = std::tuple_size<Multi_index_0>::value;
  int i=0;
  while (++indices[i]==dims[i]) {
    indices[i]=0;
    ++i;
    if (i==rank) return rank;
  }
  return i;
}

template<class Multi_index_0, class Multi_index_1> constexpr auto
increment_multi_index_c_order(const Multi_index_0& dims, Multi_index_1& indices) -> int {
  // TODO false!
  //int i=dims.size()-1;
  //while (++indices[i]==dims[i]) {
  //  indices[i]=0;
  //  --i;
  //}
  int i=0;
  while (++indices[i]==dims[i]) {
    indices[i]=0;
    ++i;
  }
  return i;
}


} // std_e
