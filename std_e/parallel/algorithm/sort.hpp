#pragma once


#include "std_e/parallel/algorithm/sort_by_rank.hpp"
#include "std_e/parallel/mpi/collective/all_to_all.hpp"
#include "std_e/parallel/distribution.hpp"
#include "std_e/parallel/partial_distribution.hpp"
#include "std_e/algorithm/permutation.hpp"
#include "std_e/meta/pack/range.hpp"
#include "std_e/future/sort/sort_ranges.hpp"
#include <climits>
#include "std_e/logging/time_logger.hpp"


namespace std_e {


template<
  class Rng,
  class Proj = identity_closure,
  class Comp = std::less<>,
  class Sort_algo = decltype(std_e::ranges::sort)
> auto
sort(
  Rng& x, MPI_Comm comm, Proj proj = {}, Comp comp = {},
  double max_imbalance = 0., Sort_algo sort_algo = {}
)
{
  // 0. preconditions
  STD_E_ASSERT(x.size() > (size_t)INT_MAX); // needed because we use `int` for indexing into `x` (see step 2.)

  // 1. global partitioning

  auto tl = std_e::time_logger("maia","sort_by_rank");
  auto rank_indices = std_e::sort_by_rank(x,comm,proj,comp,max_imbalance,interval_vector<int>{});
  tl.stop();

  // 2. exchange
  auto tl1 = std_e::time_logger("maia","all to all");
  auto [x_part,_] = all_to_all(x,rank_indices,comm); // `rank_indices::value_type == int` because `MPI_Alltoall` requires `int`
                                                     // (and `MPI_Alltoall_c` is only available in MPI >= 4)
  tl1.stop();

  // 3. local sort
  auto tl2 = std_e::time_logger("maia","sort_local");
  sort_algo(x_part,comp,proj);
  tl2.stop();

  return x_part;
}
template<
  class Rng,
  class Proj = identity_closure,
  class Comp = std::less<>,
  class Sort_algo = decltype(std_e::ranges::sort)
> auto
sort2(
  Rng& x, MPI_Comm comm, Proj proj = {}, Comp comp = {},
  double max_imbalance = 0., Sort_algo sort_algo = {}
)
{
  // 0. preconditions
  STD_E_ASSERT(x.size() > (size_t)INT_MAX); // needed because we use `int` for indexing into `x` (see step 2.)

  // 1. global partitioning

  auto tl = std_e::time_logger("maia","sort_by_rank");
  auto rank_indices = std_e::sort_by_rank(x,comm,proj,comp,max_imbalance,interval_vector<int>{});
  tl.stop();

  // 2. exchange
  auto tl1 = std_e::time_logger("maia","all to all");
  auto [x_part,_] = all_to_all(x,rank_indices,comm); // `rank_indices::value_type == int` because `MPI_Alltoall` requires `int`
                                                     // (and `MPI_Alltoall_c` is only available in MPI >= 4)
  tl1.stop();

  // 3. local sort
  auto tl2 = std_e::time_logger("maia","sort_local");
  //// 3.0
  sort_algo(x_part,comp,proj);
  //// 3.1
  //auto& x1_sorted = std_e::range<0>(x_part);
  //auto& x2_sorted = std_e::range<1>(x_part);
  //auto comp2 = [&x1_sorted](auto i, auto j){ return x1_sorted[i] < x1_sorted[j]; };
  //std::sort(x2_sorted.begin(),x2_sorted.end(),comp2);
  //permute(x1_sorted.begin(),x2_sorted);
  //// 3.2
  //auto& x1_sorted = std_e::range<0>(x_part);
  //auto& x2_sorted = std_e::range<1>(x_part);
  //int64_t max_int = 1000000; 
  //std::vector<int8_t> counts(max_int, 0);
  //for (int64_t x : x1_sorted) {
  //  ++counts[x];
  //}
  tl2.stop();

  return x_part;
}

template<
  class Rng,
  class Proj = identity_closure,
  class Comp = std::less<>,
  class Sort_algo = decltype(std_e::ranges::sort)
> auto
sort_old(
  Rng& x, MPI_Comm comm, Proj proj = {}, Comp comp = {},
  double max_imbalance = 0., Sort_algo sort_algo = {}
)
{
  auto x_part = sort(x, comm, proj, comp, max_imbalance, sort_algo);
  auto distri = distribution_from_dsizes(int64_t(x_part.size()), comm);
  return std::make_pair(std::move(x_part),std::move(distri));
}

template<class Proj, class Comp, class Return_container, class Sort_algo>
struct _global_sort {
  MPI_Comm comm;
  Proj proj;
  Comp comp;
  double max_imbalance;
  Sort_algo sort_algo;

