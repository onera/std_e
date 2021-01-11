#pragma once


#include "std_e/graph/algorithm/algo_base.hpp"


namespace graph {


template<class Rooted_graph, class F> constexpr auto
preorder_depth_first_find_adjacencies(Rooted_graph&& g, F&& f) {
  auto S = make_graph_traversal_stack(first_root(g),last_root(g));
  return preorder_depth_first_find_adjacency_stack(S,f);
}
template<class Rooted_graph, class Graph_adjacency_visitor> constexpr auto
depth_first_find_adjacencies(Rooted_graph&& g, Graph_adjacency_visitor&& f) {
  auto S = make_graph_traversal_stack(first_root(g),last_root(g));
  return depth_first_find_adjacency_stack(S,f); // TODO test
}

template<class Graph, class Graph_adjacency_visitor> constexpr auto
prepostorder_depth_first_scan_adjacencies(Graph& g, Graph_adjacency_visitor&& f) -> void {
  f.pre(g); // TODO rm, replace by first_root
  auto S = make_graph_traversal_stack(first_child(g),last_child(g));
  prepostorder_depth_first_scan_adjacency_stack(S,f);
  f.post(g);
}
template<class Rooted_graph, class Graph_adjacency_visitor> constexpr auto
depth_first_scan_adjacencies(Rooted_graph&& g, Graph_adjacency_visitor&& f) -> void {
  auto S = make_graph_traversal_stack(first_root(g),last_root(g));
  depth_first_scan_adjacency_stack(S,f);
}

template<class Rooted_graph, class Graph_adjacency_visitor> constexpr auto
prepostorder_depth_first_prune_adjacencies(Rooted_graph&& g, Graph_adjacency_visitor&& f) -> void {
  auto S = make_graph_traversal_stack(first_root(g),last_root(g));
  prepostorder_depth_first_prune_adjacency_stack(S,f);
}
template<class Rooted_graph, class Graph_adjacency_visitor> constexpr auto
depth_first_prune_adjacencies(Rooted_graph&& g, Graph_adjacency_visitor&& f) -> void {
  auto S = make_graph_traversal_stack(first_root(g),last_root(g));
  depth_first_prune_adjacency_stack(S,f);
}


// special cases {
template<class Graph, class F> constexpr auto
preorder_depth_first_scan_adjacencies(Graph& g, F&& f) -> void {
  f(g); // TODO rm, replace by first_root
  auto S = make_graph_traversal_stack(first_child(g),last_child(g));
  preorder_depth_first_scan_adjacency_stack(S,f);
}
template<class Graph, class F> constexpr auto
postorder_depth_first_scan_adjacencies(Graph& g, F&& f) -> void {
  auto S = make_graph_traversal_stack(first_child(g),last_child(g));
  postorder_depth_first_scan_adjacency_stack(S,f);
  f(g); // TODO rm, replace by first_root
}

template<class Graph, class F> constexpr auto
preorder_depth_first_prune_adjacencies(Graph& g, F&& f) -> void {
  auto S = make_graph_traversal_stack(first_root(g),last_root(g));
  preorder_depth_first_prune_adjacency_stack(S,f);
}
// special cases }


} // graph 
