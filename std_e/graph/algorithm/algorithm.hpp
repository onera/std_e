#pragma once


#include "std_e/graph/algorithm/algo_base.hpp"
#include "std_e/graph/algorithm/adjacency_to_node_visitor_adaptor.hpp"


namespace graph {


template<class Graph, class Unary_pred> constexpr auto
all_of_adjacencies(Graph&& g, Unary_pred&& p) -> bool {
  auto S = make_graph_traversal_stack(root_as_range(g));
  preorder_depth_first_find_adjacency_stack(S,p);
  return S.is_at_root_level() && S.level_is_done(); // scaned until the end
}
template<class Graph, class Unary_pred> constexpr auto
all_of(Graph&& g, Unary_pred&& p_node) -> bool {
  graph_adjacency_functor_adaptor<Unary_pred> p(FWD(p_node));
  return all_of_adjacencies(g,p);
}


} // graph 