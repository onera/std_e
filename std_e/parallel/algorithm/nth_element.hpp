#pragma once


#include "std_e/parallel/all_to_all.hpp"
#include "std_e/algorithm/partition_sort.hpp"
#include "std_e/algorithm/distribution.hpp"
#include "std_e/parallel/struct/distribution.hpp"
#include "std_e/log.hpp"


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
number_of_ticks_in_interval(int n_rank, int n_sample, int rank) {
  double n = n_rank;
  double k = n_sample;
  double j = rank;
  int n_ticks = floor( (k+1)*(j+1)/n ) - ceil( (k+1)*j/n ) + 1;
  return n_ticks;
}

auto
median_of_3_sample(span<T> x, int n, MPI_Comm comm) {
  int rk = rank(comm);
  int n_rk = n_rank(comm);
  int n_sample = 3*n; // *3 by analogy with median of 3

  int n_ticks = number_of_ticks_in_interval(n_sample,n_rk,rk);

  int sz = s.size();
  std::vector<T> local(n_ticks);
  for (int i=0; i<n_ticks; ++i) {
    local[i] = x[ i*sz/(n_ticks+1) ];
  }

  std::vector<T> sample = all_gather(local,comm); // could be optimized because can be pre-allocated at n_sample
  std::sort(begin(sample),end(sample));
  if (int(sample.size())!=n_sample) { ELOG(n_sample); ELOG(sample.size()); }

  std::vector<T> pivots(n);
  for (int i=0; i<n; ++i) {
    pivots[i] = sample[3*i+1];
  }
  return pivots;

}
auto
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

template<class T, class Comp> auto
partition_sort_minimize_imbalance(std::vector<T>& x, int sz_tot, MPI_Comm comm, double max_imbalance) -> std::vector<T> {
  const int n_rk = n_rank(comm);
  const int max_partition_shift = (max_imbalance/2.) * double(sz_tot)/double(n_rk);
  std::vector<T> pivots = median_of_3_sample(x,comm);

  auto partition_indices = partition_sort_indices(x,pivots);
  /// partition_indices[0]=0 (post condition of partition_sort_indices)
  /// but we need to impose last the index
  partition_indices.push_back( x.size() );

  while (true) {
    auto partition_indices_tot = all_reduce(partition_indices,MPI_SUM,comm);
    auto [first_indices, n_indices, interval_start] = search_intervals(partition_indices_tot);
    int n_sub_intervals = first_indices.size();
    if (n_sub_intervals==0) break;

    std::vector<std_e::span<T>> x_sub(n_sub_intervals);
    for (int i=0; i<n_sub_intervals; ++i) {
      T* start  = x.data() + partition_indices[interval_start[i]];
      T* finish = x.data() + partition_indices[interval_start[i+1]];
      x_sub[i] = make_span(start,finish);
    }
    std::vector<std::vector<T>> pivots_by_sub_intervals = median_of_3_sample(x_sub,n_indices,comm);
    for (int i=0; i<n_sub_intervals; ++i) {
      const std::vector<T>& pivots = pivots_by_sub_intervals[i];
      auto partition_indices_sub = partition_sort_indices(x_sub[i],pivots);

      int first_index = first_indices[i];
      int n_index = n_indices[i];
      STD_E_ASSERT(partition_indices.size()-2==n_index);
      for (int k=0; k<n_index; ++k) {
        int offset = partition_indices[interval_start[i]];
        partition_indices[first_index+k] = offset+partition_indices_sub[k+1];
      }
    }
  }

  return partition_indices;
}
