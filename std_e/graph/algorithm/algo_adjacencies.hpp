#pragma once


#include "std_e/graph/algorithm/algo_base.hpp"


namespace graph {


template<class Graph, class F> constexpr auto
preorder_depth_first_find_adjacencies(Graph&& g, F&& f) -> void {
  if (f(g)) {
    auto S = make_graph_traversal_stack(children(g));
    preorder_depth_first_find_adjacency_stack(S,f);
  }
}

template<class Graph, class Graph_adjacency_visitor> constexpr auto
prepostorder_depth_first_scan_adjacencies(Graph&& g, Graph_adjacency_visitor&& f) -> void {
  f.pre(g);
  auto S = make_graph_traversal_stack(children(g));
  prepostorder_depth_first_scan_adjacency_stack(S,f);
  f.post(g);
}

template<class Graph, class Graph_adjacency_visitor> constexpr auto
depth_first_scan_adjacencies(Graph&& g, Graph_adjacency_visitor&& f) -> void {
  auto S = make_graph_traversal_stack(root_as_range(g));
  depth_first_scan_adjacency_stack(S,f);
}

template<class Node_adjacency, class Graph_adjacency_visitor> constexpr auto
prepostorder_depth_first_prune_adjacencies(Node_adjacency&& a, Graph_adjacency_visitor&& f) -> void {
  if (f.pre(a)) {
    auto S = make_graph_traversal_stack(children(a));
    prepostorder_depth_first_prune_adjacency_stack(S,f);
    f.post(a);
  }
}
template<class Graph, class Graph_adjacency_visitor> constexpr auto
depth_first_prune_adjacencies(Graph&& g, Graph_adjacency_visitor&& f) -> void {
  auto S = make_graph_traversal_stack(root_as_range(g));
  depth_first_prune_adjacency_stack(S,f);
}


// special cases {
template<class Graph, class F> constexpr auto
preorder_depth_first_scan_adjacencies(Graph&& g, F&& f) -> void {
  f(g);
  auto S = make_graph_traversal_stack(children(g));
  preorder_depth_first_scan_adjacency_stack(S,f);
}
template<class Graph, class F> constexpr auto
postorder_depth_first_scan_adjacencies(Graph&& g, F&& f) -> void {
  auto S = make_graph_traversal_stack(children(g));
  postorder_depth_first_scan_adjacency_stack(S,f);
  f(g);
}
// special cases }


} // graph 
