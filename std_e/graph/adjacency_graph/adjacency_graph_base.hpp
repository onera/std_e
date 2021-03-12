#pragma once


#include <ranges>


namespace std_e {


template<class NT, class ET, class adj_list_type>
class adjacency_graph_base {
  private:
  // types
    using node_range_type = range_from_outer<adj_list_type,NT>;
    using edge_range_type = range_of_ranges_from<adj_list_type,ET>;
  // data
    adj_list_type adj_list;
    node_range_type nodes;
    edge_range_type edges;
  public:
    // Class invariant: adj_list.size() == nodes.size() == edges.size()
    // Class invariant: adj_list[i].size() == edges[i].size() for all i in [0,adj_list.size())
    constexpr auto adjacency_list()       ->       adj_list_type  & { return adj_list; }
    constexpr auto adjacency_list() const -> const adj_list_type  & { return adj_list; }
    constexpr auto nodes()                ->       node_range_type& { return nodes   ; }
    constexpr auto nodes()          const -> const node_range_type& { return nodes   ; }
    constexpr auto edges()                ->       edge_range_type& { return edges   ; }
    constexpr auto edges()          const -> const edge_range_type& { return edges   ; }
};

// specialization when no edge values, the edge value is then the target node, and the value can't be changed
template<class NT, class adj_list_type>
class adjacency_graph_base<NT,void,adj_list_type> {
  private:
  // types
    using node_range_type = range_from_outer<adj_list_type,NT>;
  // data
    adj_list_type adj_list;
    node_range_type nodes;
  public:
    constexpr auto adjacency_list()       ->       adj_list_type  & { return adj_list; }
    constexpr auto adjacency_list() const -> const adj_list_type  & { return adj_list; }
    constexpr auto nodes()                ->       node_range_type& { return nodes   ; }
    constexpr auto nodes()          const -> const node_range_type& { return nodes   ; }
    constexpr auto edges()          const -> const adj_list_type  & { return adj_list; }
};

// specialization when no node values: the node value is then just [0,N), and the value can't be changed
template<class ET, class adj_list_type>
class adjacency_graph_base<void,ET,adj_list_type> {
  private:
  // types
    using edge_range_type = range_of_ranges_from<adj_list_type,ET>;
  // data
    adj_list_type adj_list;
    edge_range_type edges;
  public:
    constexpr auto adjacency_list()       ->       adj_list_type  & { return adj_list; }
    constexpr auto adjacency_list() const -> const adj_list_type  & { return adj_list; }
    constexpr auto nodes()          const -> auto                   { return std::ranges::iota_view(0,adjacency_list.size()); }
    constexpr auto edges()                ->       edge_range_type& { return edges   ; }
    constexpr auto edges()          const -> const edge_range_type& { return edges   ; }
};

// specialization when no node and no edge values
template<class adj_list_type>
class adjacency_graph_base<void,void,adj_list_type> {
  private:
  // types
    using edge_range_type = origin_target_view<adj_list_type>;
  // data
    adj_list_type adj_list;
  public:
    constexpr auto adjacency_list()       ->       adj_list_type  & { return adj_list; }
    constexpr auto adjacency_list() const -> const adj_list_type  & { return adj_list; }
    constexpr auto nodes()          const -> auto                   { return std::ranges::iota_view(0,adjacency_list.size()); }
    constexpr auto edges()          const -> const adj_list_type  & { return adj_list; }
};


} // std_e
