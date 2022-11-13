#pragma once
// TODO rename file root_graph or rooted_graph


#include "std_e/graph/adjacency_graph/graph.hpp"


namespace std_e {


template<class graph_t>
class rooted_graph {
  public:
    using graph_type = std::remove_reference_t<graph_t>;
    using node_type = typename graph_type::node_type;
    using edge_type = typename graph_type::edge_type;
    using adjacency_list_type = typename graph_type::adjacency_list_type;
    using index_type = typename graph_type::index_type;

    using adjacency_type = io_adjacency<rooted_graph>;
    using const_adjacency_type = io_adjacency<const rooted_graph>;

    //using adjacency_node_iterator_type = adjacency_node_iterator<derived>;
    //using const_adjacency_node_iterator_type = adjacency_node_iterator<const derived>;

    using adjacency_connection_iterator_type = adjacency_edge_iterator<rooted_graph,adj_orientation::out>;
    using const_adjacency_connection_iterator_type = adjacency_edge_iterator<const rooted_graph,adj_orientation::out>;

    constexpr
    rooted_graph(graph_t g, index_type root_idx)
      : g(FWD(g))
      , root_idx(root_idx)
    {}
    constexpr
    rooted_graph(const io_adjacency<graph_type>& adj)
      : rooted_graph(*adj.graph(),adj.node_index())
    {}

    auto
    first_root() -> adjacency_connection_iterator_type {
      return {this,-1,0};
    }
    auto
    first_root() const -> const_adjacency_connection_iterator_type {
      return {this,-1,0};
    }
    auto
    last_root() -> adjacency_connection_iterator_type {
      return {this,-1,1};
    }
    auto
    last_root() const -> const_adjacency_connection_iterator_type {
      return {this,-1,1};
    }

    constexpr auto
    out_adjacencies() -> adjacency_range<graph_type,adj_orientation::out> {
      return {&g,root_idx};
    }
    constexpr auto
    out_adjacencies() const -> const adjacency_range<graph_type,adj_orientation::out> {
      return {&g,root_idx};
    }
    constexpr auto
    first_child() {
      return out_adjacencies().begin();
    }
    constexpr auto
    first_child() const {
      return out_adjacencies().begin();
    }
    constexpr auto
    last_child() {
      return out_adjacencies().end();
    }
    constexpr auto
    last_child() const {
      return out_adjacencies().end();
    }

    auto
    out_index(index_type i_node, index_type i_edge) const -> index_type {
      if (i_node==-1) {
        return root_idx;
      } else {
        return g.out_index(i_node,i_edge);
      }
    }

    auto
    nodes() -> auto& {
      return g.nodes();
    }
    auto
    nodes() const -> const auto& {
      return g.nodes();
    }

    auto
    out_degree(index_type i) const -> index_type {
      return g.out_degree(i);
    }

    auto
    to_string_impl() const -> std::string {
      return to_string(g);
    }
  public:
  // data
    std_e::remove_rvalue_reference<graph_t> g;
    index_type root_idx;
};

template<class GT>
rooted_graph(GT g, typename std::remove_reference_t<GT>::index_type root_idx) -> rooted_graph<GT>;

template<class GT>
rooted_graph(const io_adjacency<GT>& adj) -> rooted_graph<GT&>;


template<class GT> constexpr auto
first_root(rooted_graph<GT>& x) {
  return x.first_root();
}
template<class GT> constexpr auto
first_root(const rooted_graph<GT>& x) {
  return x.first_root();
}
template<class GT> constexpr auto
last_root(rooted_graph<GT>& x) {
  return x.last_root();
}
template<class GT> constexpr auto
last_root(const rooted_graph<GT>& x) {
  return x.last_root();
}

template<class GT> constexpr auto
first_child(rooted_graph<GT>& x) {
  return x.first_child();
}
//template<class GT> constexpr auto
//first_child(rooted_graph<GT>&& x) {
//  return std::move(x).first_child();
//}
template<class GT> constexpr auto
first_child(const rooted_graph<GT>& x) {
  return x.first_child();
}
template<class GT> constexpr auto
last_child(rooted_graph<GT>& x) {
  return x.last_child();
}
//template<class GT> constexpr auto
//last_child(rooted_graph<GT>&& x) {
//  return std::move(x).last_child();
//}
template<class GT> constexpr auto
last_child(const rooted_graph<GT>& x) {
  return x.last_child();
}

template<class GT> auto
to_string(const rooted_graph<GT>& x) -> std::string {
  return x.to_string_impl();
}

template<class GT>
using rooted_view = rooted_graph<GT&>;

} // std_e
