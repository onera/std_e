#pragma once


#include <vector>
#include "std_e/graph/adjacency_graph/traits/traits.hpp"
#include "std_e/graph/adjacency_graph/adjacency_graph_base.hpp"
#include "std_e/graph/adjacency_graph/adjacency.hpp"
#include "std_e/graph/adjacency_graph/adjacency_range.hpp"
#include "std_e/graph/adjacency_graph/adjacency_node_iterator.hpp"
#include "std_e/graph/adjacency_graph/adjacency_connection_iterator.hpp"


namespace std_e {


template<class adjacency_graph_traits_type>
class adjacency_graph_from_traits
  : public adjacency_graph_traits_type
  , public adjacency_graph_traits_type::adjacency_graph_base_type // TODO merge in here or above ?
{
    using base = typename adjacency_graph_traits_type::adjacency_graph_base_type;
    using index_type = typename adjacency_graph_traits_type::index_type;
    using adjacency_type = typename adjacency_graph_traits_type::adjacency_type;
    using const_adjacency_type = typename adjacency_graph_traits_type::const_adjacency_type;

    using this_type = adjacency_graph_from_traits<adjacency_graph_traits_type>;
    using adjacency_iterator_type = adjacency_node_iterator<this_type>;
    using const_adjacency_iterator_type = const adjacency_node_iterator<this_type>;
  public:
  // ctor
    constexpr
    adjacency_graph_from_traits() = default;

  // range interface
    constexpr auto
    operator[](index_type i) -> adjacency_type {
      return {this,i};
    }
    constexpr auto
    operator[](index_type i) const -> const_adjacency_type {
      return {this,i};
    }

    constexpr auto
    begin() const -> const_adjacency_iterator_type {
      return {this,0};
    }
    constexpr auto
    begin() -> adjacency_iterator_type {
      return {this,0};
    }
    constexpr auto
    end() const -> const_adjacency_iterator_type {
      return {this,this->size()};
    }
    constexpr auto
    end() -> adjacency_iterator_type {
      return {this,this->size()};
    }

    friend auto
    operator==(const adjacency_graph_from_traits& x, const adjacency_graph_from_traits& y) -> bool = default;
};


template<class NT, class ET, class adj_list_type>
class adjacency_graph
  : public adjacency_graph_from_traits<adjacency_graph_traits<adjacency_graph<NT,ET,adj_list_type>>>
{};


} // std_e
