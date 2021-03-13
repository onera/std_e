#pragma once


#include <ranges>
#include "std_e/graph/adjacency_graph/range_of_ranges.hpp"
// TODO
// nodes: we may want to store in SoA fashion
//   example: node_value = tuple<int,double>
//   then node_range_type ~ multi_range
// edges:
//   optionnally bundle with connection indices
//     would allow 4 range_of_ranges (two for [in/out]_adj_list, two for [in/out]_edges
//     to be replaced by 1 range_of_4_ranges (where 2 ins have same size, and 2 out same size)
//        -> link with nodes ~ multi_range (index+edges)
//   do force bundle, also support the compressed array data structure
//     note that for now, if its kinda possible to support compressed, its sub-optimal since the same index_array is repeated twice


namespace std_e {


// adjacency_list_mixin {
template<class adj_list_type>
class adjacency_list_mixin {
  public:
    constexpr auto adjacency_list()       ->       adj_list_type  & { return adj_list; }
    constexpr auto adjacency_list() const -> const adj_list_type  & { return adj_list; }

    friend auto operator==(const adjacency_list_mixin& x, const adjacency_list_mixin& y) -> bool = default;
  private:
    adj_list_type adj_list;
};

template<class adj_list_type>
class io_adjacency_list_mixin {
  public:
    constexpr auto in_adjacency_list ()       ->       adj_list_type  & { return in_adj_list ; }
    constexpr auto in_adjacency_list () const -> const adj_list_type  & { return in_adj_list ; }

    constexpr auto out_adjacency_list()       ->       adj_list_type  & { return out_adj_list; }
    constexpr auto out_adjacency_list() const -> const adj_list_type  & { return out_adj_list; }

    friend auto operator==(const io_adjacency_list_mixin& x, const io_adjacency_list_mixin& y) -> bool = default;
  private:
    adj_list_type in_adj_list;
    adj_list_type out_adj_list;
};
// adjacency_list_mixin }


// edge_mixin {
template<class ET, class adj_list_type>
class edge_mixin
  : adjacency_list_mixin<adj_list_type>
{
  public:
  // types
    using edge_range_type = range_of_ranges_from<adj_list_type,ET>;
  // interface
    constexpr auto edges()       ->       edge_range_type& { return es; }
    constexpr auto edges() const -> const edge_range_type& { return es; }

    friend auto operator==(const edge_mixin& x, const edge_mixin& y) -> bool = default;
  private:
    edge_range_type es;
};

/// specialization when no edge values, the edge value is then the target node, and the value can't be changed
template<class adj_list_type>
class edge_mixin<void,adj_list_type>
  : adjacency_list_mixin<adj_list_type>
{
  public:
    constexpr auto edges() const -> const adj_list_type  & { return this->adjacency_list(); }
};
// edge_mixin }

// io_edge_mixin {
template<class ET, class adj_list_type>
class io_edge_mixin
  : public io_adjacency_list_mixin<adj_list_type>
{
  public:
  // types
    using edge_range_type = range_of_ranges_from<adj_list_type,ET>;
  // interface
    constexpr auto in_edges ()       ->       edge_range_type& { return in_es ; }
    constexpr auto in_edges () const -> const edge_range_type& { return in_es ; }

    constexpr auto out_edges()       ->       edge_range_type& { return out_es; }
    constexpr auto out_edges() const -> const edge_range_type& { return out_es; }

    friend auto operator==(const io_edge_mixin& x, const io_edge_mixin& y) -> bool = default;
  private:
    edge_range_type in_es;
    edge_range_type out_es;
};

/// specialization when no edge values, the edge value is then the target node, and the value can't be changed
template<class adj_list_type>
class io_edge_mixin<void,adj_list_type>
  : public io_adjacency_list_mixin<adj_list_type>
{
  public:
    constexpr auto in_edges () const -> const adj_list_type  & { return this->in_adjacency_list (); }
    constexpr auto out_edges() const -> const adj_list_type  & { return this->out_adjacency_list(); }
};
// io_edge_mixin }


// node_mixin {
template<class NT, class adj_list_type>
class node_mixin {
  public:
  // type traits
    using node_range_type = range_from_outer<adj_list_type,NT>;
  // interface
    constexpr auto nodes()       ->       node_range_type& { return ns   ; }
    constexpr auto nodes() const -> const node_range_type& { return ns   ; }

    friend auto operator==(const node_mixin& x, const node_mixin& y) -> bool = default;
  private:
    node_range_type ns;
};

/// specialization when no node values: the node value is then just [0,N), and the value can't be changed
template<class adj_list_type>
class node_mixin<void,adj_list_type> {
    using index_type = value_type<adj_list_type>;
  public:
    constexpr auto nodes() const -> auto { return std::ranges::iota_view(0,sz); }
  private:
    index_type sz;
};
// node_mixin }


template<class NT, class ET, class adj_list_type>
class adjacency_graph_base
  : public node_mixin<NT,adj_list_type>
  , public edge_mixin<ET,adj_list_type>
{
    using index_type = value_type<adj_list_type>;
  public:
    // Class invariant: adj_list.size() == nodes.size() == edges.size()
    // Class invariant: adj_list[i].size() == edges[i].size() for all i in [0,adj_list.size())
    constexpr auto size() const -> index_type { return this->nodes.size(); }

    constexpr auto degree(index_type i) const -> index_type { return this->adjacency_list()[i].size(); }
};

template<class NT, class ET, class adj_list_type>
class io_adjacency_graph_base
  : public node_mixin<NT,adj_list_type>
  , public io_edge_mixin<ET,adj_list_type>
{
    using index_type = value_type<adj_list_type>;
  public:
    // Class invariant: size() == adj_list.size() == nodes.size() == edges.size()
    // Class invariant: [in|out]degree == [in|out]adj_list[i].size() == [in|out]edges[i].size() for all i in [0,[in|out]adj_list.size())
    constexpr auto size() const -> index_type { return this->nodes().size(); }

    constexpr auto in_degree (index_type i) const -> index_type { return this->in_adjacency_list ()[i].size(); }
    constexpr auto out_degree(index_type i) const -> index_type { return this->out_adjacency_list()[i].size(); }

    friend auto operator==(const io_adjacency_graph_base& x, const io_adjacency_graph_base& y) -> bool = default;
};

} // std_e
