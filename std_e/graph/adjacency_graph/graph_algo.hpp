#pragma once


#include "std_e/graph/adjacency_graph/graph.hpp"
#include "std_e/graph/adjacency_graph/index_adjacency.hpp"


namespace std_e {


template<class NT> auto
make_graph(const index_adjacency_vector<NT>& idx_adjs) {
  graph<NT> res;
  for (const auto& [node,ins] : idx_adjs) {
    res.nodes().push_back(node);
    res.indices().push_back(ins);
  }
  return res;
}
template<class NT> auto
make_io_graph(const io_index_adjacency_vector<NT>& idx_adjs) {
  io_graph<NT> res;
  for (const auto& [node,ins,out] : idx_adjs) {
    res.nodes().push_back(node);
    res.in_indices().push_back(ins);
    res.out_indices().push_back(out);
  }
  return res;
}


// algorithm {
template<class T> constexpr auto
make_bidirectional_from_outward_edges(io_graph<T>& g) {
  int n_node = g.size();
  for (int i=0; i<n_node; ++i) {
    for (int k=0; k<g.out_degree(i); ++k) {
      int j = g.out_index(i,k);
      //int e = g.out_edges()[i][k]; // TODO
      g.in_indices(j).push_back(i);
      //g.in_edges()[j].push_back(e); // TODO
    }
  }
}
// algorithm }


//// TODO new interface names => report in concept + nested_tree...
template<class GT> constexpr auto
out_adjacencies(adjacency<GT>& adj) {
  return adj.out_adjacencies();
}
template<class GT> constexpr auto
out_adjacencies(const adjacency<GT>& adj) {
  return adj.out_adjacencies();
}
template<class GT> constexpr auto
in_adjacencies(adjacency<GT>& adj) {
  return adj.in_adjacencies();
}
template<class GT> constexpr auto
in_adjacencies(const adjacency<GT>& adj) {
  return adj.in_adjacencies();
}
//// Node_adjacency interface }
//
//template<class T> constexpr auto
//equal(const adjacency<GT>& x, const adjacency<GT>& y, const adjacency<GT>* x_start, const adjacency<GT>* y_start) -> bool {
//  return
//      x.node == y.node
//   && std_e::equal_iterator_shifts(x.inwards,y.inwards,x_start,y_start)
//   && std_e::equal_iterator_shifts(x.outwards,y.outwards,x_start,y_start);
//}
// io_adjacency }




//// algorithms {
//template<class T> constexpr auto
//is_reflexive_in_adjacency(const adjacency<GT>& x) -> bool {
//  for (auto inward_ptr : x.inwards) {
//    auto& outs_of_in = inward_ptr->outwards;
//    auto x_pos = std::find(begin(outs_of_in),end(outs_of_in),&x);
//    if (x_pos==end(outs_of_in)) return false;
//  }
//  return true;
//}
//template<class T> constexpr auto
//is_reflexive_out_adjacency(const adjacency<GT>& x) -> bool {
//  for (auto outward_ptr : x.outwards) {
//    auto& ins_of_out = outward_ptr->inwards;
//    auto x_pos = std::find(begin(ins_of_out),end(ins_of_out),&x);
//    if (x_pos==end(ins_of_out)) return false;
//  }
//  return true;
//}
//template<class T> constexpr auto
//is_bidirectional_adjacency(const adjacency<GT>& x) -> bool {
//  return is_reflexive_in_adjacency(x) && is_reflexive_out_adjacency(x);
//}
//
//
///// redirect all edges entering "old" to now enter "new_adj"
//template<class T> constexpr auto
//redirect_entering_adjacencies(adjacency<GT>* old, adjacency<GT>* new_adj) -> void {
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
