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
  template<class graph_type, orientation ori>
  struct associated_adjacency_type;

  template<class graph_type>
  struct associated_adjacency_type<graph_type,orientation::none> {
    using type = adjacency<graph_type>;
  };
  template<class graph_type>
  struct associated_adjacency_type<graph_type,orientation::in_out> {
    using type = io_adjacency<graph_type>;
  };
}
template<class graph_type, orientation ori>
using associated_adjacency_type = typename detail::associated_adjacency_type<graph_type,ori>::type;



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
