#pragma once


#include "std_e/parallel/all_to_all.hpp"
#include "std_e/algorithm/partition_sort.hpp"
#include "std_e/algorithm/distribution.hpp"
#include "std_e/parallel/struct/distribution.hpp"
#include "std_e/parallel/algorithm/search_intervals.hpp"
#include "std_e/log.hpp"


namespace std_e {


template<class T> auto
median_of_3(const T& a, const T& b, const T& c) -> T {
  if (a < b)
    if (b < c) return b;
    else if (a < c) return c;
    else return a;
  else if (a < c) return a;
  else if (b < c) return c;
  else return b;
}

template<class T, class Comp> auto
partition_sort2__indirect_swaps(std::vector<T>& x, int sz_tot, Comp comp, MPI_Comm comm, double max_imbalance) -> std::vector<T> {
  STD_E_ASSERT(0. <= max_imbalance && max_imbalance < 1.);
  if (x.size()==0) return {};
  int n_rk = n_rank(comm);
  auto less_first_vertex = [&x](int i, const double& y){ return x[i] < y; }; // TODO ugly (non-symmetric)
  // TODO use comp
  // 0. prepare partitions
  //auto _0 = std_e::stdout_time_logger("  partition sort");
    // 0.0 take a sample for partition pivots
    T local_pivot = x.back(); // optimal for already sorted data
    std::vector<T> pivots = all_gather(local_pivot,comm);
    std::sort(begin(pivots),end(pivots));

    // 0.1 partition_sort based on these pivots
    std::vector<int> perm(x.size()); // TODO int -> ?
    std::iota(begin(perm),end(perm),0);
    auto partition_indices = partition_sort_indices(perm,pivots,less_first_vertex);
    /// partition_indices[0]=0 (post condition of partition_sort_indices)
    /// but we need to impose last the index
    partition_indices.back() = x.size();

    auto partition_indices_tot = all_reduce(partition_indices,MPI_SUM,comm);
    // 0.2 maybe partition_indices[i] may be further from uniform_partition_point(i) than partition_indices[i+-k]
    //     so correct if it is the case (can be interpreted as a merge of partitions with not enought elements)
    int k = 0;
    for (int i=0; i<n_rk; ++i) {
      auto uniform_part_index = uniform_partition_point(sz_tot,n_rk,i);
      while ( std::abs(partition_indices_tot[i]-uniform_part_index)
            > std::abs(partition_indices_tot[k]-uniform_part_index) ) {
        ++k
      }
    }


    ELOG(partition_indices);
    auto partition_szs = interval_lengths(partition_indices);
    ELOG(partition_szs);
    all_reduce_inplace(partition_szs,MPI_SUM,comm);

    auto uniform_block_size = sz_tot/n_rank(comm);
    std::
    for (const auto& partition_sz : partition_szs) {
      double imbalance = (partition_sz - uniform_block_size)/double(uniform_block_size);
      ELOG(imbalance);
      if (imbalance > max_imbalance) {

      }
    }
    ELOG(partition_szs);
  //_0.stop();

    permute(x.begin(),perm);

  // 1. exchange
  //auto _1 = std_e::stdout_time_logger("  exchange");
    jagged_span<T> x_by_rank(x,partition_indices);
    auto res_connec = all_to_all_v(x_by_rank,comm);
    std::vector<T> res_connec2 = res_connec.retrieve_values();
  //_1.stop();
  return res_connec2;
}


} // std_e
