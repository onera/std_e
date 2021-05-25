#pragma once


#include "std_e/graph/adjacency_graph/traits/traits.hpp"


namespace std_e {


template<class adjacency_graph_type, adj_orientation orientation>
class adjacency_connection_iterator {
  public:
  // type traits
    using index_type = typename adjacency_graph_type::index_type;
    using adjacency_type = adjacency_type_of<adjacency_graph_type>;

    /// std::iterator type traits
    using value_type = adjacency_type;
    using reference = adjacency_type;
    using difference_type = index_type;
    using iterator_category = std::forward_iterator_tag; // TODO random

  // ctors
    constexpr
    adjacency_connection_iterator() = default;

    constexpr
    adjacency_connection_iterator(adjacency_graph_type* g, index_type node_idx, index_type connection_idx)
      : g(g)
      , node_idx(node_idx)
      , connection_idx(connection_idx)
    {}

  // iterator interface
    constexpr auto
    operator++() -> adjacency_connection_iterator& {
      ++connection_idx;
      return *this;
    }

    constexpr auto
    operator*() const -> adjacency_type {
      return {g,index()};
    }

    friend constexpr auto
    operator==(const adjacency_connection_iterator& x, const adjacency_connection_iterator& y) -> bool {
      STD_E_ASSERT(x.g==y.g);
      STD_E_ASSERT(x.node_idx==y.node_idx);
      return x.connection_idx== y.connection_idx;
    }
    friend constexpr auto
    operator!=(const adjacency_connection_iterator& x, const adjacency_connection_iterator& y) -> bool {
      return !(x==y);
    }
  private:
  // functions
    constexpr auto
    index() const -> index_type {
      if constexpr (orientation==adj_orientation::none) {
        return g->index(node_idx,connection_idx);
      } else if constexpr (orientation==adj_orientation::in) {
        return g->in_index(node_idx,connection_idx);
      } else {
        static_assert(orientation==adj_orientation::out);
        return g->out_index(node_idx,connection_idx);
      }
    }
  // data
    adjacency_graph_type* g;
    index_type node_idx;
    index_type connection_idx;
};


} // std_e


template<class AGT, std_e::adj_orientation ori>
struct std::iterator_traits<std_e::adjacency_connection_iterator<AGT,ori>> {
  using type = std_e::adjacency_connection_iterator<AGT,ori>;
  using value_type = typename type::value_type;
  using reference = typename type::reference;
  using difference_type = typename type::difference_type;
  using iterator_category = typename type::iterator_category;
};