  _global_sort(MPI_Comm comm, Proj proj, Comp comp, double max_imbalance, Return_container&&, Sort_algo sort_algo)
    : comm(comm)
    , proj(std::move(proj))
    , comp(std::move(comp))
    , max_imbalance(max_imbalance)
    , sort_algo(std::move(sort_algo))
  {}


  template<class... Rngs> auto
  operator()(Rngs&... xs) {
    auto sz = common_size(xs...);

    using I = typename Return_container::value_type;
    std::vector<I> perm(sz);
    std::iota(begin(perm),end(perm),0);

    auto proj_indirect = [&xs...,proj=this->proj](I i){ return proj(xs[i]...); };

    auto rank_indices = std_e::sort_by_rank(perm,comm,proj_indirect,comp,max_imbalance,Return_container{});
    ( permute(xs.begin(),perm) , ... );

    return rank_indices;
  }
};

template<class Proj, class Comp, class Return_container, class Sort_algo, class Distribution>
struct _local_sort {
  Proj proj;
  Comp comp;
  Sort_algo sort_algo;
  Distribution distri;

  _local_sort(Proj proj, Comp comp, Return_container&&, Sort_algo sort_algo, Distribution distri)
    : proj(std::move(proj))
    , comp(std::move(comp))
    , sort_algo(std::move(sort_algo))
    , distri(std::move(distri))
  {}

  template<class... Rngs> auto
  operator()(Rngs&... xs_part) {
    auto sz = common_size(xs_part...);

    using I = typename Return_container::value_type;
    std::vector<I> perm_local(sz);
    std::iota(begin(perm_local),end(perm_local),0);

    auto proj_indirect_local = [&xs_part...,proj=this->proj](I i){ return proj(xs_part[i]...); };

    sort_algo(perm_local,comp,proj_indirect_local);
    ( permute(xs_part.begin(),perm_local) , ... );

    return std::make_tuple(std::move(xs_part)...,std::move(distri));
  }
};

template<
  class... Rngs,
  class Proj = identity_closure,
  class Comp = std::less<>,
  class Return_container = interval_vector<int>,
  class Sort_algo = decltype(std_e::ranges::sort)
> auto
indirect_sort(std::tuple<Rngs&...>&& xs_tuple, MPI_Comm comm, Proj proj = {}, Comp comp = {}, double max_imbalance = 0., Return_container&& = {}, Sort_algo sort_algo = {}) {
  // 0. global partitioning
  auto rank_indices = std::apply(_global_sort(comm,proj,comp,max_imbalance,Return_container{},sort_algo), xs_tuple);

  // 1. exchange
  auto [xs_part_tuple,_] = all_to_all(xs_tuple,rank_indices,comm);

  // new distribution of the ranges
  auto new_distri = all_reduce(rank_indices.as_base(),MPI_SUM,comm); // same as ex_scan(x_part.size()) // TODO as_base perm ugly!

  // 3. local sort
  return std::apply(_local_sort(proj,comp,Return_container{},sort_algo,std::move(new_distri)), xs_part_tuple);
}


} // std_e
