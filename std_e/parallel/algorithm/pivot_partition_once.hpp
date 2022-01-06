#pragma once


#include "std_e/parallel/mpi/collective/gather.hpp"
#include "std_e/algorithm/distribution/uniform_sample.hpp"


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
  size_t sz = x.size();
  size_t n_pivot = n_rank(comm)-1; // to get `n_rank` partitions, we need `n_rank-1` partition points

  size_t sz_tot = all_reduce(x.size(),MPI_SUM,comm);
  STD_E_ASSERT(sz_tot > n_pivot); // can't get n_pivot samples if less than n_pivot values

  // TODO replace by non-balanced sampling (here, works if x.size() is constant)
  // 0. local samples
  std::vector<T> local;
  if (sz <= 3) {
    local = x;
  } else {
    local = {x[sz/6] , x[sz/2], x[(5*sz)/6]}; // if `sz` is the same over all ranks, this gives a uniform sampling
  }

  // 1. gather all samples and sort them
  std::vector<T> sample = all_gather(local,comm); // TODO could be optimized because can be pre-allocated at 3*n_rank
  std::sort(begin(sample),end(sample));

  // 2. re-sample to get `n_pivot` values
  return uniform_sample(sample,n_pivot);
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
