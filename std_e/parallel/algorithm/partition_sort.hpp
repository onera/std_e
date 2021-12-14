#pragma once


#include "std_e/parallel/all_to_all.hpp"
#include "std_e/algorithm/partition_sort.hpp"
#include "std_e/algorithm/distribution.hpp"
#include "std_e/parallel/struct/distribution.hpp"
#include "std_e/parallel/algorithm/search_intervals.hpp"
#include "std_e/log.hpp"
#include <cmath>


namespace std_e {


// The performance of the quicksort algorithm depends on the choice of a good pivot:
//   - the optimal pivot would be the median but it is too long to find
//   - taking the first element, or last element of the range is the worst choice if the range is already sorted
//   - one can sample the range and find the median from the sample
//     - one simple sample is to take the "median of 3": the median of the first, last and middle elements
//        it is effective in practice (used for std::sort before introsort was discovered)
// Here we want to use a similar idea, but we need k quantiles instead of only the median
// The idea is then to take 3*k equally (or almost equally) separated samples, and take the quantiles out of them
// For k=1, we get back the "median of 3" selection
template<class T> auto
median_of_3_sample(std::vector<T>& x, MPI_Comm comm) {
  auto sz = x.size();
  int rk = rank(comm);
  int n_rk = n_rank(comm);

  std::vector<T> local;

  // 0. if x is not empty, sample 3 values out of it
  if (sz>0) {
    local = std::vector<T>(3);
  // Prop 0 with extrema
    // we want to sample equally spaced values over the whole array,
    // if all local sz where equal,
    //   - it means at positions sz/6, sz/2 and 5*sz/6
    //   - we also take the first and last values of the whole array
    if (rk==0)      local = {x[0] , x[sz/6] , x[sz/2], x[(5*sz)/6]          };
    if (rk==n_rk-1) local = {       x[sz/6] , x[sz/2], x[(5*sz)/6], x.back()};
    else            local = {       x[sz/6] , x[sz/2], x[(5*sz)/6]          };
  // Prop 1 with extrema
    local = {x[sz/6] , x[sz/2], x[(5*sz)/6]};
  }
  std::vector<T> sample = all_gather(local,comm); // could be optimized because can be pre-allocated at 3*n_rk+2
  std::sort(begin(sample),end(sample));
  int n_sample = sample.size();

  int n_pivot = n_rk-1; // -1 because n_rk-1 partition points give n_rk partitions
  std::vector<T> pivots(n_pivot);
  for (int i=0; i<n_pivot; ++i) {
    int scaled_mid = (i*n_sample)/n_pivot + n_sample/(2*n_pivot); // spread over the sample, then shift to get middle positions
    pivots[i] = sample[scaled_mid]; // TODO check no out-of-bounds
  }
  return pivots;

}

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

//template<class I> auto
//integer_division(I m, I n) {j
//  return std::make_pair( m/n , m%n );
//}

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
  int rk = rank(comm);
  int n_rk = n_rank(comm);
  int n_sample = 3*n; // *3 by analogy with median of 3

  int x_start_glob = ex_scan(x.size(),MPI_SUM,0,comm);
  int x_size_glob = all_reduce(x.size(),MPI_SUM,comm);

  //ELOG(x_start_glob);
  std::vector<int> sample_indices = ticks_in_interval(x_start_glob,x_start_glob+x.size(),x_size_glob,n_sample);
  //ELOG(sample_indices);
  int n_sample_local = sample_indices.size();

  std::vector<T> local(n_sample_local);
  for (int i=0; i<n_sample_local; ++i) {
    local[i] = x[ sample_indices[i]-x_start_glob ];
  }

