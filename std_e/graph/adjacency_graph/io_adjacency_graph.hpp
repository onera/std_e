#pragma once


#include "std_e/graph/adjacency_graph/adjacency_graph.hpp"


namespace std_e {


template<class NT, class ET = void, class adj_list_type = default_adjacency_index_list_type>
class io_adjacency_graph
  : public adjacency_graph_from_traits<adjacency_graph_traits<io_adjacency_graph<NT,ET,adj_list_type>>>
{};


// Node_adjacency interface {
template<class AGT> constexpr auto
node(adjacency<AGT>& adj) -> auto& {
  return adj.node();
}
template<class AGT> constexpr auto
node(const adjacency<AGT>& adj) -> const auto& {
  return adj.node();
}

// TODO rename out_nodes
template<class T> constexpr auto
children(io_adjacency<AGT>& adj) {
  return adj.out_adjacencies();
}
template<class T> constexpr auto
children(const adjacency<AGT>& adj) {
  return adj.out_adjacencies();
}
template<class T> constexpr auto
first_child(adjacency<AGT>& adj) {
  return adj.out_adjacencies().begin();
}
template<class T> constexpr auto
first_child(const adjacency<AGT>& adj) {
  return adj.out_adjacencies().begin();
}
template<class T> constexpr auto
last_child(adjacency<AGT>& adj) {
  return adj.out_adjacencies().end();
}
template<class T> constexpr auto
last_child(const adjacency<AGT>& adj) {
  return adj.out_adjacencies().end();
}

template<class adjacency_graph_type>
class rooted_graph {
  public:
    using index_type = typename adjacency_graph_type::index_type;
    using adjacency_iterator_type = typename adjacency_graph_type::adjacency_iterator_type;
    using const_adjacency_iterator_type = typename adjacency_graph_type::const_adjacency_iterator_type;

    constexpr
    rooted_graph(adjacency_graph_type* g, index_type root_idx)
      : g(g)
      , root_idx(root_idx)
    {}

    constexpr auto
    begin() -> adjacency_iterator_type {
      return {g,root_idx};
    }
    constexpr auto
    begin() const -> const_adjacency_iterator_type {
      return {g,root_idx};
    }
    constexpr auto
    end() -> adjacency_iterator_type {
      return {g,root_idx+1};
    }
    constexpr auto
    end() const -> const_adjacency_iterator_type {
      return {g,root_idx+1};
    }
  private:
  // data
    adjacency_graph_type* g;
    index_type root_idx;
};

// rooted graph view {
template<class AGT> auto
rooted_graph_view(AGT& g, int root_idx) -> rooted_graph<AGT> {
  return {g,root_idx};
}
// TODO
//template<class T> auto
//rooted_graph_view(const adjacency<AGT>& root) {
//  return io_adj_ptr_vector<T>{&root};
//}

template<class T> constexpr auto
first_root(rooted_graph<AGT>& root_g) {
  return root_g.begin();
}
template<class T> constexpr auto
first_root(const rooted_graph<AGT>& root_g) {
  return root_g.begin();
}
template<class T> constexpr auto
last_root(rooted_graph<AGT>& root_g) {
  return root_g.end();
}
template<class T> constexpr auto
last_root(const rooted_graph<AGT>& root_g) {
  return root_g.end();
}
// rooted graph view }


// algorithm {
template<class T> constexpr auto
make_bidirectional_from_outward_edges(io_adjacency_graph<T>& g) {
  int n_node = g.size();
  for (int i=0; i<n_node; ++i) {
    for (int k=0; k<g.out_degree(i); ++k) {
      int j = g[i].in_adjacency_list()[k];
      int e = g[i].edges()[k];
      g[j].in_adjacency_list().push_back(i);
      g[j].in_edges().push_back(e);
    }
  }
}
// algorithm }


//// TODO new interface names => report in concept + nested_tree...
//template<class T> constexpr auto
//out_adjacencies(adjacency<AGT>& adj) {
//  return std_e::make_dereferencing_range(adj.outwards);
//}
//template<class T> constexpr auto
//out_adjacencies(const adjacency<AGT>& adj) {
//  return std_e::make_dereferencing_range(adj.outwards);
//}
//template<class T> constexpr auto
//in_adjacencies(adjacency<AGT>& adj) {
//  return std_e::make_dereferencing_range(adj.inwards);
//}
//template<class T> constexpr auto
//in_adjacencies(const adjacency<AGT>& adj) {
//  return std_e::make_dereferencing_range(adj.inwards);
//}
//// Node_adjacency interface }
//
//template<class T> constexpr auto
//equal(const adjacency<AGT>& x, const adjacency<AGT>& y, const adjacency<AGT>* x_start, const adjacency<AGT>* y_start) -> bool {
//  return
//      x.node == y.node
//   && std_e::equal_iterator_shifts(x.inwards,y.inwards,x_start,y_start)
//   && std_e::equal_iterator_shifts(x.outwards,y.outwards,x_start,y_start);
//}
// io_adjacency }




//// algorithms {
//template<class T> constexpr auto
//is_reflexive_in_adjacency(const adjacency<AGT>& x) -> bool {
//  for (auto inward_ptr : x.inwards) {
//    auto& outs_of_in = inward_ptr->outwards;
//    auto x_pos = std::find(begin(outs_of_in),end(outs_of_in),&x);
//    if (x_pos==end(outs_of_in)) return false;
//  }
//  return true;
//}
//template<class T> constexpr auto
//is_reflexive_out_adjacency(const adjacency<AGT>& x) -> bool {
//  for (auto outward_ptr : x.outwards) {
//    auto& ins_of_out = outward_ptr->inwards;
//    auto x_pos = std::find(begin(ins_of_out),end(ins_of_out),&x);
//    if (x_pos==end(ins_of_out)) return false;
//  }
//  return true;
//}
//template<class T> constexpr auto
//is_bidirectional_adjacency(const adjacency<AGT>& x) -> bool {
//  return is_reflexive_in_adjacency(x) && is_reflexive_out_adjacency(x);
//}
//
//
///// redirect all edges entering "old" to now enter "new_adj"
//template<class T> constexpr auto
//redirect_entering_adjacencies(adjacency<AGT>* old, adjacency<AGT>* new_adj) -> void {
//  // Precondition: is_reflexive_in_adjacency(old)
//  // Post conditions:
//  //  - all edges previously entering "old" now enter "new_adj"
//  //  - !is_reflexive_in_adjacency(old):
//  //    - old.inwards.size()!=0
//  //    - adj_eq.inwards should become empty
//
//  // for each inward in old.inwards,
//  //     one element in inward.outwards points to old (by the precondition)
//  //         pick this one and make it point to new_adj
//  for (auto inward_ptr : old->inwards) {
//    auto& outs_of_in = inward_ptr->outwards;
//    auto first = outs_of_in.data();
//    auto last = outs_of_in.data()+outs_of_in.size();
//    auto old_pos = std::find(first,last,old);
//    STD_E_ASSERT(old_pos!=last);
//    *old_pos = new_adj;
//  }
//};
//// algorithms }


} // std_e
