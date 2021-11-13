#pragma once


#include "std_e/algorithm/iota.hpp"
#include <algorithm>
#include <iostream>
#include <numeric>


namespace std_e {


template<class Fwd_it, class T> constexpr auto
uniform_distribution(Fwd_it first, Fwd_it last, T elt0, T nb_elts) {
  auto n_interval = std::distance(first,last)-1;

  STD_E_ASSERT(n_interval>=0);

  if (n_interval==0) {
    STD_E_ASSERT(nb_elts==0);
    *first = elt0;
    return;
  }

  T quotient  = nb_elts/n_interval;
  T remainder = nb_elts%n_interval;
  auto last_upper = exclusive_iota_n(first     ,remainder+1,elt0           ,quotient+1);
                    inclusive_iota  (last_upper,last       ,*(last_upper-1),quotient  );
}


template<class Fwd_it> constexpr auto
uniform_distribution(Fwd_it first, Fwd_it last, double elt0, double nb_elts) {
  auto n_interval = std::distance(first,last)-1;

  STD_E_ASSERT(n_interval>=0);

  double quotient  = nb_elts/n_interval;
  for(int i = 0; i < n_interval+1; ++i) {
    *first++ = elt0 + i*quotient;
  }
}

template<class Fwd_it, class T> constexpr auto
uniform_distribution(Fwd_it first, Fwd_it last, T nb_elts) {
  return uniform_distribution(first,last,T(0),nb_elts);
}


template<class Fwd_it0, class S0, class Fwd_it1, class S1, class Fwd_it2, class Out_it> constexpr auto
distribution_weighted_by_blocks(Fwd_it0 first, S0 last, Fwd_it1 first_size, S1 last_size, Fwd_it2 first_weights, Out_it first_weighted) -> void {
  auto n_interval = std::distance(first,last)-1;
  using I = typename std::iterator_traits<Fwd_it1>::value_type;
  I total_weighted_size = std::inner_product(first_size,last_size,first_weights,I(0));
  // 0. distribution if the size of every block size was multiplied by its weight
  uniform_distribution(first,last,I(0),total_weighted_size);

  // 1. now we need to change this distribution by dividing by the weights
  //    for that, we iterate through each bound of the "multiplied by weight" interval to replace it with the real bound
  // TODO clean
  I acc = 0;
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
    // 1.3. If there is a remainder, take it into account for the next iteration
    //      Ensures that the last bound is exactly the sum all all block sizes
    prev_weighted_size = *first - remaining_weighted_size;
    *first = *std::prev(first)+size;
    *++first_weighted = prev_weighted_size;
  }
}
template<class I0, class Range0, class Range1> constexpr auto
distribution_weighted_by_blocks(I0 dist_sz, const Range0& block_sizes, const Range1& block_weights) {
  using I = typename Range0::value_type;
  std::vector<I> dist(1+dist_sz);
  std::vector<I> dist_weighted(1+dist_sz);

  distribution_weighted_by_blocks(
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
  I elt_after_inf = n_elts[i] - elt_before_inf;
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


} // std_e
