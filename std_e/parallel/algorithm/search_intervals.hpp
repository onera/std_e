#pragma once


#include <cmath>
#include <vector>
#include <tuple>


namespace std_e {


auto
uniform_partition_point(int sz_tot, int n_interval, int i) -> int {
  return (sz_tot/n_interval) * i;
}


template<class Rng> auto
search_intervals(const Rng& indices, double max_rel_bound_error) {
  int sz_tot = indices.back();
  int n_interval = indices.size()-1;

  int uniform_interval_len = sz_tot/n_interval;

  std::vector<int> first_index;
  std::vector<int> n_indices;
  std::vector<int> interval_start;

  int k = 0;
  for (int i=1; i<n_interval; ++i) {
    if (std::abs(indices[i] - uniform_partition_point(sz_tot,n_interval,i)) / double(uniform_interval_len) < max_rel_bound_error) continue;

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


}
