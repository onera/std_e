#pragma once


#include "std_e/graph/adjacency_graph/traits/traits.hpp"
#include "std_e/future/contract.hpp"


namespace std_e {


template<class adjacency_graph_type>
class adjacency_node_iterator {
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
    adjacency_node_iterator() = default;

    constexpr
    adjacency_node_iterator(adjacency_graph_type* g, index_type node_idx)
      : g(g)
      , node_idx(node_idx)
    {}

  // iterator interface
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


template<class AGT>
struct std::iterator_traits<std_e::adjacency_node_iterator<AGT>> {
  using type = std_e::adjacency_node_iterator<AGT>;
  using value_type = typename type::value_type;
  using reference = typename type::reference;
  using difference_type = typename type::difference_type;
  using iterator_category = typename type::iterator_category;
};
