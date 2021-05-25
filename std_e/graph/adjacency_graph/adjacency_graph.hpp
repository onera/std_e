#pragma once


#include <vector>
#include "std_e/graph/adjacency_graph/traits/traits.hpp"
#include "std_e/graph/adjacency_graph/adjacency_graph_base.hpp"
#include "std_e/graph/adjacency_graph/adjacency.hpp"
#include "std_e/graph/adjacency_graph/adjacency_range.hpp"
#include "std_e/graph/adjacency_graph/adjacency_node_iterator.hpp"
#include "std_e/graph/adjacency_graph/adjacency_connection_iterator.hpp"


namespace std_e {


namespace detail {
  template<orientation ori, class... Ts>
  struct associated_adj_graph_types;

  template<class... Ts>
  struct associated_adj_graph_types<orientation::none,Ts...> {
    using adj_type = adjacency<Ts...>;
    using graph_base_type = adjacency_graph_base<Ts...>;
  };
  template<class... Ts>
  struct associated_adj_graph_types<orientation::in_out,Ts...> {
    using adj_type = io_adjacency<Ts...>;
    using graph_base_type = io_adjacency_graph_base<Ts...>;
  };
}
template<class graph_type, orientation ori>
using associated_adjacency_type = typename detail::associated_adj_graph_types<ori,graph_type>::type;



template<class graph_base, orientation ori>
class adjacency_graph_from_traits
  : public graph_base // TODO merge in here or above ?
{
    using base = graph_base;
    using this_type = adjacency_graph_from_traits<graph_base,ori>;

    using index_type = typename graph_base::index_type;

    using adjacency_type = associated_adjacency_type<this_type,ori>;
    using const_adjacency_type = associated_adjacency_type<const this_type,ori>;

    using adjacency_iterator_type = adjacency_node_iterator<this_type>;
    using const_adjacency_iterator_type = adjacency_node_iterator<const this_type>;
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


// end-user classes {
/// NOTE: they could be aliases instead of aliases,
///       but being proper classes, the name appears
///       in compiler error messages, and this more readable
template<class NT, class ET, class adj_list_type>
class adjacency_graph
  : public
      adjacency_graph_from_traits<
        adjacency_graph_base<NT,ET,adj_list_type>,
        orientation::none
      >
{};

template<class NT, class ET, class adj_list_type>
class io_adjacency_graph
  : public
      adjacency_graph_from_traits<
        io_adjacency_graph_base<NT,ET,adj_list_type>,
        orientation::in_out
      >
{};
// end-user classes }



} // std_e
