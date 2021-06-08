#pragma once


#include "std_e/graph/algorithm/algo_base.hpp"


namespace std_e {


template<class Rooted_graph, class F> constexpr auto
preorder_depth_first_find_adjacencies(Rooted_graph&& g, F&& f) {
  auto S = graph_traversal_stack(first_root(g),last_root(g));
  return preorder_depth_first_find_adjacency_stack(S,f);
}
template<class Rooted_graph, class Graph_adjacency_visitor> constexpr auto
depth_first_find_adjacencies(Rooted_graph&& g, Graph_adjacency_visitor&& f) {
  auto S = graph_traversal_stack(first_root(g),last_root(g));
  return depth_first_find_adjacency_stack(S,f); // TODO test
}

template<class Rooted_graph, class Graph_adjacency_visitor> constexpr auto
prepostorder_depth_first_scan_adjacencies(Rooted_graph& g, Graph_adjacency_visitor&& f) -> void {
  auto S = graph_traversal_stack(first_root(g),last_root(g));
  prepostorder_depth_first_scan_adjacency_stack(S,f);
}
template<class Rooted_graph, class Graph_adjacency_visitor> constexpr auto
depth_first_scan_adjacencies(Rooted_graph&& g, Graph_adjacency_visitor&& f) -> void {
  auto S = graph_traversal_stack(first_root(g),last_root(g));
  depth_first_scan_adjacency_stack(S,f);
}

template<class Rooted_graph, class Graph_adjacency_visitor> constexpr auto
prepostorder_depth_first_prune_adjacencies(Rooted_graph&& g, Graph_adjacency_visitor&& f) -> void {
  auto S = graph_traversal_stack(first_root(g),last_root(g));
  prepostorder_depth_first_prune_adjacency_stack(S,f);
}
template<class Rooted_graph, class Graph_adjacency_visitor> constexpr auto
depth_first_prune_adjacencies(Rooted_graph&& g, Graph_adjacency_visitor&& f) -> void {
  auto S = graph_traversal_stack(first_root(g),last_root(g));
  depth_first_prune_adjacency_stack(S,f);
}


// special cases {
template<class Rooted_graph, class F> constexpr auto
preorder_depth_first_scan_adjacencies(Rooted_graph& g, F&& f) -> void {
  auto S = graph_traversal_stack(first_root(g),last_root(g));
  preorder_depth_first_scan_adjacency_stack(S,f);
}
template<class Rooted_graph, class F> constexpr auto
postorder_depth_first_scan_adjacencies(Rooted_graph& g, F&& f) -> void {
  auto S = graph_traversal_stack(first_root(g),last_root(g));
  postorder_depth_first_scan_adjacency_stack(S,f);
}

template<class Rooted_graph, class F> constexpr auto
preorder_depth_first_prune_adjacencies(Rooted_graph& g, F&& f) -> void {
  auto S = graph_traversal_stack(first_root(g),last_root(g));
  preorder_depth_first_prune_adjacency_stack(S,f);
}
// special cases }


} // std_e
