#pragma once


#include "std_e/graph/adjacency_graph/graph.hpp"


namespace std_e {


template<class graph_type>
class rooted_view {
  public:
    using node_type = typename graph_type::node_type;
    using edge_type = typename graph_type::edge_type;
    using adjacency_list_type = typename graph_type::adjacency_list_type;
    using index_type = typename graph_type::index_type;

    using adjacency_type = io_adjacency<rooted_view>;
    using const_adjacency_type = io_adjacency<const rooted_view>;

    //using adjacency_node_iterator_type = adjacency_node_iterator<derived>;
    //using const_adjacency_node_iterator_type = adjacency_node_iterator<const derived>;

    using adjacency_connection_iterator_type = adjacency_edge_iterator<rooted_view,adj_orientation::out>;
    using const_adjacency_connection_iterator_type = adjacency_edge_iterator<const rooted_view,adj_orientation::out>;

    constexpr
    rooted_view(graph_type& g, index_type root_idx)
      : g(&g)
      , root_idx(root_idx)
    {}
    constexpr
    rooted_view(const io_adjacency<graph_type>& adj)
      : rooted_view(*adj.graph(),adj.node_index())
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

    auto
    out_index(index_type i_node, index_type i_edge) const -> index_type {
      if (i_node==-1) {
        STD_E_ASSERT(i_edge==0);
        return root_idx;
      } else {
        return g->out_index(i_node,i_edge);
      }
    }

    auto
    nodes() -> auto& {
      return g->nodes();
    }
    auto
    nodes() const -> const auto& {
      return g->nodes();
    }

    auto
    out_degree(index_type i) const -> index_type {
      return g->out_degree(i);
    }

  private:
  // data
    graph_type* g;
    index_type root_idx;
};

template<class GT>
rooted_view(GT& g, typename GT::index_type root_idx) -> rooted_view<GT>;

template<class GT>
rooted_view(const io_adjacency<GT>& adj) -> rooted_view<GT>;


template<class GT> constexpr auto
first_root(rooted_view<GT>& x) {
  return x.first_root();
}
template<class GT> constexpr auto
first_root(const rooted_view<GT>& x) {
  return x.first_root();
}
template<class GT> constexpr auto
last_root(rooted_view<GT>& x) {
  return x.last_root();
}
template<class GT> constexpr auto
last_root(const rooted_view<GT>& x) {
  return x.last_root();
}
// rooted graph view }


} // std_e
