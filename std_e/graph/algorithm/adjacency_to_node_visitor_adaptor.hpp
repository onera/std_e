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
    constexpr auto
    pre(auto&& na) {
      return vis.pre(node(na));
    }
    constexpr auto
    down(auto&& na_above, auto&& na_below) -> void {
      vis.down(node(na_above),node(na_below));
    }
    constexpr auto
    up(auto&& na_below, auto&& na_above) -> void {
      vis.up(node(na_below),node(na_above));
    }
    constexpr auto
    post(auto na) -> void {
      vis.post(node(na));
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
