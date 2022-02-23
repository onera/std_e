#pragma once


#include "std_e/parallel/algorithm/sort_by_rank.hpp"
#include "std_e/parallel/mpi/collective/all_to_all.hpp"
#include "std_e/algorithm/permutation.hpp"
#include "std_e/meta/pack/range.hpp"


namespace std_e {


template<
  class Rng,
  class Proj = identity_closure,
  class Comp = std::less<>,
  class Return_container = interval_vector<int>,
  class Sort_algo = decltype(std::ranges::sort)
> auto
sort(Rng& x, MPI_Comm comm, Proj proj = {}, Comp comp = {}, double max_imbalance = 0., Return_container&& = {}, Sort_algo sort_algo = {}) {
  // 0. global partitioning
  auto rank_indices = std_e::sort_by_rank(x,comm,proj,comp,max_imbalance,Return_container{});

  // 1. exchange
  auto [x_part,_] = all_to_all(x,rank_indices,comm);

  // 2. local sort
  sort_algo(x_part,comp,proj);

  // 3. return
  auto new_distri = all_reduce(rank_indices.as_base(),MPI_SUM,comm); // same as ex_scan(x_part.size()) // TODO as_base perm ugly!
  return std::make_pair(x_part,new_distri);
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
  class Sort_algo = decltype(std::ranges::sort)
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
