#pragma once


#include "std_e/graph/adjacency_graph/traits/traits.hpp"
#include "std_e/future/contract.hpp"
#include <iterator>


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
    using iterator_category = std::random_access_iterator_tag;

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
    operator--() -> adjacency_node_iterator& {
      --node_idx;
      return *this;
    }

    constexpr auto
    operator++(int) -> adjacency_node_iterator {
      adjacency_node_iterator tmp(*this);
      ++(*this);
      return tmp;
    }
    constexpr auto
    operator--(int) -> adjacency_node_iterator {
      adjacency_node_iterator tmp(*this);
      --(*this);
      return tmp;
    }

    constexpr auto
    operator+=(index_type i) -> adjacency_node_iterator& {
      node_idx += i;
      return *this;
    }
    constexpr auto
    operator-=(index_type i) -> adjacency_node_iterator& {
      node_idx -= i;
      return *this;
    }

    friend constexpr auto
    operator+(const adjacency_node_iterator& x, index_type i) -> adjacency_node_iterator {
      return {x.g,x.node_idx+i};
    }
    friend constexpr auto
    operator-(const adjacency_node_iterator& x, index_type i) -> adjacency_node_iterator {
      return {x.g,x.node_idx-i};
    }

    friend constexpr auto
    operator+(index_type i, const adjacency_node_iterator& x) -> adjacency_node_iterator {
      return x+i;
    }
    friend constexpr auto
    operator-(const adjacency_node_iterator& x, const adjacency_node_iterator& y) -> index_type {
      STD_E_ASSERT(x.g==y.g);
      return x.node_idx - y.node_idx;
    }

    constexpr auto
    operator*() const -> adjacency_type {
      return {g,node_idx};
    }
    constexpr auto
    operator[](index_type i) const -> adjacency_type {
      return {g,node_idx+i};
    }

    friend constexpr auto
    operator==(const adjacency_node_iterator& x, const adjacency_node_iterator& y) {
      STD_E_ASSERT(x.g==y.g);
      return x.node_idx == y.node_idx;
    }
    friend constexpr auto
    operator<=>(const adjacency_node_iterator& x, const adjacency_node_iterator& y) {
      STD_E_ASSERT(x.g==y.g);
      return x.node_idx <=> y.node_idx;
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
