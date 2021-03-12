#pragma once


#include "std_e/graph/adjacency_graph/adjacency_graph_base.hpp"
#include "std_e/graph/adjacency_graph/traits/traits.hpp"


namespace std_e {


//using default_adjacency_index_list_type = range_of_ranges<int,std::vector>;

template<class adjacency_graph_traits_type>
class adjacency_graph_from_traits
  : public adjacency_graph_traits_type
  , public adjacency_graph_traits_type::adjacency_graph_base_type;
{
  public:
  // ctor
    constexpr
    adjacency_graph() = default;

  // range interface
    constexpr auto
    size() const -> index_type {
      return nodes().size();
    }

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


template<class adjacency_graph_type>
class adjacency {
  public:
    using node_range_type  = adjacency_graph_traits<adjacency_graph_type>::node_range_type;
    using index_type       = adjacency_graph_traits<adjacency_graph_type>::index_type;
    using node_type        = adjacency_graph_traits<adjacency_graph_type>::node_type;

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
    adjacencies() -> adjacency_range<const adjacency_graph_type> {
      return {g,node_idx};
    }
  private:
    adjacency_graph_type* g;
    index_type node_idx;
};


template<class adjacency_graph_type>
class adjacency_range {
  public:
    using index_type     = adjacency_graph_traits<adjacency_graph_type>::index_type;
    using node_type      = adjacency_graph_traits<adjacency_graph_type>::node_type;
    using adjacency_type = adjacency_graph_traits<adjacency_graph_type>::adjacency_type;

    constexpr
    adjacency_range() = default;

    constexpr
    adjacency_range(adjacency_graph_type* g, index_type node_idx)
      , g(g)
      : node_idx(node_idx)
    {}

    constexpr auto
    size() const -> index_type {
      return g->adjacency_list()[node_idx].size();
    }

    constexpr auto
    begin() -> adjacency_iterator_type {
      return {g,g->adjacency_list()[node_idx][0]};
    }
    constexpr auto
    begin() const -> const_adjacency_iterator_type {
      return {g,g->adjacency_list()[node_idx][0]};
    }
    constexpr auto
    end() -> adjacency_iterator_type {
      return {g,g->adjacency_list()[node_idx][size()]};
    }
    constexpr auto
    end() const -> const_adjacency_iterator_type {
      return {g,g->adjacency_list()[node_idx][size()]};
    }

    constexpr auto
    operator[](index_type i) -> adjacency_type {
      return {g,g->adjacency_list()[node_idx][i]};
    }
    constexpr auto
    operator[](index_type i) const -> const_adjacency_type {
      return {g,g->adjacency_list()[node_idx][i]};
    }
  private:
    adjacency_graph_type* g;
    index_type node_idx;
};


template<class adjacency_graph_type>
class adjacency_iterator {
  public:
    using adjacency_type = typename adjacency_graph_traits<adjacency_graph_type>::adjacency_type;

    constexpr
    adjacency_iterator() = default;

    constexpr
    adjacency_iterator(adjacency_graph_type* g, index_type node_idx)
      : g(g)
      , node_idx(node_idx)
    {}

    constexpr auto
    operator++() -> adjacency_iterator& {
      ++node_idx;
      return *this;
    }

    constexpr auto
    operator*() const -> adjacency_type {
      return {g,node_idx};
    }

    friend constexpr auto
    operator==(const adjacency_iterator& x, const adjacency_iterator& y) -> bool {
      return x.node_idx == y.node_idx;
    }
    friend constexpr auto
    operator!=(const adjacency_iterator& x, const adjacency_iterator& y) -> bool {
      return !(x==y);
    }
  private:
    adjacency_graph_type* g;
    index_type node_idx;
};


} // std_e
