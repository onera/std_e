#pragma once


#include "std_e/algorithm/iota.hpp"
#include "std_e/future/dynarray.hpp"


namespace std_e {


inline auto
step_dynarray(int start, int sz, int step = 1) {
  std_e::dynarray<int> v(sz);
  std_e::exclusive_iota(begin(v),end(v),start,step);
  return v;
}
inline auto
iota_dynarray(int sz) {
  return step_dynarray(0,sz,1);
}


} // std_e
