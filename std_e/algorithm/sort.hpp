#pragma once


#include <algorithm>
#include <functional>
#include "std_e/algorithm/permutation.hpp"
#include "std_e/utils/vector.hpp"
#include "std_e/utils/functional.hpp"
#include <ranges>


namespace std_e {


template<std::ranges::range Rng, std::ranges::range Int_rng, class Comp = std::less<>, class sort_algo_type = decltype(std_sort_lambda)> auto
sort_permutation(const Rng& x, Int_rng& perm, Comp&& comp = {}, sort_algo_type sort_algo = std_sort_lambda) -> void {
  using I = typename Int_rng::value_type;
  static_assert(std::is_integral_v<I>);

  sort_algo(perm.begin(), perm.end(), [&](I i, I j){ return comp(x[i], x[j]); });
}


template<std::ranges::range Rng, std::ranges::range Int_rng, class Comp = std::less<>> auto
indirect_sort(Rng& x, Int_rng& perm, Comp&& comp = {}) -> void {
  sort_permutation(x, perm, comp);
  permute(x,perm);
}


// TODO this sorts based on first range... but might want to do something else (lexico...). Look at parallel_sort to do better
// TODO deprecate, use `std_e::ranges::sort( std_e::zip(...), proj<0>)`
template<class Tuple, class Comp = std::less<>, class sort_algo_type = decltype(std_sort_lambda)> auto
zip_sort(Tuple&& rngs, Comp&& comp = {}, sort_algo_type sort_algo = std_sort_lambda) {
  auto& first_range = std::get<0>(rngs);
  auto perm = sort_permutation(first_range,comp,sort_algo);
  apply_permutation(perm,rngs);
  return perm;
}


} // std_e
