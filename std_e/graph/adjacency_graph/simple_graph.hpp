#pragma once


#include <ranges>
#include "std_e/data_structure/range_of_ranges.hpp"
#include "std_e/utils/to_string.hpp"
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
  // type traits
    using index_type = value_type<adj_list_type>;
  // ctor
    constexpr adjacency_list_mixin() = default;
    constexpr adjacency_list_mixin(index_type sz) : adj_list(sz) {}
  // interface
    constexpr auto indices()                                           ->       auto& { return adj_list;                 }
    constexpr auto indices()                                     const -> const auto& { return adj_list;                 }
    constexpr auto indices(index_type i_node)                          ->       auto& { return adj_list[i_node];         }
    constexpr auto indices(index_type i_node)                    const -> const auto& { return adj_list[i_node];         }
    constexpr auto index  (index_type i_node, index_type i_edge)       ->       auto& { return adj_list[i_node][i_edge]; }
    constexpr auto index  (index_type i_node, index_type i_edge) const -> const auto& { return adj_list[i_node][i_edge]; }

    constexpr auto degree(index_type i) const -> index_type { return this->indices(i).size(); }

    auto operator<=>(const adjacency_list_mixin& x) const = default;
  private:
    adj_list_type adj_list;
};

template<class adj_list_type>
class io_adjacency_list_mixin {
  public:
  // type traits
    using index_type = value_type<adj_list_type>;
  // ctor
    constexpr io_adjacency_list_mixin() = default;
    constexpr io_adjacency_list_mixin(index_type sz) : in_adj_list(sz), out_adj_list(sz) {}
  // interface
    constexpr auto in_indices()                                           ->       auto& { return in_adj_list;                 }
    constexpr auto in_indices()                                     const -> const auto& { return in_adj_list;                 }
    constexpr auto in_indices(index_type i_node)                          ->       auto& { return in_adj_list[i_node];         }
    constexpr auto in_indices(index_type i_node)                    const -> const auto& { return in_adj_list[i_node];         }
    constexpr auto in_index  (index_type i_node, index_type i_edge)       ->       auto& { return in_adj_list[i_node][i_edge]; }
    constexpr auto in_index  (index_type i_node, index_type i_edge) const -> const auto& { return in_adj_list[i_node][i_edge]; }

    constexpr auto out_indices()                                           ->       auto& { return out_adj_list;                 }
    constexpr auto out_indices()                                     const -> const auto& { return out_adj_list;                 }
    constexpr auto out_indices(index_type i_node)                          ->       auto& { return out_adj_list[i_node];         }
    constexpr auto out_indices(index_type i_node)                    const -> const auto& { return out_adj_list[i_node];         }
    constexpr auto out_index  (index_type i_node, index_type i_edge)       ->       auto& { return out_adj_list[i_node][i_edge]; }
    constexpr auto out_index  (index_type i_node, index_type i_edge) const -> const auto& { return out_adj_list[i_node][i_edge]; }

    constexpr auto in_degree (index_type i) const -> index_type { return this->in_indices ()[i].size(); }
    constexpr auto out_degree(index_type i) const -> index_type { return this->out_indices()[i].size(); }

    auto operator<=>(const io_adjacency_list_mixin& x) const = default;
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

    auto operator<=>(const edge_mixin& x) const = default;
  private:
    edge_range_type es;
};

