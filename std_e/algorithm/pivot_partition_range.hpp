#pragma once

#include <algorithm>
#include "std_e/algorithm/pivot_partition.hpp"
#include "std_e/interval/interval_sequence.hpp"
//#include "std_e/data_structure/jagged_range.hpp"
//#include "std_e/algorithm/mismatch_points.hpp"


namespace std_e {


template<
  class RA_rng, class RA_pivot_rng,
  class Bin_pred = std::less<>,
  class Return_container = interval_vector<typename RA_rng::iterator>
> constexpr auto
pivot_partition(RA_rng& x, const RA_pivot_rng& pivots, Bin_pred comp = {}, Return_container&& partition_pts = {}) -> Return_container {
  int k = pivots.size();
  partition_pts.resize(1+k+1); // also add begin and end for convenience
  partition_pts[0] = begin(x);
  pivot_partition(begin(x),end(x), begin(pivots),end(pivots), begin(partition_pts)+1, comp);
  partition_pts.back() = end(x);
  return partition_pts;
}


template<
  class RA_rng, class RA_pivot_rng,
  class Bin_pred = std::less<>,
  class Return_container = interval_vector<typename RA_rng::const_iterator>
> constexpr auto
pivot_partition_point(const RA_rng& x, const RA_pivot_rng& pivots, Bin_pred comp = {}, Return_container&& partition_pts = {}) -> Return_container {
  int k = pivots.size();
  partition_pts.resize(1+k+1); // also add begin and end for convenience
  partition_pts[0] = begin(x);
  pivot_partition_point(begin(x),end(x), begin(pivots),end(pivots), begin(partition_pts)+1, comp);
  partition_pts.back() = end(x);
  return partition_pts;
}


template<
  class RA_rng, class RA_pivot_rng,
  class Bin_pred = std::less<>,
  class Return_container = interval_vector<int>
> constexpr auto
pivot_partition_indices(RA_rng& x, const RA_pivot_rng& pivots, Bin_pred comp = {}, Return_container&& partition_is = {}) -> Return_container {
  int n = x.size();
  int k = pivots.size();
  partition_is.resize(1+k+1); // also add begin and end for convenience
  partition_is[0] = 0;
  pivot_partition_indices(begin(x),end(x), begin(pivots),end(pivots), begin(partition_is)+1, comp);
  partition_is.back() = n;
  return partition_is;
}


template<
  class RA_rng, class RA_pivot_rng,
  class Bin_pred = std::less<>,
  class Return_container = interval_vector<int>
> constexpr auto
pivot_partition_point_indices(const RA_rng& x, const RA_pivot_rng& pivots, Bin_pred comp = {}, Return_container&& partition_is = {}) -> Return_container {
  int n = x.size();
  int k = pivots.size();
  partition_is.resize(1+k+1); // also add begin and end for convenience
  partition_is[0] = 0;
  pivot_partition_point_indices(begin(x),end(x), begin(pivots),end(pivots), begin(partition_is)+1, comp);
  partition_is.back() = n;
  return partition_is;
}

//template<class RA_rng, class RA_pivot_rng, class Bin_pred = std::less<>> auto
//indirect_partition_sort(const RA_rng& x, const RA_pivot_rng& pivots, Bin_pred comp = {}) {
//  std::vector<int> new_to_old(x.size());
//  std::iota(begin(new_to_old),end(new_to_old),0);
//  auto indirect_comp = [&x,comp](int i, auto&& y){ return comp(x[i],y); }; // TODO ugly
//  auto partition_is = std_e::partition_sort_indices(new_to_old,pivots,indirect_comp);
//  return std::make_pair(std::move(partition_is),std::move(new_to_old));
//}
//
//template<class RA_rng, class RA_pivot_rng, class Bin_pred = std::less<>> auto
//apply_indirect_partition_sort(RA_rng& x, const RA_pivot_rng& pivots, Bin_pred comp = {}) {
//  auto res = std_e::indirect_partition_sort(x,pivots,comp);
//  const auto& new_to_old = res.second;
//
//  permute(x,new_to_old); // TODO rename permute->inv_permute ?
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
//sort_into_partitions(Range&& x, F proj) -> jagged_vector<T> {
//  auto&& compared_values = std_e::transform(x,proj);
//  auto sort_permutation = std_e::sort_permutation(compared_values);
//
//  auto partition_is = std_e::mismatch_indices(
//    sort_permutation,
//    [&compared_values](int i, int j){ return compared_values[i] == compared_values[j]; }
//  );
//
//  permute(x,sort_permutation);
//  return {FWD(x),std::move(partition_is)};
//}


} // std_e
