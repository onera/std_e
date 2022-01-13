#pragma once


#include "std_e/parallel/all_to_all.hpp"
#include "std_e/algorithm/pivot_partition_eq_range.hpp"
#include "std_e/algorithm/distribution.hpp"
#include "std_e/algorithm/algorithm.hpp"
#include "std_e/parallel/struct/distribution.hpp"
#include "std_e/parallel/algorithm/search_intervals.hpp"
#include "std_e/parallel/algorithm/pivot_partition_once.hpp"
#include "std_e/parallel/algorithm/exception.hpp"
#include "std_e/parallel/algorithm/ticks_in_interval.hpp"
#include <cmath>


namespace std_e {


// Inputs: array `x` of total size `sz_tot`, distributed over a communicator comm
// We want to partition `x` in n_rank buckets, with the all the total sizes of each bucket to be appox equal (up to +-max_imbalance)
// Notation:
//   sz: local size of `x`
// For that:
//   0. we make an initial partitioning
//        Output: `partition_indices`=[0, ... sz)
//        Deduced output: `partition_indices_tot`=[0, ... sz_tot), the global partition indices.
//   1. We want to iteratively partition x such that `partition_indices_tot` converges to [0 ... i*sz_tot/n_rank ... sz_tot)
template<
  class Rng,
  class Comp = std::less<>,
  class Proj = identity_closure,
  class Return_container = interval_vector<int>
> auto
//partition_indices(
pivot_partition_eq(
  Rng& x, MPI_Comm comm, Comp comp = {}, Proj proj = {}, double max_imbalance = 0., Return_container&& = {}
)
  -> Return_container
{
  using T = typename Rng::value_type;
  using T_piv = std::decay_t<decltype(proj(T{}))>;
  using I = typename Return_container::value_type;
  I sz_tot = all_reduce(x.size(),MPI_SUM,comm);
  const int n_rk = n_rank(comm);
  const I max_interval_tick_shift = (max_imbalance/2.) * double(sz_tot)/double(n_rk);


// 0. initial partitioning
  Return_container partition_indices = pivot_partition_once(x,comm,comp,proj,Return_container{});

  auto obj_ticks = objective_ticks(sz_tot,n_rk,0,n_rk-1);
  auto sub_ins = compute_intervals_containing_ticks(obj_ticks,partition_indices,max_interval_tick_shift,comm);

// 1. loop until there is no sub-interval to partition (that is, until all partition_indices are OK)
  int n_iter = 0;
  while (sub_ins.size()>0) {
    if (n_iter++>64) {
      // Note: there should be approximately log(sz_tot) loop iterations
      // If this is not the case, it could be due to
      //   - worst-case complexity scenario:
      //        => for specific inputs, the pivot selection is defective (should be as rare as median-of-3 quicksort pathological cases)
      //   - implementation bugs
      throw par_algo_exception("pivot_partition_minimize_imbalance: max number of iterations reached");
    }

    int n_sub_intervals = sub_ins.size();

    // 0. create sub-ranges of `x` that need partitioning
    std::vector<std_e::span<T>> x_sub(n_sub_intervals);
    for (int i=0; i<n_sub_intervals; ++i) {
      T* start  = x.data() + sub_ins[i].inf;
      T* finish = x.data() + sub_ins[i].sup;
      x_sub[i] = make_span(start,finish);
    }

    // 1. compute pivots
    std::vector<int> n_far_ticks(n_sub_intervals);
    for (int i=0; i<n_sub_intervals; ++i) {
      n_far_ticks[i] = sub_ins[i].n_tick;
    }
    std::vector<std::vector<T_piv>> pivots_by_sub_intervals = find_pivots(x_sub,n_far_ticks,comm,proj);

    // 2. partition sub-ranges, report results in partition_indices, and compute new sub-intervals
    std::vector<interval_containing_ticks<I>> new_sub_ins;
    for (int i=0; i<n_sub_intervals; ++i) {
      // 2.0. partition sub-ranges
      const std::vector<T_piv>& pivots = pivots_by_sub_intervals[i];
      auto partition_indices_sub = pivot_partition_eq_indices(x_sub[i],pivots,comp,proj,Return_container{});
      std_e::offset(partition_indices_sub,sub_ins[i].inf); // absolute indices

      // 2.1. compute new sub-intervals
      auto obj_ticks = objective_ticks(sz_tot,n_rk,sub_ins[i].position,sub_ins[i].n_tick);
      auto new_sub_ins_i =
        compute_interval_containing_ticks2(
          obj_ticks,partition_indices_sub,max_interval_tick_shift,
          sub_ins[i].position,partition_indices,comm
        );
      append(new_sub_ins,new_sub_ins_i);
    }

    // 3. loop
    sub_ins = std::move(new_sub_ins);
  }

  return partition_indices;
}


} // std_e
