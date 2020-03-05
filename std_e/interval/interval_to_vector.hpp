#pragma once


#include "std_e/interval/interval.hpp"
#include <vector>
#include <numeric>


namespace std_e {


template<class I> auto 
interval_to_vector(interval<I> inter) -> std::vector<I> {
  std::vector<I> res(size(inter));
  std::iota(res.begin(),res.end(),inter.first);
  return res;
}

template<class I> auto 
interval_to_vector(I n) -> std::vector<I> {
  // Precondition: n>=0
  return interval_to_vector(interval<I>{0,n});
}


} // std_e
