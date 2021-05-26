#pragma once


#include "std_e/graph/adjacency_graph/adjacency_graph.hpp"
#include "std_e/graph/algorithm/algo_nodes.hpp"
#include "std_e/utils/functional.hpp"


namespace std_e {


/** concept Node_builder
  result_type
  F::operator()(Graph::node_type) -> T
*/


template<class Graph, class Node_builder>
struct bidirectional_graph_builder {
  public:
    using node_type = typename std::decay_t<Node_builder>::arg_type;
    using index_type = typename Graph::index_type;
    using adj_list_inner_type = std_e::inner_range_type<typename Graph::adjacency_list_type>;

    template<class Node_builder_0> constexpr
    bidirectional_graph_builder(Graph* g, Node_builder_0&& f)
      : g(g)
      , i(0)
      , node_builder(FWD(f))
    {}

  // Graph_node_visitor interface {
    constexpr auto
    pre(node_type) -> void {
      height_stack.push_level(0); // TODO RENAME push
      outs_stack.push_level({});
    }
    auto
    post(const node_type& n) -> void {
      // create node
      int height_lvl = height_stack.current_level();
      auto outs_lvl = outs_stack.current_level();
      auto new_n = node_builder(n,height_lvl,outs_lvl);
      g->nodes()[i] = new_n;
      g->in_indices()[i] =  {};
      g->out_indices()[i] = outs_lvl;

      // back-tracking (add parent as the only parent node)
      for (index_type out : outs_lvl) {
        g->in_indices()[out] =  {i};
      }

      // prepare next iteration
      if (!outs_stack.is_at_root_level()) {
        height_stack.parent_level() = std::max(height_stack.parent_level(),height_stack.current_level()+1);
        outs_stack.parent_level().push_back(i);

        height_stack.pop_level();
        outs_stack.pop_level();
      }
      ++i;
    }
  // Graph_node_visitor interface }

  private:
    Graph* g;
    index_type i;
    graph::graph_stack<adj_list_inner_type> outs_stack;
    graph::graph_stack<int> height_stack;
    std_e::remove_rvalue_reference<Node_builder> node_builder;
};


template<class Graph, class Node_builder> constexpr auto
build_bidirectional_graph(const Graph& g, Node_builder&& node_builder) {
  using T = typename std::decay_t<Node_builder>::result_type;
  io_adjacency_graph<T> res(g.size());

  bidirectional_graph_builder<io_adjacency_graph<T>,Node_builder> vis(&res,FWD(node_builder));
  prepostorder_depth_first_scan(g,vis);

  return res;
}


template<class T>
struct identity_node_builder {
  using result_type = T;
  constexpr auto operator()(const T& x) { return x; }
};
template<class Graph> constexpr auto
build_bidirectional_graph(const Graph& g) {
  using T = typename Graph::node_type;
  return build_bidirectional_graph(g,identity_node_builder<T>{});
}


} // std_e
