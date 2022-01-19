#pragma once


#include "std_e/algorithm/pivot_partition_range.hpp"
#include "std_e/meta/pack/range.hpp"


namespace std_e {


// TODO facto with parallel/_global_sort
template<class Pivot_range, class Proj, class Comp, class Return_container>
struct _indirect_pivot_partition_fn {
  const Pivot_range& pivots;
  Proj proj;
  Comp comp;

  _indirect_pivot_partition_fn(const Pivot_range& pivots, Proj proj, Comp comp, Return_container&&)
    : pivots(pivots)
    , proj(std::move(proj))
    , comp(std::move(comp))
  {}


  template<class... Rngs> auto
  operator()(Rngs&... xs) {
    auto sz = common_size(xs...);

    using I = typename Return_container::value_type;
    std::vector<I> perm(sz);
    std::iota(begin(perm),end(perm),0);

    auto proj_indirect = [&xs...,proj=this->proj](I i){ return proj(xs[i]...); };

    auto partition_indices = pivot_partition_indices(perm,pivots,proj_indirect,comp,Return_container{});
    ( permute(xs.begin(),perm) , ... );

    return partition_indices;
  }
};


template<
  class... Rngs,
  class Pivot_rng,
  class Proj,
  class Comp = std::less<>,
  class Return_container = interval_vector<int>,
> auto
indirect_pivot_partition(std::tuple<Rngs&...>&& xs_tuple, const Pivot_rng& pivots, Proj proj, Comp comp = {}, Return_container&& = {}) {
  return std::apply(_indirect_pivot_partition_fn(pivots,proj,comp,Return_container{}), xs_tuple);
}


} // std_e
