#pragma once


#include "std_e/graph/algorithm/algo_base.hpp"
#include "std_e/graph/algorithm/adjacency_to_node_visitor_adaptor.hpp"
#include <functional>


namespace std_e {


template<class Graph, class Unary_pred> constexpr auto
all_of_adjacencies(Graph&& g, Unary_pred&& p) -> bool {
  auto S = graph_traversal_stack(first_root(g),last_root(g));
  preorder_depth_first_find_adjacency_stack(S,std::not_fn(p));
  return S.is_done(); // scanned until the end
}
template<class Graph, class Unary_pred> constexpr auto
all_of(Graph&& g, Unary_pred&& p_node) -> bool {
  graph_adjacency_functor_adaptor<Unary_pred> p(FWD(p_node));
  return all_of_adjacencies(g,p);
}


} // std_e
