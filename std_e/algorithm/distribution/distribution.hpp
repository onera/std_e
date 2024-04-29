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

template<class Integer> using distribution_vector = interval_vector<Integer>;
template<class Integer> using distribution_span = interval_span<Integer>;


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

// TODO move
template<class I> auto
uniform_intervals(int n_interval, I start, I size_tot) -> interval_vector<I> {
  interval_vector<I> iv(n_interval);
  uniform_intervals(begin(iv),end(iv),start,size_tot);
  return iv;
}

template<class I> auto
uniform_distribution(int n_interval, I size_tot) -> distribution_vector<I> { // rename balanced_distribution? (this name exists already when weights provided)
  return uniform_intervals(n_interval, I(0), size_tot);
}

template<class I> auto
uniform_intervals_exclude_ends(int n_interval, I start, I size_tot) -> interval_vector<I> { // rename interior_uniform_distribution?
  interval_vector<I> iv = uniform_intervals(n_interval, start, size_tot);
  iv.as_base().erase(iv.begin());
  iv.as_base().pop_back();
  return iv;
}

template<class I> auto
uniform_distribution_exclude_ends(int n_interval, I size_tot) -> distribution_vector<I> { // rename interior_uniform_distribution?
  return uniform_intervals_exclude_ends(n_interval, I(0), size_tot);
}


} // std_e
