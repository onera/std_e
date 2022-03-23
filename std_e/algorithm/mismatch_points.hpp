#pragma once


#include <vector>
#include <functional>
#include "std_e/utils/functional.hpp"


namespace std_e {


template<class Input_it, class Comp, class Proj, class F> constexpr auto
for_each_mismatch(Input_it first, Input_it last, Comp comp, Proj proj, F f) -> void {
  if (first==last) return;
  Input_it prev = first;
  f(first);
  while (++first!=last) {
    if (!comp(proj(*prev),proj(*first))) {
      f(first);
      prev = first;
    }
  }
}

template<class Random_access_rng, class Comp = std::equal_to<>, class Proj = identity_closure, class I = int> auto
mismatch_indices(const Random_access_rng& rng, Comp comp = {}, Proj proj = {}, I = {}) -> std::vector<I> {
  auto first = begin(rng);
  std::vector<I> res;
  auto f = [first,&res](auto it){ res.push_back(it-first); };
  for_each_mismatch(begin(rng),end(rng),comp,proj,f);
  res.push_back(rng.size());
  return res;
}


} // std_e
