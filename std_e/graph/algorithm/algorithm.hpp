#pragma once


#include "std_e/graph/algorithm/algo_base.hpp"
#include "std_e/future/functional.hpp"


namespace std_e {


template<class Graph, class Unary_pred> constexpr auto
all_of(Graph&& g, Unary_pred&& p) -> bool {
  auto S = graph_traversal_stack(first_root(g),last_root(g));
  preorder_depth_first_find_adjacency_stack(S,not_fn(p));
  return S.is_done(); // scanned until the end
}


} // std_e
