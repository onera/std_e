#pragma once


#include "graph/adjacency_graph/io_graph.hpp"
#include "graph/algorithm/algo_adjacencies.hpp"
#include "std_e/utils/functional.hpp"
#include <iostream> // TODO


namespace graph {
  

/** Node_builder
  result_type 
  F::operator()(Graph::node_type) -> T
*/


template<class Graph, class Node_builder>
struct bidirectional_graph_builder {
  public:
    using T = typename std::decay_t<Node_builder>::result_type;
    using node_adjacency_type = typename Graph::node_adj_type;

    template<class Node_builder_0> constexpr
    bidirectional_graph_builder(io_graph<T>& g, Node_builder_0&& f)
      : g(g)
      , node_builder(FWD(f))
    {}

  // Graph_adjacency_visitor interface {
    constexpr auto
    pre(node_adjacency_type) -> void {
      heights.push_level(0); // TODO RENAME push
      out_deps.push_level({});
    }
    constexpr auto
    post(const node_adjacency_type& na) -> void {
      const auto& n = root(na);
      auto new_n = node_builder(n,heights.current_level(),out_deps.current_level());
      int node_id = g.size();
      g.adjs.push_back(graph::io_adjacency<T>{
        new_n,
        {},
        out_deps.current_level(),
        node_id
      });

      auto* adj_ptr = &g.adjs.back();

      // back-tracking (add parent as the only parent node)
      for (auto* out_dep : out_deps.current_level()) {
        g[out_dep->index].inwards = {adj_ptr};
      }

      // prepare next iteration
      if (!out_deps.is_at_root_level()) {
        heights.parent_level() = std::max(heights.parent_level(),heights.current_level()+1);
        out_deps.parent_level().push_back(adj_ptr);

        heights.pop_level();
        out_deps.pop_level();
      }
    }
  // Graph_adjacency_visitor interface }
  
  private:
    io_graph<T>& g;
    graph_stack<connections_container<T>> out_deps;
    graph_stack<int> heights;
    std_e::remove_rvalue_reference<Node_builder> node_builder;
};


template<class Graph, class Node_builder, class T> constexpr auto
build_bidirectional_graph(const Graph& g, Node_builder&& node_builder, io_graph<T>& res) {
  bidirectional_graph_builder<Graph,Node_builder> vis(res,FWD(node_builder));
  prepostorder_depth_first_scan_adjacencies(g,vis);
}
template<class Graph, class Node_builder> constexpr auto
build_bidirectional_graph(const Graph& g, Node_builder&& node_builder) {
  using T = typename std::decay_t<Node_builder>::result_type;
  io_graph<T> res;
  build_bidirectional_graph(g,node_builder,res);
  return res;
}


template<class T>
struct identity_node_builder {
  using result_type = T;
  constexpr auto operator()(const T& x) { return x; }
};
template<class Graph> constexpr auto
build_adjacency_list(const Graph& g) {
  using T = typename Graph::node_type;
  return build_adjacency_list(g,identity_node_builder<T>{});
}
template<class Graph> constexpr auto
build_bidirectional_graph(const Graph& g) {
  using T = typename Graph::node_type;
  return build_bidirectional_graph(g,identity_node_builder<T>{});
}


} // graph
