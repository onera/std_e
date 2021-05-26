#pragma once


#include <vector>
#include "std_e/graph/adjacency_graph/traits/traits.hpp"
#include "std_e/graph/adjacency_graph/adjacency_graph_base.hpp"
#include "std_e/graph/adjacency_graph/adjacency.hpp"
#include "std_e/graph/adjacency_graph/adjacency_range.hpp"
#include "std_e/graph/adjacency_graph/adjacency_node_iterator.hpp"
#include "std_e/graph/adjacency_graph/adjacency_connection_iterator.hpp"


namespace std_e {


// adjacency_graph associated types {
namespace detail {
  template<orientation ori>
  struct associated_adj_graph_types;

  template<>
  struct associated_adj_graph_types<orientation::none> {
    template<class graph_type> using adj_type        = adjacency<graph_type>;
    template<class... Ts>      using graph_base_type = adjacency_graph_base<Ts...>;
  };
  template<>
  struct associated_adj_graph_types<orientation::in_out> {
    template<class graph_type> using adj_type        = io_adjacency<graph_type>;
    template<class... Ts>      using graph_base_type = io_adjacency_graph_base<Ts...>;
  };
}
template<class graph_type, orientation ori>
using associated_adjacency_type = typename detail::associated_adj_graph_types<ori>::template adj_type<graph_type>;

template<class NT, class ET, class ALT, orientation ori>
using associated_graph_base = typename detail::associated_adj_graph_types<ori>::template graph_base_type<NT,ET,ALT>;
// adjacency_graph associated types }


template<class derived, class NT, class ET, class ALT, orientation ori>
class adjacency_graph_from_traits
  : public associated_graph_base<NT,ET,ALT,ori>
{
  public:
    using base = associated_graph_base<NT,ET,ALT,ori>;

    using node_type = NT;
    using edge_type = ET;
    using adjacency_list_type = ALT;
    using index_type = value_type<ALT>;

    using adjacency_type = associated_adjacency_type<derived,ori>;
    using const_adjacency_type = associated_adjacency_type<const derived,ori>;

    using adjacency_node_iterator_type = adjacency_node_iterator<derived>;
    using const_adjacency_node_iterator_type = adjacency_node_iterator<const derived>;

    //using adjacency_connection_iterator_type = adjacency_connection_iterator<derived>;
    //using const_adjacency_connection_iterator_type = adjacency_connection_iterator<const derived>;
  // ctor
    constexpr
    adjacency_graph_from_traits() = default;

    constexpr
    adjacency_graph_from_traits(index_type sz)
      : base(sz)
    {}

  // range interface
    constexpr auto
    operator[](index_type i) -> adjacency_type {
      return {graph(),i};
    }
    constexpr auto
    operator[](index_type i) const -> const_adjacency_type {
      return {graph(),i};
    }

    constexpr auto
    begin() const -> const_adjacency_node_iterator_type {
      return {graph(),0};
    }
    constexpr auto
    begin() -> adjacency_node_iterator_type {
      return {graph(),0};
    }
    constexpr auto
    end() const -> const_adjacency_node_iterator_type {
      return {graph(),this->size()};
    }
    constexpr auto
    end() -> adjacency_node_iterator_type {
      return {graph(),this->size()};
    }

    auto operator<=>(const adjacency_graph_from_traits& x) const = default;
  private:
    auto
    graph() {
      return static_cast<derived*>(this);
    }
    auto
    graph() const {
      return static_cast<const derived*>(this);
    }
};

// end-user classes {
/// NOTE: We could use aliases instead creating new types by derivation,
///       but being proper classes, the name appears
///       in compiler error messages, and this is more readable
///           This implies the use of the infamous CRTP
///           Hope it can be replaced by strong types
template<class NT, class ET, class adj_list_type>
class adjacency_graph
  : public
      adjacency_graph_from_traits<
        adjacency_graph<NT,ET,adj_list_type>,
        NT,ET,adj_list_type,
        orientation::none
      >
{};

template<class NT, class ET, class adj_list_type>
class io_adjacency_graph
  : public
      adjacency_graph_from_traits<
        io_adjacency_graph<NT,ET,adj_list_type>,
        NT,ET,adj_list_type,
        orientation::in_out
      >
{};
// end-user classes }


} // std_e
