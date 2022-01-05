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

//template<class I>
//sample_position(I n, I n_sample, I i) -> I {
//  // |--------------------------|
//  //
//  I step_position = (i*n_sample)/n_pivot; // position of begining of interval i
//  I shift_to_mid = (n_sample/n_pivot) / 2; // shift to get middle positions
//  return step_position + shift_to_mid;
//}


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
  STD_E_ASSERT(sz > 0);

  // 0. local samples
  std::vector<T> local = {x[sz/6] , x[sz/2], x[(5*sz)/6]}; // if `sz` is the same over all ranks, this gives a uniform sampling

  // 1. gather all samples and sort them
  std::vector<T> sample = all_gather(local,comm); // TODO could be optimized because can be pre-allocated at 3*n_rank
  std::sort(begin(sample),end(sample));
  ELOG(sample);
  int n_sample = sample.size();

  // 2. re-sample to get n_pivot values
  int n_pivot = n_rank(comm)-1; // n_rank-1 partition points give n_rank partitions
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
  class Proj = identity_closure,
  class Return_container = interval_vector<int>
> auto
pivot_partition_once(std::vector<T>& x, MPI_Comm comm, Comp comp = {}, Proj proj = {}, Return_container&& partition_is = {}) -> Return_container {
  STD_E_ASSERT(x.size() > 0);

  std::vector<T> pivots = median_of_3_sample(x,comm);

  return pivot_partition_indices(x,pivots,comp,std::move(partition_is));
}


} // std_e
