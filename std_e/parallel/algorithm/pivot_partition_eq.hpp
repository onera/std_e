#pragma once


#include "std_e/parallel/all_to_all.hpp"
#include "std_e/algorithm/pivot_partition_range.hpp"
#include "std_e/algorithm/distribution.hpp"
#include "std_e/parallel/struct/distribution.hpp"
#include "std_e/parallel/algorithm/search_intervals.hpp"
#include "std_e/parallel/algorithm/pivot_partition_once.hpp"
#include "std_e/base/msg_exception.hpp"
#include "std_e/parallel/algorithm/ticks_in_interval.hpp"
#include "std_e/plog.hpp"
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
pivot_partition_eq(std::vector<T>& x, size_t sz_tot, MPI_Comm comm, Comp comp = {}, Return_container&& partition_is = {}) -> Return_container {
  const int n_rk = n_rank(comm);
  double max_imbalance = 0.5;
  const int max_interval_tick_shift = (max_imbalance/2.) * double(sz_tot)/double(n_rk);
  //const int n_tick_tot = n_rank(comm)-1;

// 0. initial partitioning
  interval_vector<int> partition_indices = pivot_partition_once(x,comm,comp,std::move(partition_is));
//  auto sub_ins = search_intervals3(partition_indices,max_interval_tick_shift,comm);
//  //for (int i=0; i<(int)sub_ins.size(); ++i) {
//  //  for (int k=0; k<sub_ins[i].n_ticks; ++k) {
//  //    SLOG(comm,sub_ins[i].objective_tick(k));
//  //  }
//  //}
//
//// 1. loop until there is no sub-interval to partition (that is, until all partition_indices are OK)
//  int kk = 0;
//  while (sub_ins.size()>0) {
//    //if (rank(comm)==0) {
//    //  int n_tick_max = 0;
//    //  for (int i=0; i<sub_ins.size(); ++i) {
//    //    n_tick_max = std::max(n_tick_max, sub_ins[i].n_ticks);
//    //  }
//    //  ELOG(kk);
//    //  ELOG(sub_ins.size());
//    //  ELOG(n_tick_max);
//    //}
//    //SLOG(comm,kk);
//    if (kk++>128) {
//      // Note: there should be approximately log(sz_tot) loop iteration
//      // If this is not the case, it could be due to
//      //   - worst-case complexity scenario:
//      //        => for very specific inputs, the pivot selection is defective (should be as rare as median-of-3 quicksort pathological cases)
//      //   - implementation bugs
//      //        => in particular, special cases where the array is too small to get enough samples
//      throw par_algo_exception("pivot_partition_eq: max number of iteration reached");
//    }
//
//    int n_sub_intervals = sub_ins.size();
//
//    // 0. prepare for sampling
//    std::vector<std_e::span<T>> x_sub(n_sub_intervals);
//    for (int i=0; i<n_sub_intervals; ++i) {
//      T* start  = x.data() + sub_ins[i].inf;
//      T* finish = x.data() + sub_ins[i].sup;
//      x_sub[i] = make_span(start,finish);
//    }
//
//    // 1. compute pivots
//    std::vector<int> n_indices(n_sub_intervals);
//    for (int i=0; i<n_sub_intervals; ++i) {
//      n_indices[i] = sub_ins[i].n_ticks;
//    }
//    //LOG("pivot_partition balance");
//    std::vector<std::vector<T>> pivots_by_sub_intervals = median_of_3_sample(x_sub,n_indices,comm);
//
//    // 1. partition sub-intervals, report results in partition_indices, and compute new sub-intervals
//    std::vector<interval_to_partition> new_sub_ins;
//    //SLOG(comm,n_sub_intervals);
//    for (int i=0; i<n_sub_intervals; ++i) {
//      //SLOG(comm,i);
//      // 1.0. partition sub-intervals,
//      const std::vector<T>& pivots = pivots_by_sub_intervals[i];
//      auto partition_indices_sub = pivot_partition_indices(x_sub[i],pivots,comp,Return_container{});
//
//      // 1.1. compute new sub-intervals
//      //SLOG(comm,partition_indices_sub);
//      auto partition_indices_sub_tot = all_reduce(partition_indices_sub.as_base(),MPI_SUM,comm); // TODO outside of loop
//      //SLOG(comm,partition_indices_sub_tot);
//
//      std::vector<int> objective_ticks_sub(sub_ins[i].n_ticks);
//
//      //SLOG(comm,sub_ins[i].sz_tot);
//      //SLOG(comm,sub_ins[i].n_interval);
//      //SLOG(comm,sub_ins[i].position);
//      //SLOG(comm,sub_ins[i].inf_tot);
//      //SLOG(comm,sub_ins[i].objective_tick(0));
//      for (int j=0; j<sub_ins[i].n_ticks; ++j) {
//        objective_ticks_sub[j] = sub_ins[i].objective_tick(j) - sub_ins[i].inf_tot;
//      }
//      //SLOG(comm,objective_ticks_sub);
//      auto [first_indices, n_indices, interval_start, index_ticks_found] = search_intervals4(partition_indices_sub_tot,objective_ticks_sub,max_interval_tick_shift);
//      //SLOG(comm,first_indices);
//      //SLOG(comm,n_indices);
//      //SLOG(comm,interval_start);
//      int n_sub_sub_intervals = interval_start.size();
//
//      // 1.2. create new sub-intervals
//      for (int j=0; j<n_sub_sub_intervals; ++j) {
//        int inf     = sub_ins[i].inf     + partition_indices_sub    [interval_start[j]];
//        int sup     = sub_ins[i].inf     + partition_indices_sub    [interval_start[j] + 1];
//        int inf_tot = sub_ins[i].inf_tot + partition_indices_sub_tot[interval_start[j]];
//        int sup_tot = sub_ins[i].inf_tot + partition_indices_sub_tot[interval_start[j] + 1];
//        //SLOG(comm,partition_indices_sub_tot[interval_start[j]]   );
//        //SLOG(comm,partition_indices_sub_tot[interval_start[j] +1]);
//        //if (rank(comm)==0) {
//        //  ELOG(inf_tot);
//        //  ELOG(sup_tot);
//        //}
//        new_sub_ins.push_back( {inf,sup,n_indices[j],sub_ins[i].position+first_indices[j], inf_tot,sup_tot,sz_tot,n_rk} );
//      }
//
//      // 1.3. report found ticks
//      for (int j=0; j<(int)index_ticks_found.size(); ++j) {
//        int k = index_ticks_found[j];
//        int abs_pos = sub_ins[i].position+k-1;
//        partition_indices[abs_pos] = sub_ins[i].inf + partition_indices_sub[k];
//        //kSLOG(comm,partition_indices[abs_pos]);
//      }
//    }
//
//    // 2. loop
//    sub_ins = std::move(new_sub_ins);
//  }
//  //if (rank(comm)==0) ELOG(kk);

  return partition_indices;
}


template<
  class T,
  class Comp = std::less<>,
  class Return_container = interval_vector<int>
> auto
pivot_partition_eq(std::vector<T>& x, MPI_Comm comm, Comp comp = {}, Return_container&& partition_is = {}) -> Return_container {
  size_t sz_tot = all_reduce(x.size(),MPI_SUM,comm);
  return pivot_partition_eq(x,sz_tot,comm,comp,std::move(partition_is));
}

} // std_e
