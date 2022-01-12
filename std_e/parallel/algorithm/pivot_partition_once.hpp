#pragma once


#include "std_e/parallel/algorithm/median_of_3.hpp"
#include "std_e/algorithm/pivot_partition_range.hpp"


namespace std_e {


template<
  class T,
  class Comp = std::less<>,
  class Proj = identity_closure,
  class Return_container = interval_vector<int>
> auto
pivot_partition_once(std::vector<T>& x, MPI_Comm comm, Comp comp = {}, Proj proj = {}, Return_container&& = {}) -> Return_container {
  auto size_tot = all_reduce(x.size(),MPI_SUM,comm);
  if (size_tot==0) {
    return Return_container(n_rank(comm)+1,0);
  }

  std::vector<T> pivots = median_of_3_sample(x,comm);

  return pivot_partition_indices(x,pivots,comp,Return_container{});
}


} // std_e
