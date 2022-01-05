#pragma once


#include "std_e/parallel/all_to_all.hpp"
#include "std_e/algorithm/pivot_partition_range.hpp"
#include "std_e/algorithm/distribution.hpp"
#include "std_e/parallel/struct/distribution.hpp"
#include "std_e/parallel/algorithm/search_intervals.hpp"
#include "std_e/base/msg_exception.hpp"
#include "std_e/plog.hpp"
#include <cmath>

// TODO move



namespace std_e {


// The performance of the quicksort algorithm depends on the choice of a good pivot:
//   - the optimal pivot would be the median but it is too long to find
//   - taking the first element, or last element of the range is the worst choice if the range is already sorted
//   - one can sample the range and find the median from the sample
//     - one simple sample is to take the "median of 3": the median of the first, last and middle elements
//        it is effective in practice (used by std::sort)
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
  //// Prop 0 with extrema
  //  // we want to sample equally spaced values over the whole array,
  //  // if all local sz where equal,
  //  //   - it means at positions sz/6, sz/2 and 5*sz/6
  //  //   - we also take the first and last values of the whole array
  //  if (rk==0)      local = {x[0] , x[sz/6] , x[sz/2], x[(5*sz)/6]          };
  //  if (rk==n_rk-1) local = {       x[sz/6] , x[sz/2], x[(5*sz)/6], x.back()};
  //  else            local = {       x[sz/6] , x[sz/2], x[(5*sz)/6]          };
  // Prop 1 with extrema
    local = {x[sz/6] , x[sz/2], x[(5*sz)/6]};
  }
  std::vector<T> sample = all_gather(local,comm); // could be optimized because can be pre-allocated at 3*n_rk
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


template<
  class T,
  class Comp = std::less<>,
  class Return_container = interval_vector<int>
> auto
pivot_partition_once(std::vector<T>& x, MPI_Comm comm, Comp comp = {}, Return_container&& partition_is = {}) -> Return_container {
  //LOG("pivot_partition_once");
  std::vector<T> pivots = median_of_3_sample(x,comm);

  return pivot_partition_indices(x,pivots,comp,std::move(partition_is));
}


} // std_e
