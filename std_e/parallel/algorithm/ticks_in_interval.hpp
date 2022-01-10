#pragma once


#include "std_e/interval/interval_sequence.hpp"
#include "std_e/parallel/algorithm/uniform_sample.hpp"
#include "std_e/algorithm/distribution/uniform.hpp"
#include "std_e/plog.hpp" // TODO


namespace std_e {


struct interval_to_partition {
  int inf;
  int sup;
  int n_ticks;
  int position;

  // From here, we should be able to DEL everything
  int inf_tot;
  int sup_tot;

  int sz_tot;
  int n_interval;
  auto objective_tick(int i_tick) const {
    STD_E_ASSERT(i_tick<n_ticks);
    return uniform_distribution_point(sz_tot,n_interval,position+i_tick);
  }

};
//inline auto
//to_string(const interval_to_partition& x) {
//  return
//      "Glob["+std::to_string(x.inf_tot)+','+std::to_string(x.sup_tot)+"):"
//    + "Loc[" +std::to_string(x.inf    )+','+std::to_string(x.sup    )+"):"
//    + std::to_string(x.n_ticks)+':'+std::to_string(x.position);
//}
constexpr auto
to_string_loc = [](const interval_to_partition& x) {
  return
      "["+std::to_string(x.inf)+','+std::to_string(x.sup)+"):"
    + std::to_string(x.n_ticks)+':'+std::to_string(x.position);
};
inline auto
to_string(const interval_to_partition& x) {
  return
      "["+std::to_string(x.inf_tot)+','+std::to_string(x.sup_tot)+"):"
    + std::to_string(x.n_ticks);
}

inline auto
search_intervals3(const interval_vector<int>& partition_indices, int max_interval_tick_shift, MPI_Comm comm) {
  auto partition_indices_tot = all_reduce(partition_indices.as_base(),MPI_SUM,comm);

  //SLOG(comm,partition_indices_tot);
  int n_interval = n_rank(comm);
  int sz_tot = partition_indices_tot.back();

  //auto distri = uniform_distribution_exclude_ends(n_interval,sz_tot);
  auto distri = uniform_distribution_exclude_ends(n_interval,sz_tot);
  //SLOG(comm,distri.as_base());

  auto [first_indices, n_indices, interval_start] = search_intervals6(distri.as_base(),partition_indices_tot,max_interval_tick_shift);
  //auto [first_indices2, n_indices2, interval_start2,_] = search_intervals8(distri.as_base(),partition_indices_tot,max_interval_tick_shift); // TODO use that one
  //SLOG(comm,first_indices2);
  //SLOG(comm,first_indices);
  //SLOG(comm,n_indices2);
  //SLOG(comm,n_indices);
  //SLOG(comm,interval_start2);
  //SLOG(comm,interval_start);

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


} // std_e
