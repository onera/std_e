#pragma once


#include <vector>
//#include "std_e/algorithm/distribution/uniform_range.hpp"
#include "std_e/algorithm/distribution/ticks_in_interval.hpp"


namespace std_e {


//template<
//  class RA_rng, class I,
//  class T = typename RA_rng::value_type
//> auto
//uniform_sample(const RA_rng& x, I n_sample) -> std::vector<T> {
//  if (n_sample==0) return {};
//
//  I sz = x.size();
//  STD_E_ASSERT(sz >= n_sample);
//
//  std::vector<T> sample(n_sample);
//
//  auto gen = make_uniform_distribution_generator(sz,n_sample); // TODO RENAME (no link to distri...)
//  I sub_interval_sz = sz/n_sample;
//  for (I i=0; i<n_sample; ++i) {
//    I start_sub_interval_i = *gen;
//    I sample_i = start_sub_interval_i + sub_interval_sz/2; // spread over the sample, then shift to get middle positions
//    ++gen; // increment to the next sub-interval
//    sample[i] = x[sample_i];
//  }
//  return sample;
//}


template<
  class RA_rng, class I,
  class T = typename RA_rng::value_type
> auto
uniform_sample(const RA_rng& x, I n_sample) -> std::vector<T> {
  I sz = x.size();
  STD_E_ASSERT(sz >= n_sample);

  std::vector<I> sample_indices = uniform_ticks_in_sub_interval(I(0),sz,sz,n_sample);

  std::vector<T> sample(n_sample);
  for (I i=0; i<n_sample; ++i) {
    sample[i] = x[ sample_indices[i] ];
  }
  return sample;
}

// TODO test
template<
  class RA_rng, class I,
  class T = typename RA_rng::value_type
> auto
uniform_sample_exclude_ends(const RA_rng& x, I n_sample) -> std::vector<T> {
  I sz = x.size();
  STD_E_ASSERT(sz >= n_sample);

  if (sz == n_sample) {
    return x;
  }

  if (sz == n_sample+1) {
    std::vector<T> sample(n_sample);
    std::copy_n(begin(x),n_sample,begin(sample));
    return sample;
  }

  else { // ask for two more samples, but then drop both ends
    std::vector<I> sample_indices = uniform_ticks_in_sub_interval(I(0),sz,sz,n_sample+2);

    std::vector<T> sample(n_sample);
    for (I i=0; i<n_sample; ++i) {
      sample[i] = x[ sample_indices[i]+1 ];
    }
    return sample;
  }
}


} // std_e
