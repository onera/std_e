#pragma once


#include "graph/adjacency_graph/io_graph.hpp"
//#include "graph/algorithm/algo_adjacencies.hpp"
#include "graph/algorithm/algo_nodes.hpp"
#include "std_e/utils/functional.hpp"
#include <iostream> // TODO


namespace graph {
  

/** concept Node_builder
  result_type 
  F::operator()(Graph::node_type) -> T
*/


template<class Graph, class Node_builder>
struct bidirectional_graph_builder {
  public:
    using T = typename std::decay_t<Node_builder>::result_type;
    using node_type = typename Graph::node_type;
    //using node_type = T; // TODO false, why?
    using node_adjacency_type = typename Graph::node_adj_type;
    using output_graph_iterator = io_adjacency<T>*;

    template<class Node_builder_0> constexpr
    bidirectional_graph_builder(output_graph_iterator it, Node_builder_0&& f)
      : it(it)
      , node_builder(FWD(f))
    {}

  // Graph_node_visitor interface {
    constexpr auto
    pre(node_type) -> void {
      height_stack.push_level(0); // TODO RENAME push
      outs_stack.push_level({});
    }
    constexpr auto
    post(const node_type& n) -> void {
      // create node
      int height_lvl = height_stack.current_level();
      auto outs_lvl = outs_stack.current_level();
      auto new_n = node_builder(n,height_lvl,outs_lvl);
      *it = graph::io_adjacency<T>{
        new_n,
        {},
        outs_lvl
      };

      // back-tracking (add parent as the only parent node)
      for (auto* out : outs_lvl) {
        out->inwards = {it};
      }

      // prepare next iteration
      if (!outs_stack.is_at_root_level()) {
        height_stack.parent_level() = std::max(height_stack.parent_level(),height_stack.current_level()+1);
        outs_stack.parent_level().push_back(it);

        height_stack.pop_level();
        outs_stack.pop_level();
      }
      ++it;
    }
  // Graph_node_visitor interface }
  
  private:
    output_graph_iterator it;
    graph_stack<connections_container<T>> outs_stack;
    graph_stack<int> height_stack;
    std_e::remove_rvalue_reference<Node_builder> node_builder;
};


template<class Graph, class Node_builder> constexpr auto
build_bidirectional_graph(const Graph& g, Node_builder&& node_builder) {
  using T = typename std::decay_t<Node_builder>::result_type;
  io_graph<T> res(g.size());

  bidirectional_graph_builder<Graph,Node_builder> vis(begin(res),FWD(node_builder));
  prepostorder_depth_first_scan(g,vis);

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
