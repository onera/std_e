#pragma once


#include "std_e/graph/algorithm/algo_adjacencies.hpp"
#include "std_e/graph/algorithm/adjacency_to_node_visitor_adaptor.hpp"


namespace graph {


template<class Graph, class F> constexpr auto
preorder_depth_first_find(Graph&& g, F&& f_node) -> void {
  graph_adjacency_functor_adaptor<F> f(FWD(f_node));
  preorder_depth_first_find_adjacencies(g,f);
}

template<class Graph, class F> constexpr auto
preorder_depth_first_scan(Graph&& g, F&& f_node) -> void {
  graph_adjacency_functor_adaptor<F> f(FWD(f_node));
  preorder_depth_first_scan_adjacencies(g,f);
}
template<class Graph, class F> constexpr auto
postorder_depth_first_scan(Graph&& g, F&& f_node) -> void {
  graph_adjacency_functor_adaptor<F> f(FWD(f_node));
  postorder_depth_first_scan_adjacencies(g,f);
}
template<class Graph, class Graph_node_visitor> constexpr auto
prepostorder_depth_first_scan(Graph&& g, Graph_node_visitor&& f_node) -> void {
  graph_adjacency_visitor_adaptor<Graph_node_visitor> f(FWD(f_node));
  prepostorder_depth_first_scan_adjacencies(g,f);
}

template<class Graph, class Graph_node_visitor> constexpr auto
depth_first_scan(Graph&& g, Graph_node_visitor&& f_node) -> void {
  graph_adjacency_visitor_adaptor<Graph_node_visitor> f(FWD(f_node));
  depth_first_scan_adjacencies(g,f);
}
template<class Graph, class Graph_node_visitor> constexpr auto
depth_first_prune(Graph&& g, Graph_node_visitor&& f_node) -> void {
  graph_adjacency_visitor_adaptor<Graph_node_visitor> f(FWD(f_node));
  depth_first_prune_adjacencies(g,f);
}
template<class Graph, class Graph_node_visitor> constexpr auto
prepostorder_depth_first_prune(Graph&& g, Graph_node_visitor&& f_node) -> void {
  graph_adjacency_visitor_adaptor<Graph_node_visitor> f(FWD(f_node));
  prepostorder_depth_first_prune_adjacencies(g,f);
}


} // graph 
