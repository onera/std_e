#pragma once


#include "std_e/meta/pack/range.hpp"
#include "std_e/interval/interval_sequence.hpp"


namespace std_e {


template<class Rng, class Pivot_range, class Proj, class F_copy, class Comp> auto
_pivot_partition_copy(Rng&& x, const Pivot_range& pivots, Proj proj, F_copy f_copy, Comp comp, int shift) -> void {
  auto sz = x.size();
  using I = decltype(sz);
  for (I i=0; i<sz; ++i) {
    auto it = std::upper_bound(begin(pivots),end(pivots),proj(x[i]));
    auto index = it-begin(pivots)-shift;
    f_copy(index,x[i]);
  }
}


// pivot_partition_copy & interval_partition_copy {
// both algorithm essentially do the same thing, but
//   pivot_partition_copy considers that the index before the first pivot is 0
//      because it is "standard" to be before something called a "pivot"
//   interval_partition_copy considers that the index before the first interval tick is -1
//      because it seems natural to be at position 0 if you belong to interval 0
template<
  class Rng,
  class Pivot_rng,
  class Proj,
  class F_copy,
  class Comp = std::less<>
> auto
pivot_partition_copy(Rng&& x, const Pivot_rng& pivots, Proj proj, F_copy f_copy, Comp comp = {}) -> void {
  return _pivot_partition_copy(x,pivots,proj,f_copy,comp,0);
}

template<
  class Rng,
  class Pivot_rng,
  class Proj,
  class F_copy,
  class Comp = std::less<>
> auto
interval_partition_copy(Rng&& x, const Pivot_rng& pivots, Proj proj, F_copy f_copy, Comp comp = {}) -> void {
  return _pivot_partition_copy(x,pivots,proj,f_copy,comp,1);
}
// pivot_partition_copy & interval_partition_copy }


} // std_e
