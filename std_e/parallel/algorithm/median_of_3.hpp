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


template<class T, class I> auto
median_of_3_sample(std::vector<span<T>>& xs, std::vector<I> ns, MPI_Comm comm) {
  // TODO implement with only one gather
  STD_E_ASSERT(xs.size()==ns.size());
  int n_interval = xs.size();

  std::vector<std::vector<T>> pivots_by_sub_intervals(n_interval);
  for (int i=0; i<n_interval; ++i) {
    pivots_by_sub_intervals[i] = median_of_3_sample(xs[i],ns[i],comm);
  }
  return pivots_by_sub_intervals;
}


template<class T> auto
median_of_3_sample(std::vector<T>& x, MPI_Comm comm) {
  size_t n_pivot = n_rank(comm)-1; // to get `n_rank` partitions, we need `n_rank-1` partition points
  return median_of_3_sample(x,n_pivot,comm);
}


} // std_e
