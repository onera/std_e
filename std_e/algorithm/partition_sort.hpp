#pragma once

#include <algorithm>
#include <functional>


namespace std_e {

  
// Partition range [first,last) with partition values [pv_first,pv_last)
// The partition points are written in [pp_first,k), where k = pv_last-pv_first
// The algorithm is in-place and of complexity n*log_2(k), where n = last-first
// The idea of the algorithm is very similar to quicksort where you partition by dichotomy:
//    - first you partition by the middle partition value, pv_middle, finding the middle partition point pp_middle
//    - then you recursively partition each side
//        - left side with [first,pp_middle) and [pv_first,pv_middle)
//        - right side with [pp_middle,last) and [pv_middle,pv_last)
// Complexity: n*log_2(k)
//    - One each level lvl, you have 2^lvl partitions to apply, and all partition algorithms iterate over the whole range (complexity n)
//    - since you find 2^lvl partition points at each level, you need log_2(k) levels
template<class Rand_it0, class Rand_it1, class Rand_it2, class Bin_pred = std::less<>> constexpr auto
// requires first::value_type==pv_first::value_type(==T)
// requires Bin_pred(T,T)->bool
// requires [pv_first,pv_last) is sorted
// requires [pp_first,k) valid range (where k = pv_last-pv_first)
partition_sort(Rand_it0 first, Rand_it0 last, Rand_it1 pv_first, Rand_it1 pv_last, Rand_it2 pp_first, Bin_pred comp = std::less<>{}) -> void {
  if (pv_first==pv_last) return;
  auto k = pv_last-pv_first;
  auto pv_mid = pv_first+k/2;
  auto pp_mid = pp_first+k/2;
  *pp_mid = std::partition(first,last,[mid=*pv_mid,comp](const auto& x){ return comp(x,mid); });
  if (k>1) {
    partition_sort(first  ,*pp_mid , pv_first,pv_mid  , pp_first , comp);
    partition_sort(*pp_mid,last    , pv_mid  ,pv_last , pp_mid   , comp);
  }
}


template<class Rand_range0, class Rand_range1, class Bin_pred = std::less<>> constexpr auto
partition_sort(Rand_range0& rng, const Rand_range1& partition_values, Bin_pred comp = std::less<>{}) {
  using it_type = typename Rand_range0::iterator;
  std::vector<it_type> partition_points(partition_values.size());
  partition_sort(begin(rng),end(rng),begin(partition_values),end(partition_values),begin(partition_points),comp);
  return partition_points;
}

template<class Rand_range0, class Rand_range1, class Bin_pred = std::less<>> constexpr auto
partition_sort_indices(Rand_range0& rng, const Rand_range1& partition_values, Bin_pred comp = std::less<>{}) -> std::vector<int> {
  auto partition_points = std_e::partition_sort(rng,partition_values,comp);
  int k = partition_values.size();
  std::vector<int> partition_indices(k);
  for (int i=0; i<k; ++i) {
    partition_indices[i] = partition_points[i]-begin(rng);
  }
  return partition_indices;
}


} // std_e
