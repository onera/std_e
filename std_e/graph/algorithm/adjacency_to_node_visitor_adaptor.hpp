#pragma once


#include <utility>
#include "std_e/meta/meta.hpp"


namespace std_e {


template<class Graph_node_visitor>
class graph_adjacency_visitor_adaptor {
  public:
    template<class Graph_node_visitor_0> constexpr
    graph_adjacency_visitor_adaptor(Graph_node_visitor_0&& vis)
      : vis(FWD(vis))
    {}

  // Graph_adjacency_visitor interface {
    // requires Graph_node_visitor::node_adj_type == Node_adjacency
    template<class Node_adjacency> constexpr auto
    pre(Node_adjacency&& na) {
      return vis.pre(node(na));
    }
    template<class Node_adjacency_0, class Node_adjacency_1> constexpr auto
    down(Node_adjacency_0&& na_above, Node_adjacency_1&& na_below) {
      return vis.down(node(na_above),node(na_below));
    }
    template<class Node_adjacency_0, class Node_adjacency_1> constexpr auto
    up(Node_adjacency_0&& na_below, Node_adjacency_1&& na_above) {
      return vis.up(node(na_below),node(na_above));
    }
    template<class Node_adjacency> constexpr auto
    post(Node_adjacency&& na) {
      return vis.post(node(na));
    }
  // Graph_adjacency_visitor interface }
  private:
    std_e::remove_rvalue_reference<Graph_node_visitor> vis;
};

template<class F>
class graph_adjacency_functor_adaptor {
  public:
    template<class F_0> constexpr
    graph_adjacency_functor_adaptor(F_0&& f)
      : f(FWD(f))
    {}

    template<class Node_adjacency> constexpr auto
    operator()(Node_adjacency&& na) {
      return f(node(na));
    }
  private:
    std_e::remove_rvalue_reference<F> f;
};


} // std_e
