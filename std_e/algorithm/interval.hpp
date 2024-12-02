#pragma once


#include <algorithm>
#include "std_e/future/algorithm.hpp"
#include "std_e/future/dynarray.hpp"
#include "std_e/algorithm/numeric.hpp"


namespace std_e {


template<class Rng> auto
is_interval(const Rng& interv) -> bool {
  return (interv.size() >= 1) && std::is_sorted(interv.begin(),interv.end());
}
template<class Rng> auto
is_offset(const Rng& off) -> bool {
  return is_interval(off) && off[0] == 0;
}


template<class Number, class Rng> auto
interval_index(Number x, const Rng& interv) -> ptrdiff_t {
  auto it = std::upper_bound(interv.begin(),interv.end(),x);
  return it-begin(interv)-1;
}


template<class Rng, class Out_rng> auto
offset_to_stride(const Rng& off, Out_rng& bsz) -> void {
  STD_E_ASSERT(bsz.size() >= off.size()-1);
  std_e::exclusive_adjacent_difference(off.begin(),off.end(),bsz.begin());
}
template<class Rng, class Out_rng> auto
stride_to_offset(const Rng& bsz, Out_rng& off) -> void {
  STD_E_ASSERT(off.size() >= bsz.size()+1);
  off[0] = 0;
  std_e::inclusive_scan(bsz.begin(),bsz.end(),off.begin()+1);
}


template<class Rng, class T = std::remove_const_t<typename Rng::value_type>> auto
offset_to_stride(const Rng& off) -> std_e::dynarray<T> {
  dynarray<T> bsz(off.size()-1);
  offset_to_stride(off, bsz);
  return bsz;
}
template<class Rng, class T = typename Rng::value_type> auto
stride_to_offset(const Rng& bsz) -> std_e::dynarray<T> {
  dynarray<T> off(bsz.size()+1);
  stride_to_offset(bsz, off);
  return off;
}


template<class Rng> auto
inplace_stride_to_offset(Rng& bsz) -> Rng& {
  // TODO make it more STL-like
  using I = typename Rng::value_type;
  I off_cur = 0;
  I n_blk = bsz.size()-1;
  for (I i=0; i<n_blk; ++i) {
    // loop invariant: off_cur = sum( k=[0,i) ) (bsz[k])
    I off_next = off_cur + bsz[i];
    bsz[i] = off_cur;
    off_cur = off_next;
  }
  bsz[n_blk] = off_cur;
  return bsz;
}

} // std_e
