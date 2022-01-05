#pragma once


#include "std_e/parallel/all_to_all.hpp"
#include "std_e/algorithm/pivot_partition_range.hpp"
#include "std_e/algorithm/distribution.hpp"
#include "std_e/parallel/struct/distribution.hpp"
#include "std_e/parallel/algorithm/search_intervals.hpp"
#include "std_e/parallel/algorithm/pivot_partition_once.hpp"
#include "std_e/base/msg_exception.hpp"
#include "std_e/plog.hpp"
#include <cmath>


namespace std_e {


auto
number_of_ticks_in_interval(int n_buckets, int n_ticks, int i_bucket) -> int {
  STD_E_ASSERT(0<=i_bucket && i_bucket<n_buckets);
  int n = n_buckets;
  int k = n_ticks;
  int i = i_bucket;
  // We can view the problem as the following:
  //    - We take the interval [0,(k+1)n)
  //    - for i in [0,n) sub-interval i is [(k+1)i , (k+1)(i+1))
  //    - for j in [1,k+1), tick j is j*n
  // Then, given n,k,i, we want to compute the number of ticks within sub-interval i
  // That is, the number of integer j for which (k+1)i <= j*n < (k+1)(i+1)
  int j_min = (k+1)*i / n; // ensures j_min is the inf or just before the inf of interval i
  if ( ((k+1)*i > j_min*n) || j_min==0 ) { // if strictly before the inf, or j not in [1,k+1), take next
    ++j_min;
  }
  int j_max = (k+1)*(i+1) / n; // ensures j_max is the sup or just before the sup of interval i
  if ( (k+1)*(i+1) == j_max*n ) { // if exactly the sup, this is not okay since we are open, so take previous
    --j_max;
  }
  if ((j_max-j_min) < 0) return 0;
  else return j_max-j_min + 1;
}

// TODO test
// TODO move near algorithm/distribution.hpp
// TODO clean
auto
ticks_in_interval(int start, int finish, int size, int n_ticks) -> std::vector<int> {
  int spacing_round_down = size/(n_ticks+1);
  int spacing_round_up = spacing_round_down+1;
  int n_round_up = size%(n_ticks+1);

  std::vector<int> ticks;
  if (start < n_round_up*spacing_round_up) { // if only spacing_round_up steps to get to start
    int i_tick = start/spacing_round_up;
    int n_round_up_used = i_tick;
    int first_tick = i_tick*spacing_round_up;
    if (first_tick < start || first_tick==0) { // if first_tick not in interval, take next. exclude 0
      first_tick += spacing_round_up;
      ++n_round_up_used;
    }

    int n_round_up_remaining = n_round_up - n_round_up_used;
    while (first_tick < finish && n_round_up_remaining>0) {
      ticks.push_back(first_tick);
      first_tick += spacing_round_up;
      --n_round_up_remaining;
    }
    while (first_tick < finish) {
      ticks.push_back(first_tick);
      first_tick += spacing_round_down;
    }
  } else {
    int start0 = start - n_round_up*spacing_round_up;
    int i_tick0 = start0/spacing_round_down;
    int first_tick = n_round_up*spacing_round_up + i_tick0*spacing_round_down;
    if (first_tick < start || first_tick==0) first_tick += spacing_round_down;

    while (first_tick < finish) {
      ticks.push_back(first_tick);
      first_tick += spacing_round_down;
    }
  }
  return ticks;
}

template<class T> auto
median_of_3_sample(span<T> x, int n, MPI_Comm comm) {
  //int rk = rank(comm);
  //int n_rk = n_rank(comm);
  int n_sample = 3*n; // *3 by analogy with median of 3

  int x_start_glob = ex_scan(x.size(),MPI_SUM,0,comm);
  int x_size_glob = all_reduce(x.size(),MPI_SUM,comm);

  //SLOG(comm,x_start_glob);
  //SLOG(comm,x_start_glob+x.size());
  //SLOG(comm,x_size_glob);
  std::vector<int> sample_indices = ticks_in_interval(x_start_glob,x_start_glob+x.size(),x_size_glob,n_sample);
  //SLOG(comm,sample_indices);
  int n_sample_local = sample_indices.size();

  std::vector<T> local(n_sample_local);
  for (int i=0; i<n_sample_local; ++i) {
    local[i] = x[ sample_indices[i]-x_start_glob ];
  }
  //ELOG(local);

  std::vector<T> sample = all_gather(local,comm); // could be optimized because can be pre-allocated at n_sample
  std::sort(begin(sample),end(sample));
  //ELOG(sample.size());
  //ELOG(n_sample);
  STD_E_ASSERT(int(sample.size())==n_sample);

  std::vector<T> pivots(n);
  for (int i=0; i<n; ++i) {
    pivots[i] = sample[3*i+1];
  }
  return pivots;

}
template<class T> auto
median_of_3_sample(std::vector<span<T>>& xs, std::vector<int> ns, MPI_Comm comm) {
  // TODO implement with only one gather
  STD_E_ASSERT(xs.size()==ns.size());
  int n_interval = xs.size();
  //int rk = rank(comm);
  //int n_rk = n_rank(comm);

  std::vector<std::vector<T>> pivots_by_sub_intervals(n_interval);
  for (int i=0; i<n_interval; ++i) {
    pivots_by_sub_intervals[i] = median_of_3_sample(xs[i],ns[i],comm);
  }
  return pivots_by_sub_intervals;

  //std::vector<int> ranks_with_sample;
  //std::vector<int> n_sample_by_rank;
  //std::vector<int> sub_interval_index;
  //std::vector<int> sample_indices;

  //for (int i=0; i<n_interval; ++i) {
  //  int n_sample = ns[i] * 3; // *3 by analogy with median of 3

  //}
}


struct interval_to_partition {
  int inf;
  int sup;
  int n_ticks;
  int position;

