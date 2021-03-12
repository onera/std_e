#pragma once


#include <ranges>


namespace std_e {


// adjacency_list_mixin {
template<class adj_list_type>
class adjacency_list_mixin {
  public:
    constexpr auto adjacency_list()       ->       adj_list_type  & { return adj_list; }
    constexpr auto adjacency_list() const -> const adj_list_type  & { return adj_list; }
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
    constexpr auto edges()       ->       edge_range_type& { return edges; }
    constexpr auto edges() const -> const edge_range_type& { return edges; }
  private:
    edge_range_type edges;
};

/// specialization when no edge values, the edge value is then the target node, and the value can't be changed
template<class adj_list_type>
class edge_mixin
  : adjacency_list_mixin<void,adj_list_type>
{
  public:
    constexpr auto edges() const -> const adj_list_type  & { return adjacency_list(); }
};
// edge_mixin }

// io_edge_mixin {
template<class ET, class adj_list_type>
class io_edge_mixin
  : adjacency_list_mixin<adj_list_type>
{
  public:
  // types
    using edge_range_type = range_of_ranges_from<adj_list_type,ET>;
  // interface
    constexpr auto in_edges ()       ->       edge_range_type& { return in_edges ; }
    constexpr auto in_edges () const -> const edge_range_type& { return in_edges ; }

    constexpr auto out_edges()       ->       edge_range_type& { return out_edges; }
    constexpr auto out_edges() const -> const edge_range_type& { return out_edges; }
  private:
    edge_range_type edges;
};

/// specialization when no edge values, the edge value is then the target node, and the value can't be changed
template<class adj_list_type>
class io_edge_mixin
  : adjacency_list_mixin<void,adj_list_type>
{
  public:
    constexpr auto in_edges () const -> const adj_list_type  & { return in_adjacency_list (); }
    constexpr auto out_edges() const -> const adj_list_type  & { return out_adjacency_list(); }
};
// io_edge_mixin }


// node_mixin {
template<class NT, class adj_list_type>
class node_mixin {
  public:
  // type traits
    using node_range_type = range_from_outer<adj_list_type,NT>;

  // interface
    constexpr auto nodes()       ->       node_range_type& { return nodes   ; }
    constexpr auto nodes() const -> const node_range_type& { return nodes   ; }
  private:
    node_range_type nodes;
};

/// specialization when no node values: the node value is then just [0,N), and the value can't be changed
template<class adj_list_type>
class node_mixin<void,adj_list_type> {
  public:
    constexpr auto nodes() const -> auto { return std::ranges::iota_view(0,sz); }
  private:
    index_type sz;
};
// node_mixin }


template<class NT, class ET, class adj_list_type>
class adjacency_graph_base
  : node_mixin<NT,adj_list_type>
  : edge_mixin<ET,adj_list_type>
{
  public:
    // Class invariant: adj_list.size() == nodes.size() == edges.size()
    // Class invariant: adj_list[i].size() == edges[i].size() for all i in [0,adj_list.size())
    constexpr auto size() const -> index_type { return nodes.size(); }

    constexpr auto degree(index_type i) const -> index_type { return adjacency_list()[i].size(); }
};

template<class NT, class ET, class adj_list_type>
class adjacency_graph_base
  : node_mixin<NT,adj_list_type>
  : io_edge_mixin<ET,adj_list_type>
{
  public:
    // Class invariant: size() == adj_list.size() == nodes.size() == edges.size()
    // Class invariant: [in|out]degree == [in|out]adj_list[i].size() == [in|out]edges[i].size() for all i in [0,[in|out]adj_list.size())
    constexpr auto size() const -> index_type { return nodes.size(); }

    constexpr auto in_degree (index_type i) const -> index_type { return in_adjacency_list ()[i].size(); }
    constexpr auto out_degree(index_type i) const -> index_type { return out_adjacency_list()[i].size(); }
};

} // std_e
