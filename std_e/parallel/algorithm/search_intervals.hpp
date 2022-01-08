#pragma once


#include <cmath>
#include <vector>
#include <tuple>
#include "std_e/algorithm/distribution/uniform.hpp"
#include "std_e/log.hpp" // TODO


namespace std_e {


template<class Rng> auto
search_intervals6(const std::vector<int>& ticks, const Rng& inter, int max_interval_tick_shift) {
  int n_tick = ticks.size();

  std::vector<int> first_ticks;
  std::vector<int> n_indices;
  std::vector<int> inter_indices;
  std::vector<int> index_ticks_found;

  int k = 0;
  for (int i=0; i<n_tick; ++i) {
    STD_E_ASSERT(inter[0] <= ticks[i] && ticks[i] < inter.back());
    if (std::abs(inter[i+1] - ticks[i]) <= max_interval_tick_shift) continue;

    while (not (inter[k] < ticks[i] && ticks[i] < inter[k+1]) ) {
      ++k;
    }
    // now, ticks[i] is in [inter[k],inter[k+1])

    // if inter[k] is close enought, put it in the ticks found
    if (std::abs(inter[k] - ticks[i]) < max_interval_tick_shift) {
      index_ticks_found.push_back(k);
    }

    // else, register the fact that ticks[i] is to be found in [inter[k],inter[k+1])
    if (inter_indices.size()>0 && inter_indices.back()==k) {
      ++n_indices.back();
    } else {
      first_ticks.push_back(i+1);
      n_indices.push_back(1);
      inter_indices.push_back(k);
    }
  }

  return std::make_tuple(first_ticks, n_indices, inter_indices);
}

// TODO invert args
template<class Rng> auto
search_intervals4(const std::vector<int>& ticks, const Rng& inter, int max_interval_tick_shift) {
  int n_tick = ticks.size();

  std::vector<int> first_ticks;
  std::vector<int> n_indices;
  std::vector<int> inter_indices;
  std::vector<int> index_ticks_found;

  int k = 0;
  for (int i=0; i<n_tick; ++i) {
    STD_E_ASSERT(inter[0] <= ticks[i] && ticks[i] < inter.back());

    while (not (inter[k] <= ticks[i] && ticks[i] < inter[k+1]) ) {
      ++k;
    }
    // now, ticks[i] is in [inter[k],inter[k+1])

    // if inter[k] is close enought, put it in the ticks found
    //ELOG(std::abs(inter[k] - ticks[i]));
    if (std::abs(inter[k] - ticks[i]) < max_interval_tick_shift) {
      index_ticks_found.push_back(k);

    // else, register the fact that ticks[i] is to be found in [inter[k],inter[k+1])
    } else {
      if (inter_indices.size()>0 && inter_indices.back()==k) {
        ++n_indices.back();
      } else {
        first_ticks.push_back(i);
        n_indices.push_back(1);
        inter_indices.push_back(k);
      }
    }
  }

  return std::make_tuple(first_ticks, n_indices, inter_indices, index_ticks_found);
}


template<class Interval_range0, class Interval_range1, class I = typename Interval_range0::value_type> auto
search_intervals7(const Interval_range0& ticks, const Interval_range1& inter) {
  STD_E_ASSERT(inter.size()>=2);
  if (ticks.size()>0) {
    STD_E_ASSERT(inter[0] <= ticks[0] && ticks.back() < inter.back());
  }

  I n_tick = ticks.size();

  std::vector<I> inter_indices;
  std::vector<I> first_ticks;

  I k = 0;
  for (I i=0; i<n_tick; ++i) {

    while (not (inter[k] <= ticks[i] && ticks[i] < inter[k+1]) ) {
      ++k;
    }

    // register the fact that ticks[i] is to be found in [inter[k],inter[k+1])

    // if ticks[i-1] was also in [inter[k],inter[k+1]),
    // just increment the number of
    if (inter_indices.size()>0 && inter_indices.back()==k) {

    } else {
      first_ticks.push_back(i);
      inter_indices.push_back(k);
    }
  }
  first_ticks.push_back(n_tick);
  inter_indices.push_back(k+1);

  return std::make_tuple(first_ticks, inter_indices);
}


} // std_e
