#pragma once


#include <vector>
#include <functional>


namespace std_e {


template<class Input_it, class Binary_pred, class F> constexpr auto
for_each_mismatch(Input_it first, Input_it last, Binary_pred p, F f) -> void {
  if (first==last) return;
  Input_it prev = first;
  f(first);
  while (++first!=last) {
    if (!p(*prev,*first)) {
      f(first);
      prev = first;
    }
  }
}

template<class Random_access_rng, class Binary_pred = std::equal_to<>> auto
mismatch_indices(const Random_access_rng& rng, Binary_pred p = {}) -> std::vector<int> {
  auto first = begin(rng);
  std::vector<int> res;
  auto f = [first,&res](auto it){ res.push_back(it-first); };  
  for_each_mismatch(begin(rng),end(rng),p,f);
  return res;
}


} // std_e
