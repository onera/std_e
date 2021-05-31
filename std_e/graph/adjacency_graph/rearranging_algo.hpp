#pragma once


#include "std_e/graph/adjacency_graph/adjacency_graph.hpp"
#include "std_e/graph/adjacency_graph/adjacency_graph_algo.hpp"
#include "std_e/algorithm/permutation.hpp"


namespace std_e {


template<class T, class I> constexpr auto
propagate_outward_edges(const io_adjacency_graph<T>& x, const std::vector<I>& perm, io_adjacency_graph<T>& g) -> void {
  int initial_sz = x.size();
  int sz = perm.size();
  auto inv_perm = inverse_partial_permutation(perm,initial_sz,-1);

  for (I i=0; i<sz; ++i) {
    for (const auto& old_adj_idx : x[perm[i]].out_indices()) {
      g[i].out_indices().push_back( inv_perm[old_adj_idx] );
    }
  }
}

template<class T, class I> constexpr auto
bidirectional_graph_from_outward_edges(const io_adjacency_graph<T>& x, const std::vector<I>& perm) -> io_adjacency_graph<T> {
  int sz = perm.size();

  io_adjacency_graph<T> res(sz);
  for (int i=0; i<sz; ++i) {
    res[i].node() = x[perm[i]].node();
  }

  propagate_outward_edges(x,perm,res);
  make_bidirectional_from_outward_edges(res);
  return res;
}


} // std_e
