#pragma once


#include "std_e/algorithm/iota.hpp"
#include "std_e/future/dynarray.hpp"


namespace std_e {


template<class I> auto
step_dynarray(I start, I sz, I step = 1) {
  std_e::dynarray<I> v(sz);
  std_e::exclusive_iota(begin(v),end(v),start,step);
  return v;
}
template<class I> auto
iota_dynarray(I sz) {
  return step_dynarray(I(0),sz,I(1));
}


} // std_e
