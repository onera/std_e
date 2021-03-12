#pragma once


namespace std_e {


// forward decl {
template<class NT, class ET, class adj_list_type> class adjacency_graph_base<NT,ET,adj_list_type>;
template<class NT, class ET, class adj_list_type> class adjacency_graph<NT,ET,adj_list_type>;
template<class NT, class ET, class adj_list_type> class io_adjacency_graph<NT,ET,adj_list_type>;

template<class adjacency_graph_type> class adjacency;
template<class adjacency_graph_type> class adjacency_iterator;
template<class adjacency_graph_type> class adjacency_range;

template<class adjacency_graph_type> class io_adjacency;
template<class adjacency_graph_type> class io_adjacency_iterator;
template<class adjacency_graph_type> class io_adjacency_range;
// forward decl }


// adjacency_graph_base_traits {
template<class adjacency_graph_base_type>
struct adjacency_graph_base_traits;

template<class NT, class ET, class adj_list_type>
struct adjacency_graph_base_traits<adjacency_graph_base<NT,ET,adj_list_type>> {
  using adjacency_graph_base_type = adjacency_graph_base<NT,ET,adj_list_type>;

  using index_type          = inner_range_type<adj_list_type>;
  using node_type           = NT;
  using edge_type           = ET;

  using adjacency_list_type = adj_list_type;
  using node_range_type     = range_from_outer<adj_list_type,NT>;
  using edge_range_type     = range_of_ranges_from<adj_list_type,ET>;
};
template<class NT, class ET, class adj_list_type>
struct adjacency_graph_base_traits<const adjacency_graph_base<NT,ET,adj_list_type>> {
  using adjacency_graph_base_type = const adjacency_graph_base<NT,ET,adj_list_type>;

  using index_type          = inner_range_type<adj_list_type>;
  using node_type           = const NT;
  using edge_type           = const ET;

  using adjacency_list_type = const adj_list_type;
  using node_range_type     = range_from_outer<adj_list_type,const NT>;
  using edge_range_type     = range_of_ranges_from<adj_list_type,const ET>;
};
// adjacency_graph_base_traits }


// adjacency_graph_traits {
template<class adjacency_graph_type>
struct adjacency_graph_traits;

template<class NT, class ET, class adj_list_type>
struct adjacency_graph_traits<adjacency_graph<NT,ET,adj_list_type>> {
  : adjacency_graph_base_traits<adjacency_graph_base<NT,ET,adj_list_type> 
{
  using adjacency_graph_type = adjacency_graph<NT,ET,adj_list_type>;

  using adjacency_type = adjacency<adjacency_graph_type>;
  using const_adjacency_type = adjacency<const adjacency_graph_type>;
  using adjacency_iterator_type = adjacency_iterator<adjacency_graph_type>;
  using const_adjacency_iterator_type = adjacency_iterator<const adjacency_graph_type>;
  using adjacency_range_type = adjacency_range<adjacency_graph_type>;
  using const_adjacency_range_type = adjacency_range<const adjacency_graph_type>;
};
template<class NT, class ET, class adj_list_type>
struct adjacency_graph_traits<const adjacency_graph<NT,ET,adj_list_type>> {
  : adjacency_graph_base_traits<const adjacency_graph_base<NT,ET,adj_list_type> 
{
  using adjacency_graph_type = const adjacency_graph<NT,ET,adj_list_type>;

  using adjacency_type = adjacency<adjacency_graph_type>;
  using const_adjacency_type = adjacency<const adjacency_graph_type>;
  using adjacency_iterator_type = adjacency_iterator<adjacency_graph_type>;
  using const_adjacency_iterator_type = adjacency_iterator<const adjacency_graph_type>;
  using adjacency_range_type = adjacency_range<adjacency_graph_type>;
  using const_adjacency_range_type = adjacency_range<const adjacency_graph_type>;
};
// adjacency_graph_traits }


// io_adjacency_graph_traits {
template<class adjacency_graph_type>
struct io_adjacency_graph_traits;

template<class NT, class ET, class adj_list_type>
struct io_adjacency_graph_traits<io_adjacency_graph<NT,ET,adj_list_type>> {
  : adjacency_graph_base_traits<adjacency_graph_base<NT,ET,adj_list_type> 
{
  using adjacency_graph_type = io_adjacency_graph<NT,ET,adj_list_type>;

  using adjacency_type = io_adjacency<adjacency_graph_type>;
  using const_adjacency_type = io_adjacency<const adjacency_graph_type>;
  using adjacency_iterator_type = io_adjacency_iterator<adjacency_graph_type>;
  using const_adjacency_iterator_type = io_adjacency_iterator<const adjacency_graph_type>;
  using adjacency_range_type = io_adjacency_range<adjacency_graph_type>;
  using const_adjacency_range_type = io_adjacency_range<const adjacency_graph_type>;
};
template<class NT, class ET, class adj_list_type>
struct io_adjacency_graph_traits<const io_adjacency_graph<NT,ET,adj_list_type>> {
  : adjacency_graph_base_traits<const adjacency_graph_base<NT,ET,adj_list_type> 
{
  using adjacency_type = io_adjacency<adjacency_graph_type>;
  using const_adjacency_type = io_adjacency<const adjacency_graph_type>;
  using adjacency_iterator_type = io_adjacency_iterator<adjacency_graph_type>;
  using const_adjacency_iterator_type = io_adjacency_iterator<const adjacency_graph_type>;
  using adjacency_range_type = io_adjacency_range<adjacency_graph_type>;
  using const_adjacency_range_type = io_adjacency_range<const adjacency_graph_type>;
};
// adjacency_graph_traits }


} // std_e
