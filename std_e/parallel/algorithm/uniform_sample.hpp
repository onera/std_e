#pragma once


#include "std_e/parallel/partial_distribution.hpp"
#include "std_e/algorithm/distribution/ticks_in_interval.hpp"


namespace std_e {


template<
  class RA_rng, class I, class Proj = identity_closure,
  class T = typename RA_rng::value_type, class T_piv = proj_return_type<Proj,T>
> auto
uniform_sample_local(const RA_rng& x, I n_sample, MPI_Comm comm, Proj proj = {}) -> std::vector<T_piv> {
  I sz = x.size();

  auto d = partial_distribution(sz,comm);

  STD_E_ASSERT(n_sample <= d.size_tot);
  if (n_sample == 0) return {};
  STD_E_ASSERT(d.size_tot>0); // this will hold because the two previous lines
                              // ensured we have 0 < n_sample <= d.size_tot

  std::vector<I> sample_indices = uniform_ticks_in_sub_interval(d.start,d.finish,d.size_tot,n_sample);

  I n_sample_local = sample_indices.size();
  std::vector<T_piv> sample_local(n_sample_local);
  for (I i=0; i<n_sample_local; ++i) {
    sample_local[i] = proj(x[ sample_indices[i]-d.start ]);
  }
  return sample_local;
}


} // std_e
