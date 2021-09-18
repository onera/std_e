#pragma once

#include <algorithm>
#include <functional>
#include "std_e/data_structure/jagged_range.hpp"
#include "std_e/algorithm/mismatch_points.hpp"


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
partition_sort(Rand_it0 first, Rand_it0 last, Rand_it1 pv_first, Rand_it1 pv_last, Rand_it2 pp_first, Bin_pred comp = {}) -> void {
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

// Same with indices return instead of pointers
template<class Rand_it0, class Rand_it1, class Rand_it2, class Bin_pred> constexpr auto
partition_sort_indices__impl(Rand_it0 first, Rand_it0 last, Rand_it1 pv_first, Rand_it1 pv_last, Rand_it2 pi_first, Bin_pred comp, Rand_it0 start) -> void {
  if (pv_first==pv_last) return;
  auto k = pv_last-pv_first;
  auto pv_mid = pv_first+k/2;
  auto pi_mid = pi_first+k/2;
  auto pp_mid = std::partition(first,last,[mid=*pv_mid,comp](const auto& x){ return comp(x,mid); });
  *pi_mid = pp_mid - start;
  if (k>1) {
    partition_sort_indices__impl(first ,pp_mid , pv_first,pv_mid  , pi_first , comp, start);
    partition_sort_indices__impl(pp_mid,last   , pv_mid  ,pv_last , pi_mid   , comp, start);
  }
}
template<class Rand_it0, class Rand_it1, class Rand_it2, class Bin_pred = std::less<>> constexpr auto
partition_sort_indices(Rand_it0 first, Rand_it0 last, Rand_it1 pv_first, Rand_it1 pv_last, Rand_it2 pi_first, Bin_pred comp = {}) -> void {
  return partition_sort_indices__impl(first,last,pv_first,pv_last,pi_first,comp,first);
}


template<class Rand_range0, class Rand_range1, class Bin_pred = std::less<>> constexpr auto
partition_sort(Rand_range0& rng, const Rand_range1& partition_values, Bin_pred comp = {}) {
  using it_type = typename Rand_range0::iterator;
  std::vector<it_type> partition_points(partition_values.size());
  partition_sort(begin(rng),end(rng),begin(partition_values),end(partition_values),begin(partition_points),comp);
  return partition_points;
}

template<class Rand_range0, class Rand_range1, class Bin_pred = std::less<>> auto
partition_sort_indices(Rand_range0& rng, const Rand_range1& partition_values, Bin_pred comp = {}) -> std::vector<int> {
  int k = partition_values.size();
  std::vector<int> partition_indices(k);
  partition_sort_indices(begin(rng),end(rng),begin(partition_values),end(partition_values),begin(partition_indices),comp);
  return partition_indices;
}

template<class Rand_range0, class Rand_range1, class Bin_pred = std::less<>> auto
indirect_partition_sort(const Rand_range0& rng, const Rand_range1& partition_values, Bin_pred comp = {}) {
  std::vector<int> new_to_old(rng.size());
  std::iota(begin(new_to_old),end(new_to_old),0);
  auto indirect_comp = [&rng,comp](int i, auto&& y){ return comp(rng[i],y); }; // TODO ugly
  auto partition_indices = std_e::partition_sort_indices(new_to_old,partition_values,indirect_comp);
  return std::make_pair(std::move(partition_indices),std::move(new_to_old));
}

template<class Rand_range0, class Rand_range1, class Bin_pred = std::less<>> auto
apply_indirect_partition_sort(Rand_range0& rng, const Rand_range1& partition_values, Bin_pred comp = {}) {
  auto res = std_e::indirect_partition_sort(rng,partition_values,comp);
  const auto& new_to_old = res.second;

  permute(rng,new_to_old); // TODO rename permute->inv_permute ?

  return res;
}


// TODO extract in iterator-level interface
// TODO if passed list of possible values, use partition_sort, else use std::sort
// TODO distinguish between cheap and costly swap of T
//    Here, assumed costly -> indirect sort
//    If swap is cheap (e.g. T is built-in), then the sort can be inplace
// TODO distinguish between cheap and costly "proj"
//    Here, we assume "proj" is costly, so we save the result
//    It "proj" is cheap, (e.g. access to an attribute) then we can compute it on-the-fly
template<class Range, class F, class T = typename Range::value_type> auto
sort_into_partitions(Range&& rng, F proj) -> jagged_vector<T> {
  auto&& compared_values = std_e::transform(rng,proj);
  auto sort_permutation = std_e::sort_permutation(compared_values);

  auto partition_indices = std_e::mismatch_indices(
    sort_permutation,
    [&compared_values](int i, int j){ return compared_values[i] == compared_values[j]; }
  );

  permute(rng,sort_permutation);
  return {FWD(rng),std::move(partition_indices)};
}


} // std_e
