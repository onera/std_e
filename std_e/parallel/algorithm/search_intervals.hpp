#pragma once


#include <cmath>
#include <vector>
#include <tuple>


namespace std_e {


inline auto
uniform_partition_point(int sz_tot, int n_interval, int i) -> int {
  return (sz_tot/n_interval) * i;
}


template<class Rng> auto
search_intervals(const Rng& indices, int max_interval_tick_shift) {
  int sz_tot = indices.back();
  int n_interval = indices.size()-1;

  std::vector<int> first_index;
  std::vector<int> n_indices;
  std::vector<int> interval_start;

  int k = 0;
  for (int i=1; i<n_interval; ++i) {
    if (std::abs(indices[i] - uniform_partition_point(sz_tot,n_interval,i)) <= max_interval_tick_shift) continue;

    while (not (indices[k] < uniform_partition_point(sz_tot,n_interval,i) && uniform_partition_point(sz_tot,n_interval,i) < indices[k+1]) ) {
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
search_intervals2(const Rng& indices, int sz_tot, int n_interval, int offset, int max_interval_tick_shift) {
  //int n_interval = indices.size()-1;

  std::vector<int> first_index;
  std::vector<int> n_indices;
  std::vector<int> interval_start;

  int k = 0;
  for (int i=1; i<n_interval; ++i) {
    //if (std::abs(indices[i] - uniform_partition_point(sz_tot,n_interval,i)) <= max_interval_tick_shift) continue;

    while (not ( offset+indices[k] < uniform_partition_point(sz_tot,n_interval,i) && uniform_partition_point(sz_tot,n_interval,i) < offset+indices[k+1]) ) {
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
search_intervals4(const Rng& indices, const std::vector<int>& objective_ticks,int max_interval_tick_shift) {
  int n_interval = indices.size()-1;
  STD_E_ASSERT(n_interval == (int)objective_ticks.size());

  std::vector<int> first_index;
  std::vector<int> n_indices;
  std::vector<int> interval_start;

  int k = 0;
  for (int i=0; i<n_interval; ++i) {
    //if (std::abs(indices[i] - uniform_partition_point(sz_tot,n_interval,i)) <= max_interval_tick_shift) continue;

    while (not (indices[k] < objective_ticks[i] && objective_ticks[i] < indices[k+1]) ) {
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


}
