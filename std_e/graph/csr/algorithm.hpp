#pragma once


#include "std_e/algorithm/block.hpp"
#include "std_e/algorithm/permutation.hpp"


namespace std_e {


auto
renumber_graph(auto& graph_idx, auto& graph, const auto& new_to_old_numbering) -> void {
  // 1. rearrange adjacencies according to the new numbering
  std::tie(graph_idx,graph) = std_e::permute_by_block(graph_idx, graph, new_to_old_numbering);

  // 2. then renumber the nodes
  std_e::dynarray<int> old_to_new_numbering = inverse_permutation(new_to_old_numbering);
  for (auto& node : graph) { node = old_to_new_numbering[node]; }
}


} // std_e
