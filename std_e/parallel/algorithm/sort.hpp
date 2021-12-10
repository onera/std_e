#pragma once


#include "std_e/parallel/all_to_all.hpp"
#include "std_e/algorithm/partition_sort.hpp"
#include "std_e/algorithm/distribution.hpp"
#include "std_e/parallel/struct/distribution.hpp"
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
partition_sort2__indirect_swaps(std::vector<T>& x, int sz_tot, Comp comp, MPI_Comm comm) {
  if (x.size()==0) throw;
  // partition_sort by first vertex (this is a partial sort, but good enought so we can exchange based on that)
  //auto _0 = std_e::stdout_time_logger("  partition sort");
    auto less_first_vertex = [&x](int i, const double& y){ return x[i] < y; }; // TODO ugly (non-symmetric)
    T med = median_of_3(x[0],x[x.size()/2],x.back());
    std::vector<T> meds = all_gather(med,comm);
    std::sort(begin(meds),end(meds));
    std::vector<int> perm(x.size());
    std::iota(begin(perm),end(perm),0);
    auto partition_indices = partition_sort_indices(perm,meds,less_first_vertex);
    partition_indices.back() = x.size(); // for the last median, also take all the ones above
    ELOG(partition_indices);
    permute(x.begin(),perm);
  //_0.stop();


  // exchange
  //auto _1 = std_e::stdout_time_logger("  exchange");
    jagged_span<T> x_by_rank(x,partition_indices);
    auto res_connec = all_to_all_v(x_by_rank,comm);
    std::vector<T> res_connec2 = res_connec.retrieve_values();
  //_1.stop();
  return res_connec2;
}


} // std_e
