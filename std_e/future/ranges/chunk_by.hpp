#pragma once


#include "std_e/algorithm/mismatch_points.hpp"
#include "std_e/data_structure/block_range/vblock_range.hpp"
#include "std_e/functional/pipeable.hpp"

namespace std_e {


template<class Range, class Bin_pred> constexpr auto
chunk_by(Range&& rng, Bin_pred p) {
  auto partition_is = std_e::mismatch_indices(rng,p);
  return vblock_range<std_e::remove_rvalue_reference<Range&&>,std::vector<int>>(FWD(rng),std::move(partition_is));
}


template<class Bin_pred> constexpr auto
chunk_by(Bin_pred p) {
  auto chunk_by_fn = [p](auto&& rng){ return chunk_by(FWD(rng),p); };
  return make_pipeable(chunk_by_fn);
}


} // std_e
