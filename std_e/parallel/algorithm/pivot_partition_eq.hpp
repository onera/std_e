#pragma once


#include "std_e/parallel/all_to_all.hpp"
#include "std_e/algorithm/pivot_partition_eq_range.hpp"
#include "std_e/algorithm/distribution.hpp"
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
  class T,
  class Comp = std::less<>,
  class Return_container = interval_vector<int>
> auto
pivot_partition_eq(
  std::vector<T>& x, MPI_Comm comm, double max_imbalance = 0., Comp comp = {}, Return_container&& partition_is = {}) -> Return_container {
  const int n_rk = n_rank(comm);

  size_t sz_tot = all_reduce(x.size(),MPI_SUM,comm);
  const int max_interval_tick_shift = (max_imbalance/2.) * double(sz_tot)/double(n_rk);


// 0. initial partitioning
  interval_vector<int> partition_indices = pivot_partition_once(x,comm,comp,std::move(partition_is));
  auto sub_ins = search_intervals3(partition_indices,max_interval_tick_shift,comm);

// 1. loop until there is no sub-interval to partition (that is, until all partition_indices are OK)
  int n_iter = 0;
  while (sub_ins.size()>0) {
    if (n_iter++>64) {
      // Note: there should be approximately log(sz_tot) loop iteration
      // If this is not the case, it could be due to
      //   - worst-case complexity scenario:
      //        => for specific inputs, the pivot selection is defective (should be as rare as median-of-3 quicksort pathological cases)
      //   - implementation bugs
      //        => in particular, special cases where the array is too small to get enough samples
      throw par_algo_exception("pivot_partition_minimize_imbalance: max number of iterations reached");
    }

    int n_sub_intervals = sub_ins.size();

    // 0. prepare for sampling
    std::vector<std_e::span<T>> x_sub(n_sub_intervals);
    for (int i=0; i<n_sub_intervals; ++i) {
      T* start  = x.data() + sub_ins[i].inf;
      T* finish = x.data() + sub_ins[i].sup;
      x_sub[i] = make_span(start,finish);
    }

    // 1. compute pivots
    std::vector<int> n_far_ticks(n_sub_intervals);
    for (int i=0; i<n_sub_intervals; ++i) {
      n_far_ticks[i] = sub_ins[i].n_ticks;
    }
    std::vector<std::vector<T>> pivots_by_sub_intervals = find_pivots(x_sub,n_far_ticks,comm);

    // 1. partition sub-intervals, report results in partition_indices, and compute new sub-intervals
    std::vector<interval_to_partition> new_sub_ins;
    for (int i=0; i<n_sub_intervals; ++i) {
      // 1.0. partition sub-intervals,
      const std::vector<T>& pivots = pivots_by_sub_intervals[i];
      auto partition_indices_sub = pivot_partition_eq_indices(x_sub[i],pivots,comp,{},Return_container{});
      for (size_t ii=0; ii<partition_indices_sub.size(); ++ii) {
        partition_indices_sub[ii] += sub_ins[i].inf;
      }

      // 1.1. compute new sub-intervals
      auto partition_indices_sub_tot = all_reduce(partition_indices_sub.as_base(),MPI_SUM,comm); // TODO outside of loop

      std::vector<int> objective_ticks_sub(sub_ins[i].n_ticks);
      for (int j=0; j<sub_ins[i].n_ticks; ++j) {
        objective_ticks_sub[j] = sub_ins[i].objective_tick(j);
      }
      auto [far_first_ticks,n_far_ticks,far_inter_indices,  near_tick_indices,near_inter_indices] = search_near_or_containing_interval2(objective_ticks_sub,partition_indices_sub_tot,max_interval_tick_shift);
      int n_sub_sub_intervals = far_inter_indices.size();

      // 1.2. create new sub-intervals
      for (int j=0; j<n_sub_sub_intervals; ++j) {
        int inf     = partition_indices_sub    [far_inter_indices[j]];
        int sup     = partition_indices_sub    [far_inter_indices[j] + 1];
        int inf_tot = partition_indices_sub_tot[far_inter_indices[j]];
        int sup_tot = partition_indices_sub_tot[far_inter_indices[j] + 1];
        new_sub_ins.push_back( {inf,sup,n_far_ticks[j],sub_ins[i].position+far_first_ticks[j], inf_tot,sup_tot,sz_tot,n_rk} );
      }

      // 1.3. report found ticks
      for (int j=0; j<(int)near_inter_indices.size(); ++j) {
        int k = near_inter_indices[j];
        int abs_pos = sub_ins[i].position+near_tick_indices[j];
        partition_indices[abs_pos] = partition_indices_sub[k];
      }
    }

    // 2. loop
    sub_ins = std::move(new_sub_ins);
  }

  return partition_indices;
}


} // std_e
