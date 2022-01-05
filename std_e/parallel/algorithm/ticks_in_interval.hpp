#pragma once


#include "std_e/parallel/mpi.hpp"
#include "std_e/interval/interval_sequence.hpp"
#include "std_e/algorithm/distribution/ticks_in_interval.hpp"


namespace std_e {


template<class T> auto
median_of_3_sample(span<T> x, int n, MPI_Comm comm) {
  //int rk = rank(comm);
  //int n_rk = n_rank(comm);
  int n_sample = 3*n; // *3 by analogy with median of 3

  int x_start_glob = ex_scan(x.size(),MPI_SUM,0,comm);
  int x_finish_glob = x_start_glob + x.size();
  int x_size_glob = all_reduce(x.size(),MPI_SUM,comm);

  //SLOG(comm,x_start_glob);
  //SLOG(comm,x_start_glob+x.size());
  //SLOG(comm,x_size_glob);
  std::vector<int> sample_indices = uniform_ticks_in_sub_interval(x_start_glob,x_finish_glob,x_size_glob,n_sample);
  //SLOG(comm,sample_indices);
  int n_sample_local = sample_indices.size();

  std::vector<T> local(n_sample_local);
  for (int i=0; i<n_sample_local; ++i) {
    local[i] = x[ sample_indices[i]-x_start_glob ];
  }
  //ELOG(local);

  std::vector<T> sample = all_gather(local,comm); // could be optimized because can be pre-allocated at n_sample
  std::sort(begin(sample),end(sample));
  //ELOG(sample.size());
  //ELOG(n_sample);
  STD_E_ASSERT(int(sample.size())==n_sample);

  std::vector<T> pivots(n);
  for (int i=0; i<n; ++i) {
    pivots[i] = sample[3*i+1];
  }
  return pivots;

}
template<class T> auto
median_of_3_sample(std::vector<span<T>>& xs, std::vector<int> ns, MPI_Comm comm) {
  // TODO implement with only one gather
  STD_E_ASSERT(xs.size()==ns.size());
  int n_interval = xs.size();
  //int rk = rank(comm);
  //int n_rk = n_rank(comm);

  std::vector<std::vector<T>> pivots_by_sub_intervals(n_interval);
  for (int i=0; i<n_interval; ++i) {
    pivots_by_sub_intervals[i] = median_of_3_sample(xs[i],ns[i],comm);
  }
  return pivots_by_sub_intervals;

  //std::vector<int> ranks_with_sample;
  //std::vector<int> n_sample_by_rank;
  //std::vector<int> sub_interval_index;
  //std::vector<int> sample_indices;

  //for (int i=0; i<n_interval; ++i) {
  //  int n_sample = ns[i] * 3; // *3 by analogy with median of 3

  //}
}


struct interval_to_partition {
  int inf;
  int sup;
  int n_ticks;
  int position;

  int inf_tot;
  int sup_tot;

  int sz_tot;
  int n_interval;
  auto objective_tick(int i_tick) const {
    STD_E_ASSERT(i_tick<n_ticks);
    return uniform_partition_point(sz_tot,n_interval,position+i_tick);
  }

};
inline auto
to_string(const interval_to_partition& x) {
  return '['+std::to_string(x.inf)+','+std::to_string(x.sup)+"):"+std::to_string(x.n_ticks)+':'+std::to_string(x.position);
}


inline auto
search_intervals3(const interval_vector<int>& partition_indices, int max_interval_tick_shift, MPI_Comm comm) {
  auto partition_indices_tot = all_reduce(partition_indices.as_base(),MPI_SUM,comm);

  //SLOG(comm,partition_indices_tot);
  int n_interval = n_rank(comm);
  int sz_tot = partition_indices_tot.back();

  auto [first_indices, n_indices, interval_start] = search_intervals(partition_indices_tot,max_interval_tick_shift);

  int n_sub_intervals = interval_start.size();
  std::vector<interval_to_partition> sub_ins(n_sub_intervals);
  for (int i=0; i<n_sub_intervals; ++i) {
    int inf = partition_indices[interval_start[i]];
    int sup = partition_indices[interval_start[i] + 1];
    int inf_tot = partition_indices_tot[interval_start[i]];
    int sup_tot = partition_indices_tot[interval_start[i] + 1];
    sub_ins[i] = {inf,sup,n_indices[i],first_indices[i], inf_tot,sup_tot,sz_tot,n_interval};
  }
  return sub_ins;
}


class par_algo_exception : msg_exception {
  using base = msg_exception;
  using base::base;
};


} // std_e
