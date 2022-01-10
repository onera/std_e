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
pivot_partition_once(std::vector<T>& x, MPI_Comm comm, Comp comp = {}, Proj proj = {}, Return_container&& partition_is = {}) -> Return_container {
  STD_E_ASSERT(x.size() > 0);

  std::vector<T> pivots = median_of_3_sample(x,comm);
  //if (rank(comm)==0) {
  //  ELOG(pivots);
  //}

  //return partition_sort_indices(x,pivots,comp);
  return pivot_partition_indices(x,pivots,comp,std::move(partition_is));
}


} // std_e
