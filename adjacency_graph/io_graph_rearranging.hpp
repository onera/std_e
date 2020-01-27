#pragma once


#include "graph/adjacency_graph/io_graph.hpp"
#include "std_e/future/reference_wrapper.hpp"
#include "std_e/algorithm/permutation.hpp"


namespace graph {


// io_adjacency_ref {
template<class T>
using io_adjacency_ref = std_e::reference_wrapper<io_adjacency<T>>;

/// Node_adjacency interface {
template<class T> constexpr auto
node(io_adjacency_ref<T>& x) -> T& {
  return node(x.get());
}
template<class T> constexpr auto
node(const io_adjacency_ref<T>& x) -> const T& {
  return node(x.get());
}
template<class T> constexpr auto
node2(io_adjacency_ref<T>& x) -> T& {
  return x.get().node;
}
template<class T> constexpr auto
node2(const io_adjacency_ref<T>& x) -> const T& {
  return x.get().node;
}
template<class T> constexpr auto
in_ptrs(io_adjacency_ref<T>& x) -> io_adj_ptr_vector<T>& {
  return in_ptrs(x.get());
}
template<class T> constexpr auto
in_ptrs(const io_adjacency_ref<T>& x) -> const io_adj_ptr_vector<T>& {
  return in_ptrs(x.get());
}
template<class T> constexpr auto
out_ptrs2(io_adjacency_ref<T>& x) -> io_adj_ptr_vector<T>& {
  return x.get().outwards;
}
template<class T> constexpr auto
out_ptrs2(const io_adjacency_ref<T>& x) -> const io_adj_ptr_vector<T>& {
  return x.get().outwards;
}
template<class T> constexpr auto
out_ptrs(io_adjacency_ref<T>& x) -> io_adj_ptr_vector<T>& {
  return out_ptrs(x.get());
}
template<class T> constexpr auto
out_ptrs(const io_adjacency_ref<T>& x) -> const io_adj_ptr_vector<T>& {
  return out_ptrs(x.get());
}

template<class T> constexpr auto
children(io_adjacency_ref<T>& x) {
  return children(x.get());
}
template<class T> constexpr auto
children(const io_adjacency_ref<T>& x) {
  return children(x.get());
}

template<class T> constexpr auto
out_adjacencies(io_adjacency_ref<T>& x) {
  return out_adjacencies(x.get());
}
template<class T> constexpr auto
out_adjacencies(const io_adjacency_ref<T>& x) {
  return out_adjacencies(x.get());
}
template<class T> constexpr auto
in_adjacencies(io_adjacency_ref<T>& x) {
  return in_adjacencies(x.get());
}
template<class T> constexpr auto
in_adjacencies(const io_adjacency_ref<T>& x) {
  return in_adjacencies(x.get());
}
/// Node_adjacency interface }
// io_adjacency_ref }


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
    using adj_ref_type = io_adjacency_ref<T>;

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
    resize(size_t sz) -> void {
      STD_E_ASSERT(sz <= size());
      adj_refs.resize(sz);
    }

    constexpr auto
    initial_size() const -> size_t {
      return g_ptr->size();
    }
    constexpr auto
    initial_start() const {
      return g_ptr->begin();
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
    old_index(int new_idx) const -> int {
      auto adj_iter_at_new_idx = std_e::get_pointer(adj_refs[new_idx]);
      return adj_iter_at_new_idx - initial_start();
    }
  // data members
    io_graph<T>* g_ptr; // TODO DEL
    std::vector<adj_ref_type> adj_refs;
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
    for (auto* out : x[i].outwards) {
      int index = out - x.initial_start();
      g[i].outwards.push_back( &g[old_to_new_positions[index]] );
    }
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
