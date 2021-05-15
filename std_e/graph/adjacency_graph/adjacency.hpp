#pragma once


#include "std_e/graph/adjacency_graph/traits/traits.hpp"


namespace std_e {


template<class adjacency_graph_type>
class adjacency {
  public:
    using index_type = typename adjacency_graph_traits<adjacency_graph_type>::index_type;
    using node_type  = typename adjacency_graph_traits<adjacency_graph_type>::node_type;

    constexpr
    adjacency() = default;

    constexpr
    adjacency(adjacency_graph_type* g, index_type node_idx)
      : g(g)
      , node_idx(node_idx)
    {}

    constexpr auto
    node() -> node_type& {
      return g->nodes()[node_idx];
    }
    constexpr auto
    node() const -> const node_type& {
      return g->nodes()[node_idx];
    }

    constexpr auto
    degree() const -> index_type {
      return g->degree(node_idx);
    }
    constexpr auto
    edges() -> auto& {
      return g->edges()[node_idx];
    }
    constexpr auto
    edges() const -> const auto& {
      return g->edges()[node_idx];
    }

    constexpr auto
    adjacencies() -> adjacency_range<adjacency_graph_type> {
      return {g,node_idx};
    }
    constexpr auto
    adjacencies() const -> adjacency_range<const adjacency_graph_type> {
      return {g,node_idx};
    }

    template<class AGT0, class AGT1> friend constexpr auto
    operator==(const adjacency<AGT0>& x, const adjacency<AGT1>& y) -> bool;
  private:
    adjacency_graph_type* g;
    index_type node_idx;
};
template<class AGT0, class AGT1> constexpr auto
operator==(const adjacency<AGT0>& x, const adjacency<AGT1>& y) -> bool {
  return
      x.g == y.g
   && x.node_idx == y.node_idx;
}
template<class AGT0, class AGT1> constexpr auto
operator!=(const adjacency<AGT0>& x, const adjacency<AGT1>& y) -> bool {
  return !(x==y);
}


template<class adjacency_graph_type>
class io_adjacency {
  public:
    using index_type = typename adjacency_graph_traits<adjacency_graph_type>::index_type;
    using node_type  = typename adjacency_graph_traits<adjacency_graph_type>::node_type;

    constexpr
    io_adjacency() = default;

    constexpr
    io_adjacency(adjacency_graph_type* g, index_type node_idx)
      : g(g)
      , node_idx(node_idx)
    {}

    constexpr auto
    index() const -> index_type {
      return node_idx;
    }

    constexpr auto
    node() -> node_type& {
      return g->nodes()[node_idx];
    }
    constexpr auto
    node() const -> const node_type& {
      return g->nodes()[node_idx];
    }

    constexpr auto
    in_degree() const -> index_type {
      return g->in_degree(node_idx);
    }
    constexpr auto
    out_degree() const -> index_type {
      return g->out_degree(node_idx);
    }
    constexpr auto
    in_edges() -> auto& {
      return g->in_edges()[node_idx];
    }
    constexpr auto
    in_edges() const -> const auto& {
      return g->in_edges()[node_idx];
    }
    constexpr auto
    out_edges() -> auto& {
      return g->out_edges()[node_idx];
    }
    constexpr auto
    out_edges() const -> const auto& {
      return g->out_edges()[node_idx];
    }

    constexpr auto
    in_adjacencies() -> adjacency_range<adjacency_graph_type,adj_orientation::in> {
      return {g,node_idx};
    }
    constexpr auto
    in_adjacencies() const -> adjacency_range<const adjacency_graph_type,adj_orientation::in> {
      return {g,node_idx};
    }
    constexpr auto
    out_adjacencies() -> adjacency_range<adjacency_graph_type,adj_orientation::out> {
      return {g,node_idx};
    }
    constexpr auto
    out_adjacencies() const -> adjacency_range<const adjacency_graph_type,adj_orientation::out> {
      return {g,node_idx};
    }

    template<class AGT0, class AGT1> friend constexpr auto
    operator==(const io_adjacency<AGT0>& x, const io_adjacency<AGT1>& y) -> bool;
  private:
    adjacency_graph_type* g;
    index_type node_idx;
};
template<class AGT0, class AGT1> constexpr auto
operator==(const io_adjacency<AGT0>& x, const io_adjacency<AGT1>& y) -> bool {
  return
      x.g == y.g
   && x.node_idx == y.node_idx;
}
template<class AGT0, class AGT1> constexpr auto
operator!=(const io_adjacency<AGT0>& x, const io_adjacency<AGT1>& y) -> bool {
  return !(x==y);
}


} // std_e
