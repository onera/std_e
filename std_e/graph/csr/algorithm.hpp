#pragma once


#include "std_e/algorithm/block.hpp"
#include "std_e/algorithm/permutation.hpp"


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
  auto n_face = edge_node.size()/2;
  auto edge_node_l = std_e::make_span(edge_node.data()       ,   n_face);
  auto edge_node_r = std_e::make_span(edge_node.data()+n_face, 2*n_face);
  std_e::permute(edge_node_l, new_to_old_numbering);
  std_e::permute(edge_node_r, new_to_old_numbering);
}


} // std_e
