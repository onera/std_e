#pragma once


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
  , public adjacency_graph_traits_type::adjacency_graph_base_type; // TODO merge in here or above ?
{
  public:
  // ctor
    constexpr
    adjacency_graph() = default;

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
      return {this,size()};
    }
    constexpr auto
    end() -> adjacency_iterator_type {
      return {this,size()};
    }
};


using default_adjacency_index_list_type = range_of_ranges<int,std::vector>;
template<class NT, class ET = void, class adj_list_type = default_adjacency_index_list_type>
class adjacency_graph
  : public adjacency_graph_from_traits<adjacency_graph_traits<adjacency_graph<NT,ET,adj_list_type>>>
{};


} // std_e
