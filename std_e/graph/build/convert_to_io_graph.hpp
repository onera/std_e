#pragma once


#include "std_e/graph/build/build.hpp"
#include "std_e/graph/adjacency_graph/graph.hpp"


namespace std_e {


struct int_height {
  int i;
  int height;
};
constexpr auto
operator==(const int_height& x, const int_height& y) -> bool {
  return x.i==y.i && x.height==y.height;
}
constexpr auto
operator!=(const int_height& x, const int_height& y) -> bool {
  return !(x==y);
}
[[maybe_unused]] auto
to_string(const int_height& x) -> std::string {
  return '('+std::to_string(x.i)+','+std::to_string(x.height)+')';
}

class fb_tree_ctor {
  public:
    // we initialize an empty `fb_graph`
    fb_tree_ctor()
    {}

    // each time we encounter an adjacency in our origin graph
    //   (which can happen multiple times for the same adjacency)
    //   we just create a new adjacency in our output graph
    auto operator()(const auto& adj, const auto& sub_node_indices) {
      // 0. compute height // TODO extract this logic somewhere else
      int height = 0;
      for (const auto& sub_node_idx : sub_node_indices) { // TODO std::max
        auto [_,h] = fb_graph[sub_node_idx].node();
        if (h+1 > height) {
          height = h+1;
        }
      }

      // 1. create new adjacency
      fb_graph.emplace_back(int_height{node(adj), height}, std::vector<int>{}, sub_node_indices);
      int built_idx = fb_graph.size()-1;

      // 2. now the index of the new adjacency can be propagated as parent of sub-nodes
      for (const auto& sub_node_idx : sub_node_indices) {
        fb_graph[sub_node_idx].in_indices().emplace_back( built_idx );
      }

      // 3. return the new index so that the algorithm can pass it once it builds the parent of `adj`
      return built_idx;
    }

    // return the graph we just built
    auto retrieve() {
      return std::move(fb_graph);
    }
  private:
    std_e::io_graph<int_height> fb_graph;
};


template<class Graph> constexpr auto
convert_tree_to_io_graph_with_height(const Graph& g) {
  fb_tree_ctor f;
  depth_first_build<int>(g, f); // TODO get rid of explicit `int`
  return f.retrieve();
}


} // std_e
