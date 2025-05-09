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
iota_dynarray(I start, I stop) {
  return step_dynarray(start,stop-start,I(1));
}
template<class I> auto
iota_dynarray(I n) {
  return iota_dynarray(I(0),n);
}


} // std_e
