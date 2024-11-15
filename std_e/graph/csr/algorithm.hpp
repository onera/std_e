#pragma once


#include "std_e/algorithm/block.hpp"
#include "std_e/algorithm/permutation.hpp"
#include "std_e/data_structure/multi_range/multi_range.hpp"


namespace std_e {


auto
renumber_graph(auto& graph_idx, auto& graph, const auto& new_to_old_numbering) -> void {
  // 1. rearrange adjacencies according to the new numbering
  std_e::permute_by_block(graph_idx, graph, new_to_old_numbering);

  // 2. then renumber the nodes
  std_e::dynarray<int> old_to_new_numbering = inverse_permutation(new_to_old_numbering);
  for (auto& node : graph) { node = old_to_new_numbering[node]; }
}


auto
renumber_edge(auto& edge_node, const auto& new_to_old_numbering) -> void
{
  auto n_edge = edge_node.size();
  auto edge_node_l = std_e::range<0>(edge_node);
  auto edge_node_r = std_e::range<1>(edge_node);
  std_e::permute(edge_node_l, new_to_old_numbering);
  std_e::permute(edge_node_r, new_to_old_numbering);
}


} // std_e
