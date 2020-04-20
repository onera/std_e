#pragma once


#include "std_e/graph/algorithm/algo_nodes.hpp"
#include "std_e/utils/vector.hpp"


namespace graph {


template<class builder_visitor_type>
// requires builder_from_adjacency_type::tree_type is nested_tree<...>
// requires builder_from_adjacency_type::build(...) -> tree
struct tree_builder {
  public:
    using tree_type = typename builder_visitor_type::tree_type;
    using tree_node_type = typename tree_type::node_type;

    constexpr
    tree_builder(builder_visitor_type bv)
      : build_vis(std::move(bv))
    {}

  // Graph_adjacency_visitor interface {
    template<class type_to_build_from> constexpr auto
    pre(const type_to_build_from& x) -> void {
      auto res = build_vis.build(x);
      std_e::append(nodes,res.nodes);
      std_e::append(sizes,res.sizes);

      sizes_stack.push_level(res.nodes.size());
      int last_index = nodes.size()-res.nodes.size();
      indices_stack.push_level(last_index);
    }
    template<class T> constexpr auto
    post(const T&) -> void {
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
    constexpr auto
    build_visitor() -> builder_visitor_type& {
      return build_vis;
    }
  private:
    std_e::remove_rvalue_reference<builder_visitor_type> build_vis;
    graph_stack<int> indices_stack;
    graph_stack<int> sizes_stack;
    std::vector<tree_node_type> nodes;
    std::vector<int> sizes;
};

template<class Graph, class builder_from_adjacency_type> inline auto
// requires builder_from_adjacency_type::tree_type is nested_tree<...>
// requires builder_from_adjacency_type::build(graph_node_type) -> tree
create_tree_from_adjacencies(Graph& g, builder_from_adjacency_type&& b) {
  tree_builder vis(FWD(b));
  graph::prepostorder_depth_first_scan_adjacencies(g,vis);
  return vis.retrieve_final_tree();
}

template<class Graph, class builder_from_node_type> inline auto
// requires builder_from_adjacency_type::tree_type is nested_tree<...>
// requires builder_from_node_type::build(graph_node_type) -> tree
create_tree_from_nodes(Graph& g, builder_from_node_type&& b) {
  tree_builder vis(FWD(b));
  graph::prepostorder_depth_first_scan(g,vis);
  return vis.retrieve_final_tree();
}


template<class builder_visitor_type>
// requires builder_from_adjacency_type::tree_type is nested_tree<...>
// requires builder_from_adjacency_type::should_go_down(...) -> bool
// requires builder_from_adjacency_type::build(...) -> tree
class pruned_tree_builder {
  public:
    using tree_builder_type = tree_builder<builder_visitor_type>;

    constexpr
    pruned_tree_builder(builder_visitor_type bv)
      : tb(std::move(bv))
    {}

  // Graph_adjacency_visitor interface {
    template<class T> constexpr auto
    pre(const T& x) -> bool {
      tb.pre(x);
      return tb.build_visitor().should_go_down(x);
    }
    template<class T> constexpr auto
    post(const T& x) -> void {
      tb.post(x);
    }
  // Graph_adjacency_visitor interface }

    constexpr auto
    retrieve_final_tree() {
      return tb.retrieve_final_tree();
    }

  private:
     tree_builder_type tb;
};

template<class Graph, class builder_from_adjacency_type> inline auto
// requires builder_from_adjacency_type::tree_type is nested_tree<...>
// requires builder_from_adjacency_type::should_go_down(graph_node_type) -> bool
// requires builder_from_adjacency_type::build(graph_node_type) -> tree
create_pruned_tree_from_adjacencies(Graph& g, builder_from_adjacency_type&& b) {
  pruned_tree_builder vis(FWD(b));
  graph::prepostorder_depth_first_prune_adjacencies(g,vis);
  return vis.retrieve_final_tree();
}

template<class Graph, class builder_from_node_type> inline auto
// requires builder_from_adjacency_type::tree_type is nested_tree<...>
// requires builder_from_node_type::should_go_down(graph_node_type) -> bool
// requires builder_from_node_type::build(graph_node_type) -> tree
create_pruned_tree_from_nodes(Graph& g, builder_from_node_type&& b) {
  pruned_tree_builder vis(FWD(b));
  graph::prepostorder_depth_first_prune(g,vis);
  return vis.retrieve_final_tree();
}


} // graph
