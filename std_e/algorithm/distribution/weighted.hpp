#pragma once


#include "std_e/algorithm/iota.hpp"
#include "std_e/algorithm/distribution/uniform.hpp"
#include "std_e/interval/interval_sequence.hpp"
#include <algorithm>
#include <numeric>


namespace std_e {


template<class Fwd_it0, class S0, class Fwd_it1, class S1, class Fwd_it2, class Out_it> constexpr auto
balanced_distribution(Fwd_it0 first, S0 last, Fwd_it1 first_size, S1 last_size, Fwd_it2 first_weights, Out_it first_weighted) -> void {
  using I = typename std::iterator_traits<Fwd_it1>::value_type;
  I total_weighted_size = std::inner_product(first_size,last_size,first_weights,I(0));
  // 0. distribution if the size of every block size was multiplied by its weight
  uniform_distribution(first,last,I(0),total_weighted_size);
  *first_weighted = 0;

  // 1. now we need to change this distribution by dividing by the weights
  //    for that, we iterate through each bound of the "multiplied by weight" interval to replace it with the real bound
  // TODO clean
  I prev_weighted_size = 0;
  I partial_size = 0;
  // For each bound:
  while (++first != last) {
    I weighted_size = *first - prev_weighted_size;
    I size = 0;
    // 1.0. Put as much complete blocks as you can
    while (true) {
      I remaining_first_size = *first_size-partial_size;
      I w_sz = (remaining_first_size)*(*first_weights);
      if (weighted_size>w_sz) {
        weighted_size -= w_sz;
        size += remaining_first_size;
        ++first_size;
        ++first_weights;
        partial_size = 0; // since we managed to fit all the elements of the block
      } else {
        break;
      }
    }
    I remaining_weighted_size = 0;
    // 1.1. If there are remaining space to get to the bound but one block does not fit completly,
    //      take as much element as possible, but do not cross the "multiplied by weight" corresponding bound
    if (weighted_size>0) {
      I cur_partial_size = weighted_size / *first_weights; // number of elts we managed to fit
      size += cur_partial_size;
      remaining_weighted_size = weighted_size % *first_weights;
      partial_size += cur_partial_size;
    }
    // 1.2. If there is a remainder, take it into account for the next iteration
    //      Ensures that the last bound is exactly the sum all all block sizes
    prev_weighted_size = *first - remaining_weighted_size;
    *first = *std::prev(first)+size;
    *++first_weighted = prev_weighted_size;
  }
}
template<class I0, class Range0, class Range1> constexpr auto
balanced_distribution(I0 dist_sz, const Range0& block_sizes, const Range1& block_weights) {
  using I = typename Range0::value_type;
  std_e::interval_vector<I> dist(dist_sz);
  std_e::interval_vector<I> dist_weighted(dist_sz);

  balanced_distribution(
    begin(dist),end(dist),
    begin(block_sizes),end(block_sizes),
    begin(block_weights),
    begin(dist_weighted)
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
  I elt_before_inf = rem/w_elts[i];
  if (acc + n_elts[i]*w_elts[i] > sup) { // enough to fill the interval
    I rem2 = acc + n_elts[i]*w_elts[i] - sup;
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
