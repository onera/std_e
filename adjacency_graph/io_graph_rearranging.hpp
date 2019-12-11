#pragma once


#include "graph/adjacency_graph/io_graph.hpp"
#include "std_e/future/reference_wrapper.hpp"
#include "std_e/algorithm/permutation.hpp"
#include "std_e/future/constexpr_quick_sort.hpp"
#include <iostream> // TODO


namespace graph {


/** 
*  class "io_graph_rearranging_view"
*
*  Directly changing adjacency positions when reordering a graph has to be done in two steps:
*    - change position of the adjacency
*    - update its position for its inward and outward adj_refs
*  Plus, doing it for each reordering can be expansive,
*  since we also need to change all ins and outs connectivities
*
*  The idea here is to keep a permutation table and to always access adj_refs through its indirect indexing
*/

template<class T>
class io_graph_rearranging_view {
  public:
    using adj_type = io_adjacency<T>;
    using adj_iterator_type = io_adjacency<T>*;
    using ref_wrapper_type = std_e::reference_wrapper<adj_type>;

    using node_type = T;

    constexpr
    io_graph_rearranging_view(io_graph<T>& g)
      : g_ptr(&g)
      , adj_refs(g.begin(),g.end())
    {}

    constexpr auto
    size() const -> size_t {
      return adj_refs.size();
    }
    constexpr auto
    initial_size() const -> size_t {
      return g_ptr->size();
    }
    constexpr auto
    resize(size_t sz) -> void {
      STD_E_ASSERT(sz <= size());
      adj_refs.resize(sz);
    }

    constexpr auto
    begin() {
      return adj_refs.begin();
    }
    constexpr auto
    begin() const {
      return adj_refs.begin();
    }
    constexpr auto
    end() {
      return adj_refs.end();
    }
    constexpr auto
    end() const {
      return adj_refs.end();
    }

    constexpr auto
    operator[](int i) const -> const adj_type& {
      return adj_refs[i];
    }
    constexpr auto
    operator[](int i) -> adj_type& {
      return adj_refs[i];
    }

    constexpr auto
    old_to_new_positions() const {
      std::vector<int> old_to_new(initial_size());

      int sz = size();
      for (int i=0; i<sz; ++i) {
        old_to_new[old_index(i)] = i;
      }
      return old_to_new;
    }
  private:
  public: //TODO
  // member functions
    constexpr auto
    underlying_graph() const -> const io_graph<T>& {
      return *g_ptr;
    }
    constexpr auto
    old_index(int new_idx) const -> int {
      auto adj_iter_at_new_idx = std_e::get_pointer(adj_refs[new_idx]);
      auto first_adj_iter = underlying_graph().begin();
      return adj_iter_at_new_idx - first_adj_iter;
    }
  // data members
    io_graph<T>* g_ptr; // TODO DEL
    std::vector<ref_wrapper_type> adj_refs;
};

template<class T> constexpr auto
begin(io_graph_rearranging_view<T>& x) {
  return x.begin();
}
template<class T> constexpr auto
begin(const io_graph_rearranging_view<T>& x) {
  return x.begin();
}
template<class T> constexpr auto
end(io_graph_rearranging_view<T>& x) {
  return x.end();
}
template<class T> constexpr auto
end(const io_graph_rearranging_view<T>& x) {
  return x.end();
}


template<class T> constexpr auto
make_rearranging_view(io_graph<T>& g) {
  return io_graph_rearranging_view<T>(g);
}

template<class array_type, class T> constexpr auto
propagate_outward_edges(const io_graph_rearranging_view<T>& x, const array_type& old_to_new_positions, io_graph<T>& g) {
  int sz = x.size();
  for (int i=0; i<sz; ++i) {
    auto& old_outs = x[i].outwards;
    int sz_out = old_outs.size();
    auto& new_outs = g[i].outwards;
    new_outs.resize(sz_out);
    auto* old_start = x.underlying_graph().begin();
    auto* start = begin(g);
    std::transform(begin(old_outs),end(old_outs),begin(new_outs),[&](const auto& old_out){
      int index = old_out - old_start;
      return start + old_to_new_positions[index];
    });

    //for (auto* out : x[i].outwards) {
    //  int index = out - x.underlying_graph().begin(); // TODO
    //  g[i].outwards.push_back( &g[old_to_new_positions[index]] ); // TODO
    //}
  }
}


template<class T> constexpr auto
bidirectional_graph_from_outward_edges(const io_graph_rearranging_view<T>& x) -> io_graph<T> {
  int sz = x.size();

  io_graph<T> res(sz);
  for (int i=0; i<sz; ++i) {
    res[i].node = x[i].node;
  }

  propagate_outward_edges(x,x.old_to_new_positions(),res);
  make_bidirectional_from_outward_edges(res);
  return res;
}


} // graph
