#pragma once


#include <algorithm>


namespace std_e {


// Partition range [first,last) with partition values [piv_first,piv_last)
// The partition points are written in [out_first,k), where k = piv_last-piv_first
template<class RA_it, class RA_it_piv, class RA_it_out, class Comp = std::less<>> constexpr auto
pivot_partition(RA_it first, RA_it last, RA_it_piv piv_first, RA_it_piv piv_last, RA_it_out out_first, Comp comp = {}) -> void {
  if (piv_first==piv_last) return;
  auto k = piv_last-piv_first;
  auto pv_mid = piv_first+k/2;
  auto pp_mid = out_first+k/2;
  *pp_mid = std::partition(first,last,[&mid=*pv_mid,comp](const auto& x){ return comp(x,mid); });
  if (k>1) {
    pivot_partition(first  ,*pp_mid , piv_first,pv_mid  , out_first , comp);
    pivot_partition(*pp_mid,last    , pv_mid+1,piv_last , pp_mid+1 , comp);
  }
}


// Same idea as `std_e::pivot_partition`,
// but `std_e::pivot_partition_point` is to `std_e::pivot_partition`
// what `std::partition_point` is to `std::partition`.
// That is, while the latter does the partition and returns the partition point,
// the former pre-supposes that the range is already partitioned, and returns the partition point.
template<class RA_it, class RA_it_piv, class RA_it_out, class Comp = std::less<>> constexpr auto
pivot_partition_point(RA_it first, RA_it last, RA_it_piv piv_first, RA_it_piv piv_last, RA_it_out out_first, Comp comp = {}) -> void {
  if (piv_first==piv_last) return;
  auto k = piv_last-piv_first;
  auto pv_mid = piv_first+k/2;
  auto pp_mid = out_first+k/2;
  *pp_mid = std::partition_point(first,last,[&mid=*pv_mid,comp](const auto& x){ return comp(x,mid); });
  if (k>1) {
    pivot_partition_point(first  ,*pp_mid , piv_first,pv_mid  , out_first , comp);
    pivot_partition_point(*pp_mid,last    , pv_mid+1,piv_last , pp_mid+1 , comp);
  }
}


// pivot_partition_indices {
//
// Same idea as `std_e::pivot_partition`,
// but elements returned in [out_first,k) are the indices (i.e. integers) of the partition points,
// rather than the partition points (i.e. iterators) themselves
template<class RA_it, class RA_it_piv, class RA_it_out, class Comp, class Proj> constexpr auto
pivot_partition_i_aux(RA_it first, RA_it last, RA_it_piv piv_first, RA_it_piv piv_last, RA_it_out out_first, Comp comp, Proj proj, RA_it start) -> void {
  if (piv_first==piv_last) return;
  auto k = piv_last-piv_first;
  auto pv_mid = piv_first+k/2;
  auto pi_mid = out_first+k/2;
  auto pp_mid = std::partition(first,last,[&mid=*pv_mid,comp,proj](const auto& x){ return comp(proj(x),mid); });
  *pi_mid = pp_mid - start;
  if (k>1) {
    pivot_partition_i_aux(first ,pp_mid , piv_first,pv_mid  , out_first, comp,proj, start);
    pivot_partition_i_aux(pp_mid,last   , pv_mid+1,piv_last , pi_mid+1 , comp,proj, start);
  }
}

template<class RA_it, class RA_it_piv, class RA_it_out, class Comp = std::less<>, class Proj = identity_closure> constexpr auto
pivot_partition_indices(RA_it first, RA_it last, RA_it_piv piv_first, RA_it_piv piv_last, RA_it_out out_first, Comp comp = {}, Proj proj = {}) -> void {
  return pivot_partition_i_aux(first,last,piv_first,piv_last,out_first,comp,proj,first);
}
// pivot_partition_indices }


// pivot_partition_point_indices {
// Same idea as `std_e::pivot_partition_point`, but returns the indices rather than the iterators
template<class RA_it, class RA_it_piv, class RA_it_out, class Comp> constexpr auto
pivot_partition_point_i_aux(RA_it first, RA_it last, RA_it_piv piv_first, RA_it_piv piv_last, RA_it_out out_first, Comp comp, RA_it start) -> void {
  if (piv_first==piv_last) return;
  auto k = piv_last-piv_first;
  auto pv_mid = piv_first+k/2;
  auto pi_mid = out_first+k/2;
  auto pp_mid = std::partition_point(first,last,[&mid=*pv_mid,comp](const auto& x){ return comp(x,mid); });
  *pi_mid = pp_mid - start;
  if (k>1) {
    pivot_partition_point_i_aux(first ,pp_mid , piv_first,pv_mid  , out_first , comp, start);
    pivot_partition_point_i_aux(pp_mid,last   , pv_mid+1,piv_last , pi_mid+1 , comp, start);
  }
}
template<class RA_it, class RA_it_piv, class RA_it_out, class Comp = std::less<>> constexpr auto
pivot_partition_point_indices(RA_it first, RA_it last, RA_it_piv piv_first, RA_it_piv piv_last, RA_it_out out_first, Comp comp = {}) -> void {
  return pivot_partition_point_i_aux(first,last,piv_first,piv_last,out_first,comp,first);
}
// pivot_partition_point_indices }


} // std_e
