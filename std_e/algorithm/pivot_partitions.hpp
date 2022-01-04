#pragma once

#include <algorithm>
#include <functional>
#include "std_e/data_structure/jagged_range.hpp"
#include "std_e/interval/interval_sequence.hpp"
#include "std_e/algorithm/mismatch_points.hpp"


namespace std_e {


// Partition range [first,last) with partition values [piv_first,piv_last)
// The partition points are written in [out_first,k), where k = piv_last-piv_first
// The algorithm is in-place and of complexity n*log_2(k), where n = last-first
// The idea of the algorithm is very similar to quicksort where you partition by dichotomy:
//    - first you partition by the middle partition value, pv_middle, finding the middle partition point pp_middle
//    - then you recursively partition each side
//        - left side with [first,pp_middle) and [piv_first,pv_middle)
//        - right side with [pp_middle,last) and [pv_middle,piv_last)
// Complexity: n*log_2(k)
//    - One each level lvl, you have 2^lvl partitions to apply, and all partition algorithms iterate over the whole range (complexity n)
//    - since you find 2^lvl partition points at each level, you need log_2(k) levels
template<class RA_it, class RA_it_piv, class RA_it_out, class Bin_pred = std::less<>> constexpr auto
// requires first::value_type==piv_first::value_type(==T)
// requires Bin_pred(T,T)->bool
// requires [piv_first,piv_last) is sorted
// requires [out_first,k) valid range (where k = piv_last-piv_first)
pivot_partitions(RA_it first, RA_it last, RA_it_piv piv_first, RA_it_piv piv_last, RA_it_out out_first, Bin_pred comp = {}) -> void {
  if (piv_first==piv_last) return;
  auto k = piv_last-piv_first;
  auto pv_mid = piv_first+k/2;
  auto pp_mid = out_first+k/2;
  *pp_mid = std::partition(first,last,[&mid=*pv_mid,comp](const auto& x){ return comp(x,mid); });
  if (k>1) {
    pivot_partitions(first  ,*pp_mid , piv_first,pv_mid  , out_first , comp);
    pivot_partitions(*pp_mid,last    , pv_mid  ,piv_last , pp_mid   , comp);
  }
}


// Same idea as `std_e::partition`,
// but `std_e::partition_point` is to `std_e::partition`
// what `std::partition_point` is to `std::partition`.
// That is, while the latter does the partition and returns the partition point,
// the former pre-supposes that the range is already partitioned, and returns the partition point.
template<class RA_it, class RA_it_piv, class RA_it_out, class Bin_pred = std::less<>> constexpr auto
pivot_partition_points(RA_it first, RA_it last, RA_it_piv piv_first, RA_it_piv piv_last, RA_it_out out_first, Bin_pred comp = {}) -> void {
  if (piv_first==piv_last) return;
  auto k = piv_last-piv_first;
  auto pv_mid = piv_first+k/2;
  auto pp_mid = out_first+k/2;
  *pp_mid = std::partition_point(first,last,[&mid=*pv_mid,comp](const auto& x){ return comp(x,mid); });
  if (k>1) {
    pivot_partition_points(first  ,*pp_mid , piv_first,pv_mid  , out_first , comp);
    pivot_partition_points(*pp_mid,last    , pv_mid  ,piv_last , pp_mid   , comp);
  }
}

// Same idea as `std_e::partition`,
// but elements returned in [pi_first,k) are the indices (i.e. integers) of the partition points,
// rather than the partition points (i.e. iterators) themselves
template<class RA_it, class RA_it_piv, class RA_it_out, class Bin_pred> constexpr auto
pivot_partitions_i_aux(RA_it first, RA_it last, RA_it_piv piv_first, RA_it_piv piv_last, RA_it_out pi_first, Bin_pred comp, RA_it start) -> void {
  if (piv_first==piv_last) return;
  auto k = piv_last-piv_first;
  auto pv_mid = piv_first+k/2;
  auto pi_mid = pi_first+k/2;
  auto pp_mid = std::partition(first,last,[&mid=*pv_mid,comp](const auto& x){ return comp(x,mid); });
  *pi_mid = pp_mid - start;
  if (k>1) {
    pivot_partitions_i_aux(first ,pp_mid , piv_first,pv_mid  , pi_first , comp, start);
    pivot_partitions_i_aux(pp_mid,last   , pv_mid  ,piv_last , pi_mid   , comp, start);
  }
}

// Same idea as `std_e::partition_point`, but returns the indices rather than the iterators
template<class RA_it, class RA_it_piv, class RA_it_out, class Bin_pred> constexpr auto
pivot_partition_points_i_aux(RA_it first, RA_it last, RA_it_piv piv_first, RA_it_piv piv_last, RA_it_out pi_first, Bin_pred comp, RA_it start) -> void {
  if (piv_first==piv_last) return;
  auto k = piv_last-piv_first;
  auto pv_mid = piv_first+k/2;
  auto pi_mid = pi_first+k/2;
  auto pp_mid = std::partition_point(first,last,[&mid=*pv_mid,comp](const auto& x){ return comp(x,mid); });
  *pi_mid = pp_mid - start;
  if (k>1) {
    pivot_partition_points_i_aux(first ,pp_mid , piv_first,pv_mid  , pi_first , comp, start);
    pivot_partition_points_i_aux(pp_mid,last   , pv_mid  ,piv_last , pi_mid   , comp, start);
  }
}

template<class RA_it, class RA_it_piv, class RA_it_out, class Bin_pred = std::less<>> constexpr auto
pivot_partitions_i(RA_it first, RA_it last, RA_it_piv piv_first, RA_it_piv piv_last, RA_it_out pi_first, Bin_pred comp = {}) -> void {
  return pivot_partitions_i_aux(first,last,piv_first,piv_last,pi_first,comp,first);
}
template<class RA_it, class RA_it_piv, class RA_it_out, class Bin_pred = std::less<>> constexpr auto
pivot_partition_points_i(RA_it first, RA_it last, RA_it_piv piv_first, RA_it_piv piv_last, RA_it_out pi_first, Bin_pred comp = {}) -> void {
  return pivot_partition_points_i_aux(first,last,piv_first,piv_last,pi_first,comp,first);
}


// ========================================================
// Range versions
template<
  class RA_rng, class RA_pivot_rng,
  class Bin_pred = std::less<>,
  class Return_container = interval_vector<typename RA_rng::iterator>
> constexpr auto
partition(RA_rng& rng, const RA_pivot_rng& pivots, Bin_pred comp = {}, Return_container&& partition_pts = {}) -> Return_container {
  int n = rng.size();
  int k = pivots.size();
  partition_pts.resize(k);
  partition_pts[0] = begin(rng);
  partition(begin(rng),end(rng), begin(pivots),end(pivots), begin(partition_pts)+1, comp);
  partition_pts.back() = end(rng);
  return partition_pts;
}

template<
  class RA_rng, class RA_pivot_rng,
  class Bin_pred = std::less<>,
  class Return_container = interval_vector<typename RA_rng::iterator>
> constexpr auto
partition_point(RA_rng& rng, const RA_pivot_rng& pivots, Bin_pred comp = {}, Return_container&& partition_pts = {}) -> Return_container {
  int n = rng.size();
  int k = pivots.size();
  partition_pts.resize(k);
  partition_pts[0] = begin(rng);
  partition_point(begin(rng),end(rng), begin(pivots),end(pivots), begin(partition_pts)+1, comp);
  partition_pts.back() = end(rng);
  return partition_pts;
}

template<
  class RA_rng, class RA_pivot_rng,
  class Bin_pred = std::less<>,
  class Return_container = interval_vector<int>
> constexpr auto
partition_indices(RA_rng& rng, const RA_pivot_rng& pivots, Bin_pred comp = {}, Return_container&& partition_is = {}) -> Return_container {
  int n = rng.size();
  int k = pivots.size();
  partition_is.resize(k+1);
  partition_is[0] = 0;
  partition_indices(begin(rng),end(rng), begin(pivots),end(pivots), begin(partition_is)+1, comp);
  partition_is.back() = n;
  return partition_is;
}

template<
  class RA_rng, class RA_pivot_rng,
  class Bin_pred = std::less<>,
  class Return_container = interval_vector<int>
> constexpr auto
partition_point_indices(const RA_rng& rng, const RA_pivot_rng& pivots, Bin_pred comp = {}, Return_container&& partition_is = {}) -> std::vector<int> {
  int n = rng.size();
  int k = pivots.size();
  partition_is.resize(k+1);
  partition_is[0] = 0;
  partition_point_indices(begin(rng),end(rng), begin(pivots),end(pivots), begin(partition_is)+1, comp);
  partition_is.back() = n;
  return partition_is;
}

//template<class RA_rng, class RA_pivot_rng, class Bin_pred = std::less<>> auto
//indirect_partition_sort(const RA_rng& rng, const RA_pivot_rng& pivots, Bin_pred comp = {}) {
//  std::vector<int> new_to_old(rng.size());
//  std::iota(begin(new_to_old),end(new_to_old),0);
//  auto indirect_comp = [&rng,comp](int i, auto&& y){ return comp(rng[i],y); }; // TODO ugly
//  auto partition_is = std_e::partition_sort_indices(new_to_old,pivots,indirect_comp);
//  return std::make_pair(std::move(partition_is),std::move(new_to_old));
//}
//
//template<class RA_rng, class RA_pivot_rng, class Bin_pred = std::less<>> auto
//apply_indirect_partition_sort(RA_rng& rng, const RA_pivot_rng& pivots, Bin_pred comp = {}) {
//  auto res = std_e::indirect_partition_sort(rng,pivots,comp);
//  const auto& new_to_old = res.second;
//
//  permute(rng,new_to_old); // TODO rename permute->inv_permute ?
//  return res;
//}
//
//
//// TODO extract in iterator-level interface
//// TODO if passed list of possible values, use partition_sort, else use std::sort
//// TODO distinguish between cheap and costly swap of T
////    Here, assumed costly -> indirect sort
////    If swap is cheap (e.g. T is built-in), then the sort can be inplace
//// TODO distinguish between cheap and costly "proj"
////    Here, we assume "proj" is costly, so we save the result
////    It "proj" is cheap, (e.g. access to an attribute) then we can compute it on-the-fly
//template<class Range, class F, class T = typename Range::value_type> auto
//sort_into_partitions(Range&& rng, F proj) -> jagged_vector<T> {
//  auto&& compared_values = std_e::transform(rng,proj);
//  auto sort_permutation = std_e::sort_permutation(compared_values);
//
//  auto partition_is = std_e::mismatch_indices(
//    sort_permutation,
//    [&compared_values](int i, int j){ return compared_values[i] == compared_values[j]; }
//  );
//
//  permute(rng,sort_permutation);
//  return {FWD(rng),std::move(partition_is)};
//}


} // std_e
