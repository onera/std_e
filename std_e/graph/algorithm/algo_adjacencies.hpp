#pragma once


#include "std_e/graph/algorithm/algo_base.hpp"


namespace std_e {


template<class Rooted_graph, class F> constexpr auto
preorder_depth_first_find_adjacencies(Rooted_graph&& g, F&& f) {
  auto S = graph_traversal_stack(first_root(g),last_root(g));
  return preorder_depth_first_find_adjacency_stack(S,f);
}

// Case where decltype(first_root(g)) != decltype(first_child(g))
// We can still apply the algorithm, but can't return a unified iterator
// So we just return a boolean
template<class Rooted_graph, class F> constexpr auto
preorder_depth_first_find_adjacencies_b(Rooted_graph&& g, F&& f) -> bool {
  // TODO for root in [first_root,last_root)
  if (f(*first_root(g))) return true;
  auto S = graph_traversal_stack(first_child(g),last_child(g));
  auto last = preorder_depth_first_find_adjacency_stack(S,f);
  return last != last_child(g);
}
template<class Rooted_graph, class Graph_adjacency_visitor> constexpr auto
depth_first_find_adjacencies(Rooted_graph&& g, Graph_adjacency_visitor&& f) {
  auto S = graph_traversal_stack(first_root(g),last_root(g));
  return depth_first_find_adjacency_stack(S,f); // TODO test
}

template<class Rooted_graph, class Graph_adjacency_visitor> constexpr auto
prepostorder_depth_first_scan_adjacencies(Rooted_graph& g, Graph_adjacency_visitor&& f) -> void {
  // TODO for root in [first_root,last_root)
  f.pre(*first_root(g));
  auto S = graph_traversal_stack(first_child(g),last_child(g));
  prepostorder_depth_first_scan_adjacency_stack(S,f);
  f.post(*first_root(g));
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

// TODO use both implementations based on the type of Rooted_graph:
//  if first_root and first_child return the same type
//    -> then the first (commented out) implementation is enough
//  if first_root and first_child return different type,
//  we can't use a uniform graph_traversal_stack for the root iterator and the rest
//  so we need to basically re-write the algorithm just for the first level
//  (and use the "normal" stack algorithm for other levels)
//    -> this is the second algorithm
//      The second algorithm would work instead of the first,
//      but it is a tad more complex to read,
//      and maybe a little less efficient (same big O complexity, but with n_child more allocations)

template<class Rooted_graph, class Graph_adjacency_visitor> constexpr auto
depth_first_prune_adjacencies(Rooted_graph&& g, Graph_adjacency_visitor&& f) -> void {
  auto S = graph_traversal_stack(first_root(g),last_root(g));
  depth_first_prune_adjacency_stack(S,f);
}

//template<class Rooted_graph, class Graph_adjacency_visitor> constexpr auto
//depth_first_prune_adjacencies(Rooted_graph&& g, Graph_adjacency_visitor&& f) -> void {
//  auto first_r = first_root(g);
//  auto last_r = last_root(g);
//  while (first_r != last_r) {
//    if (!f.pre(*first_r)) { // go down
//      auto first = first_child(g);
//      auto last = last_child(g);
//      while (first != last) {
//        f.down(*first_r,*first);
//        auto S = graph_traversal_stack(first,std::next(first));
//        depth_first_prune_adjacency_stack(S,f);
//        f.up(*first,*first_r);
//        ++first;
//      }
//    }
//    f.post(*first_r);
//    ++first_r;
//  }
//}

template<class Rooted_graph, class Graph_adjacency_visitor> constexpr auto
depth_first_search_adjacencies(Rooted_graph&& g, Graph_adjacency_visitor&& f) {
  auto S = graph_traversal_stack(first_root(g),last_root(g));
  return depth_first_search_adjacency_stack(S,f); // TODO test
}


// special cases {
template<class Rooted_graph, class F> constexpr auto
preorder_depth_first_scan_adjacencies(Rooted_graph& g, F&& f) -> void {
  // TODO for root in [first_root,last_root)
  f(*first_root(g));
  auto S = graph_traversal_stack(first_child(g),last_child(g));
  preorder_depth_first_scan_adjacency_stack(S,f);
}
template<class Rooted_graph, class F> constexpr auto
postorder_depth_first_scan_adjacencies(Rooted_graph& g, F&& f) -> void {
  // TODO for root in [first_root,last_root)
  auto S = graph_traversal_stack(first_child(g),last_child(g));
  postorder_depth_first_scan_adjacency_stack(S,f);
  f(*first_root(g));
}
template<class Rooted_graph, class F> constexpr auto
preorder_depth_first_prune_adjacencies(Rooted_graph& g, F&& f) -> void {
  auto S = graph_traversal_stack(first_root(g),last_root(g));
  preorder_depth_first_prune_adjacency_stack(S,f);
}
// special cases }


} // std_e