  int inf_tot;
  int sup_tot;

  int sz_tot;
  int n_interval;
  auto objective_tick(int i_tick) const {
    STD_E_ASSERT(i_tick<n_ticks);
    return uniform_partition_point(sz_tot,n_interval,position+i_tick);
  }

};
inline auto
to_string(const interval_to_partition& x) {
  return '['+std::to_string(x.inf)+','+std::to_string(x.sup)+"):"+std::to_string(x.n_ticks)+':'+std::to_string(x.position);
}


auto
search_intervals3(const interval_vector<int>& partition_indices, int max_interval_tick_shift, MPI_Comm comm) {
  auto partition_indices_tot = all_reduce(partition_indices.as_base(),MPI_SUM,comm);

  //SLOG(comm,partition_indices_tot);
  int n_interval = n_rank(comm);
  int sz_tot = partition_indices_tot.back();

  auto [first_indices, n_indices, interval_start] = search_intervals(partition_indices_tot,max_interval_tick_shift);

  int n_sub_intervals = interval_start.size();
  std::vector<interval_to_partition> sub_ins(n_sub_intervals);
  for (int i=0; i<n_sub_intervals; ++i) {
    int inf = partition_indices[interval_start[i]];
    int sup = partition_indices[interval_start[i] + 1];
    int inf_tot = partition_indices_tot[interval_start[i]];
    int sup_tot = partition_indices_tot[interval_start[i] + 1];
    sub_ins[i] = {inf,sup,n_indices[i],first_indices[i], inf_tot,sup_tot,sz_tot,n_interval};
  }
  return sub_ins;
}


class par_algo_exception : msg_exception {
  using base = msg_exception;
  using base::base;
};

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
pivot_partition_minimize_imbalance(
  std::vector<T>& x, int sz_tot, MPI_Comm comm, double max_imbalance = 0.05, Comp comp = {}, Return_container&& partition_is = {}) -> interval_vector<int> {
  const int n_rk = n_rank(comm);
  const int max_interval_tick_shift = (max_imbalance/2.) * double(sz_tot)/double(n_rk);
  //const int n_tick_tot = n_rank(comm)-1;

// 0. initial partitioning
  interval_vector<int> partition_indices = pivot_partition_once(x,comm,comp,std::move(partition_is));
  auto sub_ins = search_intervals3(partition_indices,max_interval_tick_shift,comm);
  //for (int i=0; i<(int)sub_ins.size(); ++i) {
  //  for (int k=0; k<sub_ins[i].n_ticks; ++k) {
  //    SLOG(comm,sub_ins[i].objective_tick(k));
  //  }
  //}

// 1. loop until there is no sub-interval to partition (that is, until all partition_indices are OK)
  int kk = 0;
  while (sub_ins.size()>0) {
    //if (rank(comm)==0) {
    //  int n_tick_max = 0;
    //  for (int i=0; i<sub_ins.size(); ++i) {
    //    n_tick_max = std::max(n_tick_max, sub_ins[i].n_ticks);
    //  }
    //  ELOG(kk);
    //  ELOG(sub_ins.size());
    //  ELOG(n_tick_max);
    //}
    //SLOG(comm,kk);
    if (kk++>128) {
      // Note: there should be approximately log(sz_tot) loop iteration
      // If this is not the case, it could be due to
      //   - worst-case complexity scenario:
      //        => for very specific inputs, the pivot selection is defective (should be as rare as median-of-3 quicksort pathological cases)
      //   - implementation bugs
      //        => in particular, special cases where the array is too small to get enough samples
      throw par_algo_exception("pivot_partition_minimize_imbalance: max number of iteration reached");
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
    std::vector<int> n_indices(n_sub_intervals);
    for (int i=0; i<n_sub_intervals; ++i) {
      n_indices[i] = sub_ins[i].n_ticks;
    }
    //LOG("pivot_partition balance");
    std::vector<std::vector<T>> pivots_by_sub_intervals = median_of_3_sample(x_sub,n_indices,comm);

    // 1. partition sub-intervals, report results in partition_indices, and compute new sub-intervals
    std::vector<interval_to_partition> new_sub_ins;
    //SLOG(comm,n_sub_intervals);
    for (int i=0; i<n_sub_intervals; ++i) {
      //SLOG(comm,i);
      // 1.0. partition sub-intervals,
      const std::vector<T>& pivots = pivots_by_sub_intervals[i];
      auto partition_indices_sub = pivot_partition_indices(x_sub[i],pivots,comp,Return_container{});

      // 1.1. compute new sub-intervals
      //SLOG(comm,partition_indices_sub);
      auto partition_indices_sub_tot = all_reduce(partition_indices_sub.as_base(),MPI_SUM,comm); // TODO outside of loop
      //SLOG(comm,partition_indices_sub_tot);

      std::vector<int> objective_ticks_sub(sub_ins[i].n_ticks);

      //SLOG(comm,sub_ins[i].sz_tot);
      //SLOG(comm,sub_ins[i].n_interval);
      //SLOG(comm,sub_ins[i].position);
      //SLOG(comm,sub_ins[i].inf_tot);
      //SLOG(comm,sub_ins[i].objective_tick(0));
      for (int j=0; j<sub_ins[i].n_ticks; ++j) {
        objective_ticks_sub[j] = sub_ins[i].objective_tick(j) - sub_ins[i].inf_tot;
      }
      //SLOG(comm,objective_ticks_sub);
      auto [first_indices, n_indices, interval_start, index_ticks_found] = search_intervals4(partition_indices_sub_tot,objective_ticks_sub,max_interval_tick_shift);
      //SLOG(comm,first_indices);
      //SLOG(comm,n_indices);
      //SLOG(comm,interval_start);
      int n_sub_sub_intervals = interval_start.size();

      // 1.2. create new sub-intervals
      for (int j=0; j<n_sub_sub_intervals; ++j) {
        int inf     = sub_ins[i].inf     + partition_indices_sub    [interval_start[j]];
        int sup     = sub_ins[i].inf     + partition_indices_sub    [interval_start[j] + 1];
        int inf_tot = sub_ins[i].inf_tot + partition_indices_sub_tot[interval_start[j]];
        int sup_tot = sub_ins[i].inf_tot + partition_indices_sub_tot[interval_start[j] + 1];
        //SLOG(comm,partition_indices_sub_tot[interval_start[j]]   );
        //SLOG(comm,partition_indices_sub_tot[interval_start[j] +1]);
        //if (rank(comm)==0) {
        //  ELOG(inf_tot);
        //  ELOG(sup_tot);
        //}
        new_sub_ins.push_back( {inf,sup,n_indices[j],sub_ins[i].position+first_indices[j], inf_tot,sup_tot,sz_tot,n_rk} );
      }

      // 1.3. report found ticks
      for (int j=0; j<(int)index_ticks_found.size(); ++j) {
        int k = index_ticks_found[j];
        int abs_pos = sub_ins[i].position+k-1;
        partition_indices[abs_pos] = sub_ins[i].inf + partition_indices_sub[k];
        //kSLOG(comm,partition_indices[abs_pos]);
      }
    }

    // 2. loop
    sub_ins = std::move(new_sub_ins);
  }
  //if (rank(comm)==0) ELOG(kk);

  return partition_indices;
}


} // std_e
