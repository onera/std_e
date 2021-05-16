#pragma once


#include "std_e/graph/adjacency_graph/range_of_ranges.hpp"
#include <type_traits>
#include <vector>


namespace std_e {


// forward decl {
using default_adjacency_index_list_type = range_of_ranges<int,std::vector>;
template<class NT, class ET = void, class adj_list_type = default_adjacency_index_list_type> class adjacency_graph;
template<class NT, class ET = void, class adj_list_type = default_adjacency_index_list_type> class io_adjacency_graph;

template<class NT, class ET, class adj_list_type> class adjacency_graph_base;
template<class NT, class ET, class adj_list_type> class io_adjacency_graph_base;

template<class adjacency_graph_type> class adjacency_node_iterator;

template<class adjacency_graph_type> class adjacency;
template<class adjacency_graph_type> class io_adjacency;


enum class orientation {
  none,
  in_out,
};
enum class adj_orientation {
  none,
  in,
  out
};
template<class adjacency_graph_type, adj_orientation orientation = adj_orientation::none> class adjacency_range;
template<class adjacency_graph_type, adj_orientation orientation = adj_orientation::none> class adjacency_connection_iterator;
// forward decl }


// adjacency_graph_traits {
template<class graph_type>
struct adjacency_graph_traits;

template<template<class...> class Graph_type, class NT, class ET, class adj_list_type>
struct adjacency_graph_traits<Graph_type<NT,ET,adj_list_type>> {
  using adjacency_graph_type = Graph_type<NT,ET,adj_list_type>;

  using index_type          = value_type<adj_list_type>;
  using node_type           = NT;
  using edge_type           = ET;

  using adjacency_list_type = adj_list_type;
  using node_range_type     = range_from_outer<adj_list_type,NT>;
  using edge_range_type     = range_of_ranges_from<adj_list_type,ET>;

  constexpr auto operator<=>(const adjacency_graph_traits&) const = default;
};
template<template<class...> class Graph_type, class NT, class ET, class adj_list_type>
struct adjacency_graph_traits<const Graph_type<NT,ET,adj_list_type>> {
  using adjacency_graph_type = const Graph_type<NT,ET,adj_list_type>;

  using index_type          = value_type<adj_list_type>;
  using node_type           = const NT;
  using edge_type           = const ET;

  using adjacency_list_type = const adj_list_type;
  using node_range_type     = range_from_outer<adj_list_type,const NT>;
  using edge_range_type     = range_of_ranges_from<adj_list_type,const ET>;

  constexpr auto operator<=>(const adjacency_graph_traits&) const = default;
};
// adjacency_graph_traits }


} // std_e