/// specialization when no edge values, the edge value is then the target node, and the value can't be changed
template<class adj_list_type>
class edge_mixin<void,adj_list_type>
  : adjacency_list_mixin<adj_list_type>
{
  public:
    constexpr auto edges() const -> const adj_list_type  & { return this->indices(); }

    auto operator<=>(const edge_mixin& x) const = default;
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
    using index_type = value_type<adj_list_type>;
  // ctor
    constexpr io_edge_mixin() = default;
    constexpr io_edge_mixin(index_type sz): io_adjacency_list_mixin<adj_list_type>(sz), in_es(sz), out_es(sz) {}
  // interface
    constexpr auto in_edges ()       ->       edge_range_type& { return in_es ; }
    constexpr auto in_edges () const -> const edge_range_type& { return in_es ; }

    constexpr auto out_edges()       ->       edge_range_type& { return out_es; }
    constexpr auto out_edges() const -> const edge_range_type& { return out_es; }

    auto operator<=>(const io_edge_mixin& x) const = default;
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
  // type traits
    using index_type = value_type<adj_list_type>;
  // ctor
    constexpr io_edge_mixin() = default;
    constexpr io_edge_mixin(index_type sz): io_adjacency_list_mixin<adj_list_type>(sz) {}
  // interface
    constexpr auto in_edges () const -> const adj_list_type  & { return this->in_indices (); }
    constexpr auto out_edges() const -> const adj_list_type  & { return this->out_indices(); }

    auto operator<=>(const io_edge_mixin& x) const = default;
};
// io_edge_mixin }


// node_mixin {
template<class NT, class adj_list_type>
class node_mixin {
  public:
  // type traits
    using index_type = value_type<adj_list_type>;
    using node_range_type = range_from_outer<adj_list_type,NT>;
  // ctor
    constexpr node_mixin() = default;
    constexpr node_mixin(index_type sz): ns(sz) {}
  // interface
    constexpr auto nodes()                  ->       auto& { return ns; }
    constexpr auto nodes()            const -> const auto& { return ns; }
    constexpr auto node(index_type i)       ->       auto& { return ns[i]; }
    constexpr auto node(index_type i) const -> const auto& { return ns[i]; }

    auto operator<=>(const node_mixin& x) const = default;
  private:
    node_range_type ns;
};

/// specialization when no node values: the node value is then just [0,N), and the value can't be changed
template<class adj_list_type>
class node_mixin<void,adj_list_type> {
  public:
  // type traits
    using index_type = value_type<adj_list_type>;
  // ctor
    constexpr node_mixin() = default;
    constexpr node_mixin(index_type sz): sz(sz) {}
  // interface
    constexpr auto nodes() const -> auto { return std::ranges::iota_view(0,sz); }

    auto operator<=>(const node_mixin& x) const = default;
  private:
    index_type sz;
};
// node_mixin }


template<class NT, class ET, class adj_list_type>
class simple_graph
  : public node_mixin<NT,adj_list_type>
  , public edge_mixin<ET,adj_list_type>
{
  public:
    using index_type = value_type<adj_list_type>;

    // Class invariant: adj_list.size() == nodes.size() == edges.size()
    // Class invariant: adj_list[i].size() == edges[i].size() for all i in [0,adj_list.size())
    constexpr auto size() const -> index_type { return this->nodes.size(); }

    auto operator<=>(const simple_graph& x) const = default;
};

template<class NT, class ET, class adj_list_type>
class io_simple_graph
  : public node_mixin<NT,adj_list_type>
  , public io_edge_mixin<ET,adj_list_type>
{
  public:
    using index_type = value_type<adj_list_type>;

    // Class invariant: size() == adj_list.size() == nodes.size() == edges.size()
    // Class invariant: [in|out]degree == [in|out]adj_list[i].size() == [in|out]edges[i].size() for all i in [0,[in|out]adj_list.size())
    constexpr io_simple_graph() = default;
    constexpr io_simple_graph(index_type sz)
      : node_mixin<NT,adj_list_type>(sz)
      , io_edge_mixin<ET,adj_list_type>(sz)
    {}

    constexpr auto size() const -> index_type { return this->nodes().size(); }

    auto operator<=>(const io_simple_graph& x) const = default;
};


template<class NT, class ET, class ALT> auto
to_string(const io_simple_graph<NT,ET,ALT>& x) {
  return
      "nodes: " + std_e::range_to_string(x.nodes()) + '\n'
    + "in_indices:" + std_e::range_to_string(x.in_indices()) + '\n'
    + "out_indices:" + std_e::range_to_string(x.out_indices()) + '\n';
}

} // std_e
