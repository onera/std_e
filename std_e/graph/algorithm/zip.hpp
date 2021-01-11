#pragma once


#include "std_e/future/zip.hpp"


namespace std_e {


template<class G0, class G1>
using graph_zip_view = zpair<G0,G1>;

template<class T>
struct is_graph_zip_view__impl {
  static constexpr bool value = false;
};
template<class G0, class G1>
struct is_graph_zip_view__impl<graph_zip_view<G0,G1>> {
  static constexpr bool value = true;
};
template<class T>
constexpr bool is_graph_zip_view = is_graph_zip_view__impl<std::decay_t<T>>::value;


template<class graph_zip_view_type, std::enable_if_t<is_graph_zip_view<graph_zip_view_type>,int> =0> auto
first_root(graph_zip_view_type&& g) {
  return make_zip_iterator(first_root(g.first),first_root(g.second));
}
template<class graph_zip_view_type, std::enable_if_t<is_graph_zip_view<graph_zip_view_type>,int> =0> auto
last_root(graph_zip_view_type&& g) {
  return make_zip_iterator( last_root(g.first), last_root(g.second));
}
template<class graph_zip_view_type, std::enable_if_t<is_graph_zip_view<graph_zip_view_type>,int> =0> auto
first_child(graph_zip_view_type&& g) {
  return make_zip_iterator(first_child(g.first),first_child(g.second));
}
template<class graph_zip_view_type, std::enable_if_t<is_graph_zip_view<graph_zip_view_type>,int> =0> auto
last_child(graph_zip_view_type&& g) {
  return make_zip_iterator( last_child(g.first), last_child(g.second));
}


template<class G0, class G1> auto
zip_graphs(G0 g0, G1 g1) {
  return graph_zip_view<G0,G1>(g0,g1);
}


} // std_e

