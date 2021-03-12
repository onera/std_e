#pragma once


#include <ranges>


namespace std_e {


// nodes {
template<class NT, class adj_list_type>
class adjacency_list_node_mixin {
  public:
  // type traits
    using node_range_type = range_from_outer<adj_list_type,NT>;

  // interface
    constexpr auto size() const -> index_type { return nodes.size(); }
    constexpr auto nodes()       ->       node_range_type& { return nodes   ; }
    constexpr auto nodes() const -> const node_range_type& { return nodes   ; }
  private:
    node_range_type nodes;
};

/// specialization when no node values: the node value is then just [0,N), and the value can't be changed
template<class adj_list_type>
class adjacency_list_node_mixin<void,adj_list_type> {
  public:
    constexpr auto size() const -> index_type { return sz; }
    constexpr auto nodes() const -> auto { return std::ranges::iota_view(0,adjacency_list.size()); }
  private:
    index_type sz;
};
// nodes }


// connection {
template<class ET, class adj_list_type>
class adjacency_list_connection_mixin {
  public:
  // types
    using edge_range_type = range_of_ranges_from<adj_list_type,ET>;
  // interface
    constexpr auto adjacency_list()       ->       adj_list_type  & { return adj_list; }
    constexpr auto adjacency_list() const -> const adj_list_type  & { return adj_list; }
    constexpr auto edges()                ->       edge_range_type& { return edges   ; }
    constexpr auto edges()          const -> const edge_range_type& { return edges   ; }
  private:
  // data
    adj_list_type adj_list;
    edge_range_type edges;
};

// specialization when no edge values, the edge value is then the target node, and the value can't be changed
template<class adj_list_type>
class adjacency_list_connection_mixin<void,adj_list_type> {
  public:
  // interface
    constexpr auto adjacency_list()       ->       adj_list_type  & { return adj_list; }
    constexpr auto adjacency_list() const -> const adj_list_type  & { return adj_list; }
    constexpr auto edges()          const -> const adj_list_type  & { return adj_list; }
  private:
  // data
    adj_list_type adj_list;
};
// connection }


// io_connection {
template<class ET, class adj_list_type>
class adjacency_list_connection_mixin {
  // types
    using edge_range_type = range_of_ranges_from<adj_list_type,ET>;
  public:
    constexpr auto adjacency_list()       ->       adj_list_type  & { return adj_list; }
    constexpr auto adjacency_list() const -> const adj_list_type  & { return adj_list; }
    constexpr auto edges()                ->       edge_range_type& { return edges   ; }
    constexpr auto edges()          const -> const edge_range_type& { return edges   ; }
  private:
  // data
    adj_list_type adj_list;
    edge_range_type edges;
};

// specialization when no edge values, the edge value is then the target node, and the value can't be changed
template<class adj_list_type>
class adjacency_list_connection_mixin<void,adj_list_type> {
  public:
    constexpr auto adjacency_list()       ->       adj_list_type  & { return adj_list; }
    constexpr auto adjacency_list() const -> const adj_list_type  & { return adj_list; }
    constexpr auto edges()          const -> const adj_list_type  & { return adj_list; }
  private:
    adj_list_type adj_list;
};
// io_connection }

template<class NT, class ET, class adj_list_type>
class adjacency_graph_base
  : adjacency_list_node_mixin<NT,adj_list_type>
  : adjacency_list_connection_mixin<ET,adj_list_type>
{
  // Class invariant: adj_list.size() == nodes.size() == edges.size()
  // Class invariant: adj_list[i].size() == edges[i].size() for all i in [0,adj_list.size())
};

} // std_e
