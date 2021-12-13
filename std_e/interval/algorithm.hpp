#pragma once


#include <algorithm>
#include "std_e/interval/interval.hpp"


namespace std_e {


template<class Int_array> auto
is_interval(const Int_array& x) -> bool {
  using I = typename Int_array::value_type;
  I sz = x.size();
  if (sz==0) return true;

  I next = x[0];
  for (I i=1; i<sz; ++i) {
    ++next;
    if (x[i] != next) {
      return false;
    }
  }
  return true;
}


template<class I> auto
rotated_position(I index, I first, I n_first, I last) -> I {
  // Returns the new index of a sequence [first,last) having been rotated around n_first
  // Preconditions:
  // - first <= n_first <= last
  I first_range_size = n_first-first;
  I second_range_size = last-n_first;
  if (is_in(interval{first,n_first},index)) {
    return index + second_range_size;
  }
  if (is_in(interval{n_first,last},index)) {
    return index - first_range_size;
  }
  return index;
}


template<class Rng, class Interval_sequence> auto
minmax_over_interval_sequence(const Rng& x, const Interval_sequence& indices) {
  using T = typename Rng::value_type;
  std::vector<T> mins;
  std::vector<T> maxs;
  for (int i=0; i<indices.n_interval(); ++i) {
    auto start  = begin(x)+indices[i];
    auto finish = begin(x)+indices[i+1];
    if (start != finish) {
      auto [min_it,max_it] = std::minmax_element(start,finish);
      mins.push_back( *min_it );
      maxs.push_back( *max_it );
    }
  }
  return std::make_pair(std::move(mins),std::move(maxs));
}


} // std_e
