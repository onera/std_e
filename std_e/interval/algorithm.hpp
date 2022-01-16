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


// TODO rename minmax_over_sub_ranges_indices
template<class Rng, class Interval_sequence, class Comp = std::less<>> auto
minmax_over_interval_sequence(const Rng& x, const Interval_sequence& indices, Comp comp = {}) {
  using T = typename Rng::value_type;
  if (indices.size()==0) {
    return std::make_pair(std::vector<T>{},std::vector<T>{});
  }
  std::vector<T> mins;
  std::vector<T> maxs;
  for (size_t i=0; i<indices.size()-1; ++i) {
    auto start  = begin(x)+indices[i];
    auto finish = begin(x)+indices[i+1];
    if (start != finish) {
      auto [min_it,max_it] = std::minmax_element(start,finish,comp);
      mins.push_back( *min_it );
      maxs.push_back( *max_it );
    }
  }
  return std::make_pair(std::move(mins),std::move(maxs));
}

template<class Rng, class Interval_sequence, class Comp = std::less<>> auto
is_partitioned_at_indices(const Rng& x, const Interval_sequence& partition_indices, Comp comp = {}) -> bool {
  auto [min_by_partition,max_by_partition] = minmax_over_interval_sequence(x,partition_indices,comp);
  for (size_t i=0; i<min_by_partition.size()-1; ++i) {
    if (!comp(max_by_partition[i],min_by_partition[i+1])) {
      return false;
    }
  }
  return true;
}

// TODO factor with minmax_over_interval_sequence
template<class Rng, class It_range, class Comp = std::less<>> auto
minmax_over_sub_ranges(const Rng& x, const It_range& partition_points, Comp comp = {}) {
  using T = typename Rng::value_type;
  if (partition_points.size()==0) {
    return std::make_pair(std::vector<T>{},std::vector<T>{});
  }
  std::vector<T> mins;
  std::vector<T> maxs;
  for (size_t i=0; i<partition_points.size()-1; ++i) {
    auto start  = partition_points[i];
    auto finish = partition_points[i+1];
    if (start != finish) {
      auto [min_it,max_it] = std::minmax_element(start,finish,comp);
      mins.push_back( *min_it );
      maxs.push_back( *max_it );
    }
  }
  return std::make_pair(std::move(mins),std::move(maxs));
}


// TODO factor with is_partitioned_at_indices
template<class Rng, class It_range, class Comp = std::less<>> auto
is_partitioned_at(const Rng& x, const It_range& partition_points, Comp comp = {}) -> bool {
  auto [min_by_partition,max_by_partition] = minmax_over_sub_ranges(x,partition_points,comp);
  for (size_t i=0; i<min_by_partition.size()-1; ++i) {
    if (!comp(max_by_partition[i],min_by_partition[i+1])) {
      return false;
    }
  }
  return true;
}

//template<class Rng0,class Rng1> auto
//contains(const Rng0& x, const Rng1& sub_x) -> bool {
//  return x[0] <= sub_x[0] && sub_x.back() <= x.back();
//}


} // std_e
