#pragma once


#include "std_e/parallel/partial_distribution.hpp"
#include "std_e/algorithm/distribution/ticks_in_interval.hpp"
#include "std_e/plog.hpp"


namespace std_e {


template<
  class RA_rng, class I,
  class T = typename RA_rng::value_type
> auto
uniform_sample_local(const RA_rng& x, I n_sample, MPI_Comm comm) -> std::vector<T> {
  I sz = x.size();

  auto d = partial_distribution(sz,comm);

  STD_E_ASSERT(n_sample <= d.size_tot);
  if (n_sample == 0) return {};
  STD_E_ASSERT(d.size_tot>0); // this will hold because the two previous lines
                              // ensured we have 0 < n_sample <= d.size_tot

  SLOG(comm,d);
  SLOG(comm,n_sample);
  std::vector<I> sample_indices = uniform_ticks_in_sub_interval(d.start,d.finish,d.size_tot,n_sample);
  SLOG(comm,sample_indices);

  I n_sample_local = sample_indices.size();
  std::vector<T> sample_local(n_sample_local);
  for (I i=0; i<n_sample_local; ++i) {
    sample_local[i] = x[ sample_indices[i]-d.start ];
  }
  return sample_local;
}


} // std_e
