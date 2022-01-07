#pragma once


#include "std_e/algorithm/distribution.hpp"
#include "std_e/interval/interval_sequence.hpp"


namespace std_e {


template<class Integer> using distribution_vector = std_e::interval_vector<Integer>;


template<class I> auto
uniform_distribution(int size, I nb_elts) -> distribution_vector<I> {
  distribution_vector<I> distrib(size);
  uniform_distribution(begin(distrib),end(distrib),nb_elts);
  return distrib;
}

template<class I> auto
uniform_distribution_exclude_ends(int size, I nb_elts) -> distribution_vector<I> {
  auto distrib = uniform_distribution(size,nb_elts);
  distrib.as_base().erase(distrib.begin());
  distrib.as_base().pop_back();
  return distrib;
}


template<class Integer, class Distribution> auto
rank_offset(Integer i, const Distribution& dist) -> std::pair<int,Integer> {
  int rank = std_e::interval_index(i,dist);
  auto offset = i - dist[rank];
  return {rank,offset};
}


} // std_e
