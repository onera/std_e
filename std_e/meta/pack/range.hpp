#pragma once


#include "std_e/future/contract.hpp"


namespace std_e {


template<class Rng, class... Rngs> auto
common_size(const Rng& x, const Rngs&... xs) {
  auto sz = x.size();
  ( STD_E_ASSERT(xs.size() == sz) , ...);
  return sz;
}


} // std_e
