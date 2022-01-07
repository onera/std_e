#pragma once


#include <cmath>
#include <vector>
#include <tuple>
#include "std_e/algorithm/distribution/uniform.hpp"
#include "std_e/log.hpp" // TODO


namespace std_e {


template<class Rng> auto
search_intervals6(const Rng& indices, const std::vector<int>& objective_ticks, int max_interval_tick_shift) {
  int sz_tot = indices.back();
  int n_tick = indices.size()-1;

  std::vector<int> first_index;
  std::vector<int> n_indices;
  std::vector<int> interval_start;

  int k = 0;
  for (int i=1; i<n_tick; ++i) {
    if (std::abs(indices[i] - objective_ticks[i-1]) <= max_interval_tick_shift) continue;

    while (not (indices[k] < objective_ticks[i-1] && objective_ticks[i-1] < indices[k+1]) ) {
      ++k;
    }
    if (interval_start.size()>0 && interval_start.back()==k) {
      ++n_indices.back();
    } else {
      first_index.push_back(i);
      n_indices.push_back(1);
      interval_start.push_back(k);
    }
  }

  return std::make_tuple(first_index, n_indices, interval_start);
}

template<class Rng> auto
search_intervals4(const Rng& indices, const std::vector<int>& objective_ticks, int max_interval_tick_shift) {
  int n_tick = objective_ticks.size();

  std::vector<int> first_index;
  std::vector<int> n_indices;
  std::vector<int> interval_start;

  std::vector<int> index_ticks_found;

  int k = 0;
  for (int i=0; i<n_tick; ++i) {
    STD_E_ASSERT(indices[0] <= objective_ticks[i] && objective_ticks[i] < indices.back());

    while (not (indices[k] <= objective_ticks[i] && objective_ticks[i] < indices[k+1]) ) {
      ++k;
    }
    // ticks[i] is in [indices[k],indices[k+1])

    // if indices[k] is close enought, put it in the ticks found
    if (std::abs(indices[k] - objective_ticks[i]) < max_interval_tick_shift) {
      index_ticks_found.push_back(k);

    // else, register the fact that ticks[i] is to be found in [indices[k],indices[k+1])
    } else {
      if (interval_start.size()>0 && interval_start.back()==k) {
        ++n_indices.back();
      } else {
        first_index.push_back(i);
        n_indices.push_back(1);
        interval_start.push_back(k);
      }
    }
  }

  return std::make_tuple(first_index, n_indices, interval_start, index_ticks_found);
}


} // std_e
