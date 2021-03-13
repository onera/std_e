#pragma once


#include "std_e/graph/adjacency_graph/traits/traits.hpp"


namespace std_e {


template<class adjacency_graph_type>
class adjacency_node_iterator {
  public:
    using index_type = typename adjacency_graph_traits<adjacency_graph_type>::index_type;
    using adjacency_type = typename adjacency_graph_traits<adjacency_graph_type>::adjacency_type;

    constexpr
    adjacency_node_iterator() = default;

    constexpr
    adjacency_node_iterator(adjacency_graph_type* g, index_type node_idx)
      : g(g)
      , node_idx(node_idx)
    {}

    constexpr auto
    operator++() -> adjacency_node_iterator& {
      ++node_idx;
      return *this;
    }

    constexpr auto
    operator*() const -> adjacency_type {
      return {g,node_idx};
    }

    friend constexpr auto
    operator==(const adjacency_node_iterator& x, const adjacency_node_iterator& y) -> bool {
      STD_E_ASSERT(x.g==y.g);
      return x.node_idx == y.node_idx;
    }
    friend constexpr auto
    operator!=(const adjacency_node_iterator& x, const adjacency_node_iterator& y) -> bool {
      return !(x==y);
    }
  private:
    adjacency_graph_type* g;
    index_type node_idx;
};


} // std_e
