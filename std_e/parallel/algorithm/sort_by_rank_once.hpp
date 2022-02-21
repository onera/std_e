#pragma once


#include "std_e/parallel/algorithm/median_of_3.hpp"
#include "std_e/algorithm/pivot_partition_range.hpp"


namespace std_e {


template<
  class Rng,
  class Proj = identity_closure,
  class Comp = std::less<>,
  class Return_container = interval_vector<int>
> auto
sort_by_rank_once(Rng& x, MPI_Comm comm, Proj proj = {}, Comp comp = {}, Return_container&& = {}) -> Return_container {
  using T = typename Rng::value_type;
  using T_piv = value_return_t<Proj,T>;

  auto size_tot = all_reduce(x.size(),MPI_SUM,comm);
  if (size_tot==0) {
    return Return_container(n_rank(comm),0); // TODO interval_vector size ctor
  }

  std::vector<T_piv> pivots = median_of_3_sample(x,comm,proj);

  return pivot_partition_indices(x,pivots,proj,comp,Return_container{});
}


} // std_e
