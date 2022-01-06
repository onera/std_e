#pragma once


#include "std_e/future/contract.hpp"


namespace std_e {


template<class I> constexpr auto
number_of_ticks_in_interval(I n_buckets, I n_tick, I i_bucket) -> I {
  STD_E_ASSERT(0<=i_bucket && i_bucket<n_buckets);
  I n = n_buckets;
  I k = n_tick;
  I i = i_bucket;
  // We can view the problem as the following:
  //    - We take the interval [0,(k+1)n)
  //    - for i in [0,n) sub-interval i is [(k+1)i , (k+1)(i+1))
  //    - for j in [1,k+1), tick j is j*n
  // Then, given n,k,i, we want to compute the number of ticks within sub-interval i
  // That is, the number of integer j for which (k+1)i <= j*n < (k+1)(i+1)
  I j_min = (k+1)*i / n; // ensures j_min is the inf or just before the inf of interval i
  if ( ((k+1)*i > j_min*n) || j_min==0 ) { // if strictly before the inf, or j not in [1,k+1), take next
    ++j_min;
  }
  I j_max = (k+1)*(i+1) / n; // ensures j_max is the sup or just before the sup of interval i
  if ( (k+1)*(i+1) == j_max*n ) { // if exactly the sup, this is not okay since we are open, so take previous
    --j_max;
  }
  if ((j_max-j_min) < 0) return 0;
  else return j_max-j_min + 1;
}

// TODO clean
template<class I> constexpr auto
uniform_ticks_in_sub_interval(I start, I finish, I end, I n_tick) -> std::vector<I> {
  // closed interval [0,end)
  // sub-interval [start,finish)
  // if we were to place `n_tick` uniformly spaced ticks on [0,end),
  // compute the ticks that would be lying inside [start,finish)
  //    Note: given
  //            n_partition = n_tick-1
  //            size = end
  //          if size%n_partition != 0, uniformity is impossible,
  //          so the first size%n_partition ticks are spaced by size/n_partition + 1
  //          and the others ticks are spaced by size/n_partition
  I size = end;
  STD_E_ASSERT(n_tick <= size);
  STD_E_ASSERT(start >= 0);
  STD_E_ASSERT(finish <= end);

  if (n_tick == 0) return {};
  if (n_tick == 1) {
    I mid = (end-1)/2 + (end-1)%2;
    if (start <= mid && mid < finish) {
      return {mid};
    } else {
      return {};
    }
  }

  I n_partition = n_tick-1;

  I spacing_round_down = (end-1)/n_partition;
  I spacing_round_up = spacing_round_down+1;
  I n_round_up = (end-1)%n_partition;

  std::vector<I> ticks;
  if (start < n_round_up*spacing_round_up) { // if only spacing_round_up steps to get to start
    I i_tick = start/spacing_round_up;
    I n_round_up_used = i_tick;
    I first_tick = i_tick*spacing_round_up;
    if (first_tick < start) { // if first_tick not in interval, take next
      first_tick += spacing_round_up;
      ++n_round_up_used;
    }

    I n_round_up_remaining = n_round_up - n_round_up_used;
    while (first_tick < finish && n_round_up_remaining>0) {
      ticks.push_back(first_tick);
      first_tick += spacing_round_up;
      --n_round_up_remaining;
    }
    while (first_tick < finish) {
      ticks.push_back(first_tick);
      first_tick += spacing_round_down;
    }
  } else {
    I start0 = start - n_round_up*spacing_round_up;
    I i_tick0 = start0/spacing_round_down;
    I first_tick = n_round_up*spacing_round_up + i_tick0*spacing_round_down;
    if (first_tick < start) first_tick += spacing_round_down;

    while (first_tick < finish) {
      ticks.push_back(first_tick);
      first_tick += spacing_round_down;
    }
  }

  return ticks;
}


} // std_e
