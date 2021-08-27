#pragma once


#include "std_e/parallel/struct/distributed_array.hpp"
#include "std_e/parallel/mpi/one_sided/communication.hpp"


namespace std_e {


template<class Integer, class Distribution> auto
rank_offset(Integer i, const Distribution& dist) -> std::pair<int,Integer> {
  int rank = std_e::interval_index(i,dist);
  auto offset = i - dist[rank];
  return {rank,offset};
}

template<class T> auto
get(const distributed_array<T>& a, MPI_Aint i) -> T {
  auto [rank,offset] = rank_offset(i,a.distribution());
  return get(a.win(),rank,offset);
}

//template<class T> auto
//remote_values_from_block(distributed_array<T>& d_array, int rank, const std::vector<int>& idx) -> std::vector<T> {
//  upcxx::global_ptr<T> remote_blk = d_array.fetch(rank).wait(); 
//
//  int sz = idx.size();
//  std::vector<upcxx::global_ptr<T>> remote_ptrs(sz);
//  for (int i=0; i<sz; ++i) {
//    remote_ptrs[i] = remote_blk + idx[i];
//  }
//  std::vector<T> res(sz);
//  std::vector<T*> res_ptr(sz);
//  for (int i=0; i<sz; ++i) {
//    res_ptr[i] = &res[i];
//  }
//  upcxx::rget_regular(remote_ptrs.data(),remote_ptrs.data()+sz,1,res_ptr.data(),res_ptr.data()+sz,1).wait();
//  return res;
//}
//
//
//template<class T> auto
//remote_values_from_blocks(distributed_array<T>& d_array, const std::vector<int>& ranks, const std::vector<std::vector<int>>& idx_by_rank) -> std::vector<T> {
//
//  int n_rank = ranks.size(); 
//  int tot_sz = 0;
//  for (int k=0; k<n_rank; ++k) {
//    tot_sz += idx_by_rank[k].size();
//  }
//  std::vector<T> res(tot_sz);
//
//  int current_tot_sz = 0;
//
//  for (int k=0; k<n_rank; ++k) {
//    auto& idx = idx_by_rank[k];
//    int sz = idx.size();
//    int rank = ranks[k];
//    upcxx::global_ptr<T> remote_blk = d_array.fetch(rank).wait(); 
//    std::vector<upcxx::global_ptr<T>> remote_ptrs(sz);
//    for (int i=0; i<sz; ++i) {
//      remote_ptrs[i] = remote_blk + idx[i];
//    }
//    std::vector<T*> res_ptr(sz);
//    for (int i=0; i<sz; ++i) {
//      res_ptr[i] = &res[current_tot_sz+i];
//    }
//    upcxx::rget_regular(remote_ptrs.data(),remote_ptrs.data()+sz,1,res_ptr.data(),res_ptr.data()+sz,1).wait();
//    current_tot_sz += sz;
//  }
//  return res;
//}
//
//
//template<class T> auto
//block_to_part(distributed_array<T>& d_array, const std::vector<g_num>& distri, const std::vector<g_num>& ln_to_gn) -> std::vector<T> {
//  int n_elt = ln_to_gn.size();
//
//  int n_rank = distri.size() - 1;
//
//  std::vector<int> ranks(n_rank);
//  std::iota(begin(ranks),end(ranks),0);
//
//  std::vector<std::vector<int>> indices(n_rank);
//  std::vector<int> gn_ranks(n_elt);
//  std::vector<int> gn_idxs(n_elt);
//
//  for (int i=0; i<n_elt; ++i) {
//    int gn = ln_to_gn[i];
//    auto it = upper_bound(begin(distri),end(distri),gn);
//    auto gn_rank = it-begin(distri)-1;
//    gn_ranks[i] = gn_rank;
//    int gn_idx = gn - *(it-1);
//    indices[gn_rank].push_back(gn_idx);
//    gn_idxs[i] = indices[gn_rank].size()-1;
//  }
//
//  auto btp_res = remote_values_from_blocks(d_array,ranks,indices);
//
//  std::vector<int> sizes_by_rank(n_rank);
//  int size_by_rank = 0;
//  for (int k=0; k<n_rank; ++k) {
//    sizes_by_rank[k] = size_by_rank;
//    size_by_rank += indices[k].size();
//  }
//
//  std::vector<T> res(n_elt);
//  for (int i=0; i<n_elt; ++i) {
//    int start_rank_pos = sizes_by_rank[gn_ranks[i]];
//    int pos = start_rank_pos+gn_idxs[i];
//    res[i] = btp_res[pos];
//  }
//
//  return res;
//}


} // std_e
