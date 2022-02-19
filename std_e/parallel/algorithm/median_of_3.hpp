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


template<
  class RA_rng, class I = typename RA_rng::size_type, class Proj = identity_closure,
  class T = std::ranges::range_value_t<RA_rng>, class T_piv = proj_return_type<Proj,T>
> auto
median_of_3_sample(const RA_rng& x, I n_pivot, MPI_Comm comm, Proj proj = {}) -> std::vector<T_piv> {
  I size_tot = all_reduce(x.size(),MPI_SUM,comm);
  I n_sample = std::min(size_tot,3*n_pivot); // 3*n_pivot by analogy with median of 3 (if n_pivot==1, we have a median of 3)
  return uniform_sample_local(x,n_sample,comm,proj);
}


// Sample all ranks equally regardless of their size
// Avoid the start and end (because the end of one rank is the element just after the start of the next rank)
// TODO the problem with the algorithm is that it takes 3*n_rank values
//    - it can't be used in a context where n_pivot>3*n_rank
//    - more importantly, it may be too much samples (worst case: pivot_partition_eq may have an iteration with n_rank/2 interval -> 3/2*n_rank^2 samples!
//  -> Solution: n_pivot gives which ranks to hit, and how many times, but then we take values in the rank similar to this (excluding begin/end)
template<
  class RA_rng, class I = typename RA_rng::size_type, class Proj = identity_closure,
  class T = std::ranges::range_value_t<RA_rng>, class T_piv = proj_return_type<Proj,T>
> auto
median_of_3_sample_mod(const RA_rng& x, I n_pivot, Proj proj = {}) -> std::vector<T_piv> {
  auto sz = x.size();
  if (sz==0) {
    return {};
  } else if (sz==1) {
    return {proj(x[0])};
  } else if (sz==2) {
    return {proj(x[0]),proj(x[1])};
  } else { // sz>=3
    // if we view the global array as the concatenation of `x` accross the ranks
    // and if we suppose that `x.size()` is the same accross the ranks,
    // then this sampling is uniform and avoids the ends
    return {proj(x[sz/6]),proj(x[sz/2]),proj(x[5*sz/6])};

    // worse choices :D
    // these two trigger a linear sub-interval reduction (instead of exponential)
    // return {x[0]};
    // return {x[1]};
    //
    // return {x[sz/2]};
    // return {x[sz/4],x[3*sz/4]};
    // return {x[0],x[sz/6],x[sz/3],x[sz/2],x[2*sz/3],x[5*sz/6]};
  }
}


constexpr auto median_of_3_sample_fn     = [](const auto& x, auto n_pivot, MPI_Comm comm, auto proj){ return median_of_3_sample    (x,n_pivot,comm,proj); };
constexpr auto median_of_3_sample_mod_fn = [](const auto& x, auto n_pivot, MPI_Comm     , auto proj){ return median_of_3_sample_mod(x,n_pivot     ,proj); };
using median_of_3_sample_mod_closure = decltype(median_of_3_sample_mod_fn);


template<
  class RA_rng, class I, class sampling_algo_type, class Proj = identity_closure,
  class T = std::ranges::range_value_t<RA_rng>, class T_piv = proj_return_type<Proj,T>
> auto
find_pivots_by_sampling(const RA_rng& x, I n_pivot, MPI_Comm comm, sampling_algo_type sampling_algo, Proj proj = {}) -> std::vector<T_piv> {
  // 1. local samples
  std::vector<T_piv> sample_local = sampling_algo(x,n_pivot,comm,proj);

  // 2. gather all samples and sort them
  std::vector<T_piv> sample = all_gather(sample_local,comm); // could be optimized because can be pre-allocated at n_sample
  std::sort(begin(sample),end(sample));

  // 3. get exactly `n_pivot` values
  I n_sample = sample.size();
  if (n_sample >= n_pivot) {
    // re-sample
    return uniform_sample_exclude_ends(sample,n_pivot);
  } else {
    // expand with empty toward the end
    T_piv last_sample = sample.back();
    sample.resize(n_pivot);
    std::fill(begin(sample)+n_sample,end(sample),last_sample);
    return sample;
  }
}



template<
  class RA_rng, class I, class Proj = identity_closure, class sampling_algo_type = median_of_3_sample_mod_closure,
  class T = std::ranges::range_value_t<RA_rng>, class T_piv = proj_return_type<Proj,T>
> auto
find_pivots(std::vector<RA_rng>& xs, const std::vector<I>& ns, MPI_Comm comm, Proj proj = {}, sampling_algo_type sampling_algo = {}) {
  // TODO implement with only one gather:
  // because worst case complexity:
  //   pivot_partition_eq may have an iteration with n_rank/2 interval, then we have n_rank MPI calls
  //    -> merge them into one!
  STD_E_ASSERT(xs.size()==ns.size());
  int n_interval = xs.size();

  std::vector<std::vector<T_piv>> pivots_by_sub_intervals(n_interval);
  for (int i=0; i<n_interval; ++i) {
    pivots_by_sub_intervals[i] = find_pivots_by_sampling(xs[i],ns[i],comm,sampling_algo,proj);
  }
  return pivots_by_sub_intervals;
}


template<class RA_rng, class Proj = identity_closure> auto
median_of_3_sample(const RA_rng& x, MPI_Comm comm, Proj proj = {}) {
  size_t n_pivot = n_rank(comm)-1; // to get `n_rank` partitions, we need `n_rank-1` partition points
  return find_pivots_by_sampling(x,n_pivot,comm,median_of_3_sample_fn,proj);
}


} // std_e
