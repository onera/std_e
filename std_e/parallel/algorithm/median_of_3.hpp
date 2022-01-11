#pragma once


#include "std_e/interval/interval_sequence.hpp"
#include "std_e/parallel/algorithm/uniform_sample.hpp"
#include "std_e/algorithm/distribution/uniform_sample.hpp"
#include "std_e/parallel/mpi/collective/gather.hpp"
#include "std_e/log.hpp"


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


template<class RA_rng, class I = typename RA_rng::size_type, class T = typename RA_rng::value_type> auto
median_of_3_sample(const RA_rng& x, I n_pivot, MPI_Comm comm) {
  // 0. n_sample
  I size_tot = all_reduce(x.size(),MPI_SUM,comm);
  STD_E_ASSERT(size_tot >= n_pivot);

  I n_sample = std::min(size_tot,3*n_pivot); // 3*n_pivot by analogy with median of 3 (if n_pivot==1, we have a median of 3)

  // 1. local samples
  std::vector<T> sample_local = uniform_sample_local(x,n_sample,comm);

  // 2. gather all samples and sort them
  std::vector<T> sample = all_gather(sample_local,comm); // could be optimized because can be pre-allocated at n_sample
  STD_E_ASSERT(I(sample.size())==n_sample);
  std::sort(begin(sample),end(sample));
  // TODO std::unique + handle if not enought samples

  // 3. re-sample to get `n_pivot` values
  return uniform_sample_exclude_ends(sample,n_pivot);
}


// Sample all ranks equally regardless of their size
// Avoid the start and end (because the end of one rank is the element just after the start of the next rank)
// TODO the problem with the algorithm is that it takes 3*n_rank values
//    - it can't be used in a context where n_pivot>3*n_rank
//    - more importantly, it may be too much samples (worst case: pivot_partition_eq may have an iteration with n_rank/2 interval -> 3/2*n_rank^2 samples!
//  -> Solution: n_pivot gives which ranks to hit, and how many times, but then we take values in the rank similar to this (excluding begin/end)
template<class RA_rng, class I = typename RA_rng::size_type, class T = typename RA_rng::value_type> auto
median_of_3_sample_mod(const RA_rng& x, I n_pivot, MPI_Comm comm) {
  // 0. n_sample
  I size_tot = all_reduce(x.size(),MPI_SUM,comm);
  STD_E_ASSERT(size_tot >= n_pivot);

  // 1. local samples
  auto sz = x.size();
  std::vector<T> sample_local;
  if (sz==1) {
    sample_local = std::vector{x[0]};
  }
  if (sz==2) {
    sample_local = std::vector{x[0],x[1]};
  }
  if (sz>=3) {
    // if we view the global array as the concatenation of `x` accross the ranks
    // and if we suppose that `x.size()` is the same accross the ranks,
    // then this sampling is uniform and avoids the ends
    sample_local = {x[sz/6],x[sz/2],x[5*sz/6]};

    // worse choices :D
    // these two trigger a linear sub-interval reduction (instead of exponential)
    //sample_local = {x[0]};
    //sample_local = {x[1]};
    //sample_local = {x[sz/2]};
    //sample_local = {x[sz/4],x[3*sz/4]};
    //sample_local = {x[0],x[sz/6],x[sz/3],x[sz/2],x[2*sz/3],x[5*sz/6]};
  }

  // 2. gather all samples and sort them
  std::vector<T> sample = all_gather(sample_local,comm); // could be optimized because can be pre-allocated at n_sample
  std::sort(begin(sample),end(sample));
  // TODO std::unique + handle if not enought samples

  // 3. re-sample to get `n_pivot` values
  return uniform_sample_exclude_ends(sample,n_pivot);
}


//constexpr auto median_of_3_sample_fn     = [](const auto& x, auto n_pivot, MPI_Comm comm){ return median_of_3_sample    (x,n_pivot,comm); };
//constexpr auto median_of_3_sample_mod_fn = [](const auto& x, auto n_pivot, MPI_Comm comm){ return median_of_3_sample_mod(x,n_pivot,comm); };


template<class T, class I> auto
find_pivots(std::vector<span<T>>& xs, std::vector<I> ns, MPI_Comm comm) {
  // TODO implement with only one gather:
  // worst case here: pivot_partition_eq may have an iteration with n_rank/2 interval, then we have n_rank MPI calls -> merge them into one!
  STD_E_ASSERT(xs.size()==ns.size());
  int n_interval = xs.size();

  std::vector<std::vector<T>> pivots_by_sub_intervals(n_interval);
  for (int i=0; i<n_interval; ++i) {
    pivots_by_sub_intervals[i] = median_of_3_sample_mod(xs[i],ns[i],comm);
  }
  return pivots_by_sub_intervals;
}


template<class T> auto
median_of_3_sample(std::vector<T>& x, MPI_Comm comm) {
  size_t n_pivot = n_rank(comm)-1; // to get `n_rank` partitions, we need `n_rank-1` partition points
  return median_of_3_sample(x,n_pivot,comm);
}


} // std_e
