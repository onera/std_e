#pragma once


#include "graph/algorithm/algo_nodes.hpp"
#include "std_e/utils/vector.hpp"
#include <iostream> // TODO DEL


namespace graph {


template<class Tree_accumulator>
struct tree_builder {
  public:
    using tree_type = typename Tree_accumulator::tree_type;
    using tree_node_type = typename tree_type::node_type;

    using graph_node_type = typename Tree_accumulator::graph_node_type;

    constexpr
    tree_builder(Tree_accumulator f)
      : tree_accumulator(std::move(f))
    {}

  // Graph_adjacency_visitor interface {
    constexpr auto
    pre(const graph_node_type& n) -> bool {
      auto res = tree_accumulator.build(n);
      std_e::append(nodes,res.nodes);
      std_e::append(sizes,res.sizes);

      sizes_stack.push_level(res.nodes.size());
      int last_index = nodes.size()-1;
      indices_stack.push_level(last_index);
      return tree_accumulator.should_go_down(n);
    }
    constexpr auto
    post(const graph_node_type&) -> void {
      sizes[indices_stack.current_level()] = sizes_stack.current_level();

      if (!indices_stack.is_at_root_level()) {
        sizes_stack.parent_level() += sizes_stack.current_level();

        indices_stack.pop_level();
        sizes_stack.pop_level();
      }
    }
  // Graph_adjacency_visitor interface }

    constexpr auto
    retrieve_final_tree() -> tree_type {
      return tree_type(std::move(nodes),std::move(sizes));
    }
  private:
    std_e::remove_rvalue_reference<Tree_accumulator> tree_accumulator;
    graph::graph_stack<int> indices_stack;
    graph::graph_stack<int> sizes_stack;
    std::vector<tree_node_type> nodes;
    std::vector<int> sizes;
};

template<class Adjacency, class node_builder_type> inline auto
// requires node_builder_type::should_go_down(graph_node_type) -> bool
// requires node_builder_type::build(graph_node_type) -> tree
create_tree_from_graph(Adjacency& adj, node_builder_type b) {
  tree_builder vis(b);
  graph::prepostorder_depth_first_prune(adj,vis);
  return vis.retrieve_final_tree();
}


template<class Tree_accumulator>
struct tree_builder2 {
  public:
    using tree_type = typename Tree_accumulator::tree_type;
    using tree_node_type = typename tree_type::node_type;

    //using graph_node_type = typename Tree_accumulator::graph_node_type;
    //using graph_adj_type = typename Tree_accumulator::graph_adj_type;

    constexpr
    tree_builder2(Tree_accumulator f)
      : tree_accumulator(std::move(f))
    {}

  // Graph_adjacency_visitor interface {
    template<class TTT> constexpr auto
    pre(const TTT& adj) -> bool {
      auto res = tree_accumulator.build(adj);
      std_e::append(nodes,res.nodes);
      std_e::append(sizes,res.sizes);

      sizes_stack.push_level(res.nodes.size());
      int last_index = nodes.size()-1;
      indices_stack.push_level(last_index);
      return tree_accumulator.should_go_down(adj);
    }
    template<class TTT> constexpr auto
    post(const TTT&) -> void {
      sizes[indices_stack.current_level()] = sizes_stack.current_level();

      if (!indices_stack.is_at_root_level()) {
        sizes_stack.parent_level() += sizes_stack.current_level();

        indices_stack.pop_level();
        sizes_stack.pop_level();
      }
    }
  // Graph_adjacency_visitor interface }

    constexpr auto
    retrieve_final_tree() -> tree_type {
      return tree_type(std::move(nodes),std::move(sizes));
    }
  private:
    std_e::remove_rvalue_reference<Tree_accumulator> tree_accumulator;
    graph::graph_stack<int> indices_stack;
    graph::graph_stack<int> sizes_stack;
    std::vector<tree_node_type> nodes;
    std::vector<int> sizes;
};

template<class Adjacency, class node_builder_type> inline auto
// requires node_builder_type::should_go_down(graph_node_type) -> bool
// requires node_builder_type::build(graph_node_type) -> tree
create_tree_from_graph2(Adjacency& adj, node_builder_type b) {
  tree_builder2 vis(b);
  graph::prepostorder_depth_first_prune_adjacencies(adj,vis);
  return vis.retrieve_final_tree();
}


struct tree_builder3 {
  public:

  // Graph_adjacency_visitor interface {
    template<class TTT> constexpr auto
    pre(const TTT& n) -> void {
      //auto res = tree_accumulator.build(n);
      //std_e::append(nodes,res.nodes);
      //std_e::append(sizes,res.sizes);

      //sizes_stack.push_level(res.nodes.size());
      //int last_index = nodes.size()-1;
      //indices_stack.push_level(last_index);
      //return tree_accumulator.should_go_down(n);
    }
    template<class TTT> constexpr auto
    post(const TTT&) -> void {
      //sizes[indices_stack.current_level()] = sizes_stack.current_level();

      //if (!indices_stack.is_at_root_level()) {
      //  sizes_stack.parent_level() += sizes_stack.current_level();

      //  indices_stack.pop_level();
      //  sizes_stack.pop_level();
      //}
    }
  // Graph_adjacency_visitor interface }

};

template<class Adjacency> inline auto
// requires node_builder_type::should_go_down(graph_node_type) -> bool
// requires node_builder_type::build(graph_node_type) -> tree
create_tree_from_graph3(const Adjacency& adj) {
  std::cout << "lolo01\n";
  std::cout << "lolo02\n";
  graph::prepostorder_depth_first_scan(adj,tree_builder3{});
  std::cout << "lolo03\n";
  //return vis.retrieve_final_tree();
}


} // graph
