#pragma once


#include <algorithm>
#include "std_e/utils/functional.hpp"
#include "std_e/future/dynarray.hpp"


namespace std_e {


template<class Rng, class Comp = std::less<>, class sort_algo_type = decltype(std_sort_lambda)> auto
sorted(const Rng& x, Comp&& comp = {}, sort_algo_type sort_algo = std_sort_lambda) {
  using T = std::decay_t<Rng>::value_type;
  std_e::dynarray<T> res(x.data(), x.data()+x.size());
  sort_algo(res.begin(), res.end(), comp);
  return res;
}


} // std_e
