#pragma once


#include <algorithm>
#include "std_e/future/algorithm.hpp"
#include "std_e/future/dynarray.hpp"
#include "std_e/algorithm/numeric.hpp"


namespace std_e {


template<class Rng> auto
is_interval(const Rng& idx) -> bool {
  return (idx.size() >= 1) && std::is_sorted(idx.begin(),idx.end());
}


template<class Number, class Rng> auto
interval_index(Number x, const Rng& idx) -> ptrdiff_t {
  auto it = std::upper_bound(idx.begin(),idx.end(),x);
  return it-begin(idx)-1;
}


template<class Rng, class Out_rng> auto
index_to_size(const Rng& idx, Out_rng& bsz) -> void {
  STD_E_ASSERT(bsz.size() == idx.size()-1);
  std_e::exclusive_adjacent_difference(idx.begin(),idx.end(),bsz.begin());
}
template<class Rng, class Out_rng> auto
size_to_index(const Rng& bsz, Out_rng& idx) -> void {
  STD_E_ASSERT(idx.size() == bsz.size()+1);
  idx[0] = 0;
  std_e::inclusive_scan(bsz.begin(),bsz.end(),idx.begin()+1);
}


template<class Rng, class T = std::remove_const_t<typename Rng::value_type>> auto
index_to_size(const Rng& idx) -> std_e::dynarray<T> {
  dynarray<T> bsz(idx.size()-1);
  index_to_size(idx, bsz);
  return bsz;
}
template<class Rng, class T = typename Rng::value_type> auto
size_to_index(const Rng& bsz) -> std_e::dynarray<T> {
  dynarray<T> idx(bsz.size()+1);
  size_to_index(bsz, idx);
  return idx;
}


} // std_e
