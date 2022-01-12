#pragma once


#include "std_e/interval/interval_sequence.hpp"
#include "std_e/parallel/algorithm/uniform_sample.hpp"
#include "std_e/algorithm/distribution/uniform.hpp"
#include "std_e/plog.hpp" // TODO


namespace std_e {


template<class I>
struct interval_containing_ticks {
  I inf;
  I sup;
  int n_tick;
  int position;
};

template<class I> auto
to_string(const interval_containing_ticks<I>& x) {
  return
      "["+std::to_string(x.inf)+','+std::to_string(x.sup)+"):"
    + std::to_string(x.n_tick)+':'+std::to_string(x.position);
};


template<class I>
auto objective_ticks(I sz_tot, I n_interval, I position, I n_tick) -> std::vector<I> {
  std::vector<I> ticks(n_tick);
  for (I i=0; i<n_tick; ++i) {
    ticks[i] = uniform_distribution_point(sz_tot,n_interval,position+i+1); // +1, because we want to exclude tick 0
  }
  return ticks;
}


template<class I> auto
compute_intervals_containing_ticks(const auto& ticks, const auto& partition_indices_sub, I max_interval_tick_shift, MPI_Comm comm) {
  auto partition_indices_sub_tot = all_reduce(partition_indices_sub.as_base(),MPI_SUM,comm);

  auto [far_first_ticks,n_far_ticks,far_inter_indices, near_tick_indices,near_inter_indices] = search_near_or_containing_interval(ticks,partition_indices_sub_tot,max_interval_tick_shift);

  int position_offset = 0;
  int n_far_interval = far_inter_indices.size();
  std::vector<interval_containing_ticks<I>> sub_ins(n_far_interval);
  for (int i=0; i<n_far_interval; ++i) {
    int position_i = position_offset+far_first_ticks[i];
    I inf = partition_indices_sub[far_inter_indices[i]];
    I sup = partition_indices_sub[far_inter_indices[i] + 1];
    sub_ins[i] = {inf,sup,n_far_ticks[i],position_i};
  }
  return sub_ins;
}


// TODO facto with compute_intervals_containing_ticks
template<class I> auto
compute_interval_containing_ticks2(const auto& ticks, const auto& partition_indices_sub, I max_interval_tick_shift, auto position_offset, auto& partition_indices, MPI_Comm comm) {
  auto partition_indices_sub_tot = all_reduce(partition_indices_sub.as_base(),MPI_SUM,comm); // TODO outside of loop

  auto [far_first_ticks,n_far_ticks,far_inter_indices, near_tick_indices,near_inter_indices] = search_near_or_containing_interval2(ticks,partition_indices_sub_tot,max_interval_tick_shift);

  // 1.2. create new sub-intervals
  int n_far_interval = far_inter_indices.size();
  std::vector<interval_containing_ticks<I>> sub_ins(n_far_interval);
  for (int i=0; i<n_far_interval; ++i) {
    int position_i = position_offset+far_first_ticks[i];
    I inf = partition_indices_sub[far_inter_indices[i]];
    I sup = partition_indices_sub[far_inter_indices[i] + 1];
    sub_ins[i] = {inf,sup,n_far_ticks[i],position_i};
  }

  // 1.3. report found ticks
  int n_near_interval = near_inter_indices.size();
  for (int i=0; i<n_near_interval; ++i) {
    int k = near_inter_indices[i];
    int position_i = 1+position_offset+near_tick_indices[i]; // 1+ because we are not using position 0 where partition_indices[0]==0
    partition_indices[position_i] = partition_indices_sub[k];
  }

  return sub_ins;
}


} // std_e