  std::vector<T> sample = all_gather(local,comm); // could be optimized because can be pre-allocated at n_sample
  std::sort(begin(sample),end(sample));
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
  int rk = rank(comm);
  int n_rk = n_rank(comm);

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

template<class T> auto
partition_sort_once(std::vector<T>& x, MPI_Comm comm) -> interval_vector<int> {
  std::vector<T> pivots = median_of_3_sample(x,comm);

  auto partition_indices = partition_sort_indices(x,pivots);
  /// partition_indices[0]=0 (post condition of partition_sort_indices)
  /// but we need to impose last the index
  partition_indices.push_back( x.size() );

  return partition_indices;
}



struct interval_to_partition {
  int inf;
  int sup;
  int n_ticks;
  int position;
};
inline auto
to_string(const interval_to_partition& x) {
  return '['+std::to_string(x.inf)+','+std::to_string(x.sup)+"):"+std::to_string(x.n_ticks)+':'+std::to_string(x.position);
}

template<class T> auto
partition_sort_minimize_imbalance(std::vector<T>& x, int sz_tot, MPI_Comm comm, double max_imbalance) -> interval_vector<int> {
  const int n_rk = n_rank(comm);
  const int max_interval_tick_shift = (max_imbalance/2.) * double(sz_tot)/double(n_rk);

  interval_vector<int> partition_indices = partition_sort_once(x,comm);

  auto partition_indices_tot = all_reduce(partition_indices.as_base(),MPI_SUM,comm);
  auto [first_indices, n_indices, interval_start] = search_intervals(partition_indices_tot,max_interval_tick_shift);
  int n_sub_intervals = first_indices.size();
  if (n_sub_intervals==0) return partition_indices;

  std::vector<interval_to_partition> sub_ins(n_sub_intervals);
  for (int i=0; i<n_sub_intervals; ++i) {
    int inf = partition_indices[interval_start[i]];
    int sup = partition_indices[interval_start[i] + 1];
    sub_ins[i] = {inf,sup,n_indices[i],interval_start[i]};
  }

  int kk = 0;
  while (sub_ins.size()>0) {
    if (rank(comm)==0) ELOG(kk);
    if (kk++>10) break;

    ELOG(sub_ins);
    int n_sub_intervals = sub_ins.size();

    // 0. prepare for sampling
    std::vector<std_e::span<T>> x_sub(n_sub_intervals);
    for (int i=0; i<n_sub_intervals; ++i) {
      //ELOG(partition_indices[interval_start[i]]);
      //ELOG(partition_indices[interval_start[i+1]]);
      T* start  = x.data() + sub_ins[i].inf;
      T* finish = x.data() + sub_ins[i].sup;
      x_sub[i] = make_span(start,finish);
    }

    //std::vector<double> xx(x_sub[0].begin(),x_sub[0].end());
    //std::sort(begin(xx),end(xx));
    //ELOG(xx);
    std::vector<int> n_indices(n_sub_intervals);
    for (int i=0; i<n_sub_intervals; ++i) {
      n_indices[i] = sub_ins[i].n_ticks;
    }
    std::vector<std::vector<T>> pivots_by_sub_intervals = median_of_3_sample(x_sub,n_indices,comm);
    //ELOG(pivots_by_sub_intervals);

    // 1. partition sub-intervals, report results in partition_indices, and compute new sub-intervals
    std::vector<interval_to_partition> new_sub_ins;
    for (int i=0; i<n_sub_intervals; ++i) {
      // 1.0. partition sub-intervals,
      const std::vector<T>& pivots = pivots_by_sub_intervals[i];
      auto partition_indices_sub = partition_sort_indices(x_sub[i],pivots);
      ELOG(partition_indices_sub);

      // 1.1. report results
      int first_index = sub_ins[i].inf;
      int n_index = sub_ins[i].n_ticks;
      STD_E_ASSERT(partition_indices_sub.size()-1==n_index);
      for (int k=0; k<n_index; ++k) {
        int offset = partition_indices[sub_ins[i].position];
        partition_indices[first_index+k] = offset+partition_indices_sub[k+1];
      }

      // 1.2. compute new sub-intervals
      auto partition_indices_tot_sub = all_reduce(partition_indices_sub.as_base(),MPI_SUM,comm); // TODO outside of loop
      ELOG(partition_indices_tot_sub);

      auto [first_indices, n_indices, interval_start] = search_intervals(partition_indices_tot,max_interval_tick_shift);
      int n_sub_sub_intervals = first_indices.size();
      ELOG(first_indices);
      ELOG(n_indices);
      ELOG(interval_start);

      for (int j=0; j<n_sub_sub_intervals; ++j) {
        int inf = partition_indices_sub[interval_start[j]];
        int sup = partition_indices_sub[interval_start[j] + 1];
        new_sub_ins.push_back(  {inf,sup,n_indices[j],sub_ins[i].position+interval_start[j]} );
      }
    }

    // 2. loop
    sub_ins = std::move(new_sub_ins);
  }

  return partition_indices;
}


} // std_e
