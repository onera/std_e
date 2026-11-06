#pragma once


#include "std_e/algorithm/iota.hpp"
#include "std_e/algorithm/distribution/uniform.hpp"
#include "std_e/interval/interval_sequence.hpp"
#include <algorithm>
#include <numeric>
#include "std_e/debug.hpp"
#include "std_e/contract/contract.hpp"


namespace std_e {


// TODO rename weighted_distribution
template<class Rng0, class Rng1, class Rng2, class Rng3> constexpr auto
balanced_distribution(const Rng0& sizes, const Rng1& weights, Rng2& dist, Rng3& weighted) -> void {
  using I = typename Rng1::value_type;

  I n = dist.size();
  I n_block = sizes.size();

  STD_E_PRECOND_LVL0(n>0);
  STD_E_PRECOND_LVL0(n_block>0);
  STD_E_PRECOND_LVL0(sizes.size() == weights.size());
  STD_E_PRECOND_LVL0(dist.size() == weighted.size());
  STD_E_PRECOND_LVL1(*std::min       (sizes  .begin(),sizes  .end()) >= 1);
  STD_E_PRECOND_LVL1(*std::min       (weights.begin(),weights.end()) >= 0);
  STD_E_PRECOND_LVL1(std::accumulate(weights.begin(),weights.end(),I(0)) >  0);

  // 0. first guess for `weighted` (almost correct, except we may cut inside a stride)
  I total_weighted_size = std::inner_product(sizes.begin(),sizes.end(),weights.begin(),I(0));
  uniform_distribution_n(weighted.begin(),n,total_weighted_size);

  // 1. Compute the final results

  /// 1.0. first element
  dist[0] = 0;
  // weighted[0] = 0; already done by first guess

  // 1.1. interior
  I acc = 0;
  I dist_acc = 0;
  I i_block = 0;
  for (I i=0; i<n-1; ++i) {
    STD_E_INVARIANT_LVL0(acc <= weighted[i+1]); // true at `i=0`,
                                                // and `acc` will only be updated to `next_acc` when `next_acc < weighted[i+1]`
                                                // and `weighted[i+1]` is only updated after it has been read
    while (i_block < n_block) { // loop over the inputs while we can
      I next_acc = acc + sizes[i_block] * weights[i_block]; // accumulate until we reach the first guess
      if (next_acc <= weighted[i+1]) { // we are before (or just on) `weighted` first guess: continue to accumulate
        acc = next_acc;
        dist_acc = dist_acc + sizes[i_block];
        ++i_block;
      } else { // now we are one block passed the guess
        STD_E_INVARIANT_LVL0(weights[i_block] > 0); // here, we are on a branch where `next_acc > weighted[i+1]`,
                                                    // i.e. where `acc + sizes[i_block] * weights[i_block] > weighted[i+1]`,
                                                    // but the previous invariant says `acc < weighted[i+1]`
                                                    // so we need `sizes[i_block] * weights[i_block] > 0`
        I quo = (weighted[i+1] - acc) / weights[i_block];
        I rem = (weighted[i+1] - acc) % weights[i_block];
        if (rem>0) ++quo; // if not equal to guess, always go after the cut
        weighted[i+1] = acc + quo*weights[i_block];
        dist[i+1] = dist_acc + quo;
        break; // we are done with index `i`
      }
    }
  }

  /// 1.2. last element
  dist.back() = dist_acc;
  // weighted.back() = total_weighted_size; already done by first guess
}

template<class I0, class Range0, class Range1> constexpr auto
balanced_distribution(I0 dist_sz, const Range0& sizes, const Range1& weights) {
  using I = typename Range0::value_type;
  std_e::interval_vector<I> dist(dist_sz);
  std_e::interval_vector<I> dist_weighted(dist_sz);

  balanced_distribution(
    sizes,
    weights,
    dist,
    dist_weighted
  );

  return std::make_pair(dist,dist_weighted);
}


template<class I, class Range> auto
elements_in_interval(I inf, I sup, const Range& n_elts, const Range& w_elts) {
  // TODO clean
  STD_E_ASSERT(n_elts.size()==w_elts.size());
  int n = n_elts.size();
  std::vector<I> elts_inf(n,0);
  std::vector<I> elts_sup(n,0);
  I acc = 0;
  int i=0;
  while (acc + n_elts[i]*w_elts[i] < inf) {
    acc += n_elts[i]*w_elts[i];
    elts_inf[i] = n_elts[i];
    elts_sup[i] = n_elts[i];
    ++i;
  }
  I rem = inf - acc;
  STD_E_ASSERT(w_elts[i]>0);
  I elt_before_inf = rem/w_elts[i];
  if (acc + n_elts[i]*w_elts[i] > sup) { // enough to fill the interval
    I rem2 = acc + n_elts[i]*w_elts[i] - sup;
    STD_E_ASSERT(w_elts[i]>0);
    I elt_after_sup = rem2/w_elts[i];
    elts_inf[i] = elt_before_inf;
    elts_sup[i] = n_elts[i]- elt_after_sup;
  } else {
    acc += n_elts[i]*w_elts[i];
    elts_inf[i] = elt_before_inf;
    elts_sup[i] = n_elts[i];
    ++i;
    if (i<n) {
      while (acc + n_elts[i]*w_elts[i] < sup) {
        acc += n_elts[i]*w_elts[i];
        elts_inf[i] = 0;
        elts_sup[i] = n_elts[i];
        ++i;
      }
      I rem3 = sup - acc;
      STD_E_ASSERT(w_elts[i]>0);
      I elt_before_sup = rem3/w_elts[i];
      elts_inf[i] = 0;
      elts_sup[i] = elt_before_sup;
    }
  }
  return std::make_pair(elts_inf,elts_sup);
}

template<class I, class Range> auto
elements_in_interval(I inf, I sup, const Range& n_elts) {
  auto sz = n_elts.size();
  std::vector<I> w_elts(sz,1);
  return elements_in_interval(inf,sup,n_elts,w_elts);
}
template<class I, class Range> auto
n_elements_in_interval(I inf, I sup, const Range& n_elts) -> std::vector<I> {
  size_t sz = n_elts.size();
  auto [elts_inf,elts_sup] = elements_in_interval(inf,sup,n_elts);
  std::vector<I> ns(sz);
  for (size_t i=0; i<sz; ++i) {
    ns[i] = elts_sup[i]-elts_inf[i];
  }
  return ns;
}
template<class I, class Range> auto
n_elements_in_interval(interval<I> inter, const Range& n_elts) -> std::vector<I> {
  return n_elements_in_interval(inter.first(),inter.last(),n_elts);
}


} // std_e
