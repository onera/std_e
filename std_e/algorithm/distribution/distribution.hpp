#pragma once


#include "std_e/interval/interval_sequence.hpp"
#include "std_e/algorithm/distribution/uniform.hpp"
#include <vector>


namespace std_e {


/*
concept Distribution : std_e::Interval_sequence (means the values are ordered)
  - value_type is Integer
  - the interval starts at 0
*/

template<class Integer> using distribution_vector = std_e::interval_vector<Integer>;
template<class Integer> using distribution_span = std_e::interval_span<Integer>;


// Distribution vocabulary around interval_sequence functions {
// TODO rename
// split_global_index   | split_gi   | split_gidx
// rank_of_global_index | rank_of_gi | rank_of_gidx
// to_global_index      | to_gi      | to_gidx
template<class Integer, class Distribution> auto
search_rank(Integer i, const Distribution& dist) { // TODO rename global_index_rank, rank_of_gi, gi_rank
  return std_e::interval_index(i,dist);
}
template<class Integer, class Distribution> auto
rank_offset(Integer i, const Distribution& dist) -> std::pair<int,Integer> { // TODO rename rank_and_local_index? split_global_index global_index_split, 
  int rank = search_rank(i,dist);
  auto offset = i - dist[rank];
  return {rank,offset};
}
template<class Random_access_range, class I = typename Random_access_range::value_type> auto
distribution_from_sizes(const Random_access_range& r) -> distribution_vector<I> {
  return std_e::indices_from_strides(r);
}
// Distribution vocabulary around interval_sequence functions }


template<class I> auto
uniform_distribution(int n_interval, I size_tot) -> distribution_vector<I> { // rename balanced_distribution? (this name exists already when weights provided)
  distribution_vector<I> distrib(n_interval);
  uniform_distribution(begin(distrib),end(distrib),size_tot);
  return distrib;
}

template<class I> auto
uniform_distribution_exclude_ends(int n_interval, I size_tot) -> distribution_vector<I> { // rename interior_uniform_distribution?
  auto distrib = uniform_distribution(n_interval,size_tot);
  distrib.as_base().erase(distrib.begin());
  distrib.as_base().pop_back();
  return distrib;
}


} // std_e
