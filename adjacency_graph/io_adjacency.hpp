#pragma once


#include "graph/constexpr_max_sizes.hpp"
#include "std_e/future/constexpr_vector.hpp"
#include "std_e/utils/array_iterator.hpp" // TODO DEL?
#include "std_e/utils/iterator_range.hpp"
#include "std_e/utils/dereferencing_range.hpp"


namespace graph {


// connection containers {
using connection_indices_container = std_e::constexpr_vector<int,Max_nb_adj_connections>;

template<class T> struct io_adjacency;
template<class T> using io_adjacency_iterator = io_adjacency<T>*;
template<class T> using const_io_adjacency_iterator = const io_adjacency<T>*;
//template<class T> using io_adjacency_iterator = array_iterator<io_adjacency<T>*>;
//template<class T> using const_io_adjacency_iterator = array_iterator<const io_adjacency<T>*>;
template<class T> using connections_container = std_e::constexpr_vector<io_adjacency_iterator<T>,Max_nb_adj_connections>;

template<class T> constexpr auto
to_connection_indices_container(const connections_container<T>& x, const io_adjacency<T>* start) -> connection_indices_container {
  int sz = x.size();
  connection_indices_container x_indices(sz);
  for (int i=0; i<sz; ++i) {
    x_indices[i] = x[i] - start;
  }
  return x_indices;
}
template<class T> constexpr auto
to_connections_container(const connection_indices_container& x_indices, io_adjacency<T>* start) -> connections_container<T> {
  int sz = x_indices.size();
  connections_container<T> x(sz);
  for (int j=0; j<sz; ++j) {
    //x[j] = {start , x_indices[j]}; // TODO
    x[j] = start + x_indices[j];
  }
  return x;
}
template<class T> constexpr auto
to_connections_container(const connections_container<T>& old, const io_adjacency<T>* old_start, io_adjacency<T>* start) -> connections_container<T> {
  int sz = old.size();
  connections_container<T> x(sz);
  for (int j=0; j<sz; ++j) {
    //int index = old[j].ptr - old_start; //TODO
    int index = old[j] - old_start;
    //x[j] = {start , index}; // TODO
    x[j] = start + index;
  }
  return x;
}

template<class T> constexpr auto
equal(const connections_container<T>& x, const connections_container<T>& y, const_io_adjacency_iterator<T> x_start, const_io_adjacency_iterator<T> y_start) -> bool { // TODO
  if (x.size() != y.size()) return false;
  int sz = x.size();
  for (int j=0; j<sz; ++j) {
    int x_index = x[j] - x_start;
    int y_index = y[j] - y_start;
    if (x_index != y_index) return false;
  }
  return true;
}
// connection containers }


// io_index_adjacency {
template<class T>
struct io_index_adjacency {
  T node;
  connection_indices_container inwards;
  connection_indices_container outwards;
};
template<class T> using io_index_adjacency_list = std_e::constexpr_vector<io_index_adjacency<T>,Max_graph_size>;
// io_index_adjacency }


// io_adjacency {
template<class T>
struct io_adjacency {
  constexpr
  io_adjacency()
    : node(T())
    , index(0)
  {}

  constexpr
  io_adjacency(T node, connections_container<T> inwards, connections_container<T> outwards, int index)
    : node(std::move(node))
    , inwards(std::move(inwards))
    , outwards(std::move(outwards))
    , index(index)
  {}
  constexpr
  io_adjacency(const io_index_adjacency<T>& x, io_adjacency<T>* start, int i)
    : node(x.node)
    , inwards(to_connections_container(x.inwards,start))
    , outwards(to_connections_container(x.outwards,start))
    , index(i)
  {}
  constexpr
  io_adjacency(const io_adjacency<T>& x, const io_adjacency<T>* old_start, io_adjacency<T>* start, int i)
    : node(x.node)
    , inwards(to_connections_container(x.inwards,old_start,start))
    , outwards(to_connections_container(x.outwards,old_start,start))
    , index(i)
  {}

  T node;
  connections_container<T> inwards;
  connections_container<T> outwards;

  // Sometimes, pointer differences are not resolved as constexpr by gcc
  // (This is a gcc <= 9 bug [works with clang 10])
  // To work around it, node adjacency must store where they are in a sequence
  // This is not a very bad workaround, but it prevents the copy/move ctors/assigment op to do the right thing most of the time
  int index; // TODO
};
template<class T> using io_adjacency_list = std_e::constexpr_vector<io_adjacency<T>,Max_graph_size>;

// Node_adjacency interface {
template<class T> constexpr auto
node(io_adjacency<T>& adj) -> T& {
  return adj.node;
}
template<class T> constexpr auto
node(const io_adjacency<T>& adj) -> const T& {
  return adj.node;
}

// TODO rename out_nodes
template<class T> constexpr auto
children(io_adjacency<T>& adj) {
  return std_e::make_dereferencing_range(adj.outwards);
}
template<class T> constexpr auto
children(const io_adjacency<T>& adj) {
  return std_e::make_dereferencing_range(adj.outwards);
}
// Node_adjacency interface }

template<class T> constexpr auto
equal(const io_adjacency<T>& x, const io_adjacency<T>& y, const_io_adjacency_iterator<T> x_start, const_io_adjacency_iterator<T> y_start) -> bool { // TODO
  if (x.node != y.node) return false;

  return equal(x.inwards,y.inwards,x_start,y_start)
      && equal(x.outwards,y.outwards,x_start,y_start);
}
// io_adjacency }




// algorithms {
template<class T> constexpr auto
is_reflexive_in_adjacency(const io_adjacency<T>& x) -> bool {
  for (auto inward_ptr : x.inwards) {
    auto& outs_of_in = inward_ptr->outwards;
    auto x_pos = std_e::find(begin(outs_of_in),end(outs_of_in),&x);
    if (x_pos==end(outs_of_in)) return false;
  }
  return true;
}
template<class T> constexpr auto
is_reflexive_out_adjacency(const io_adjacency<T>& x) -> bool {
  for (auto outward_ptr : x.outwards) {
    auto& ins_of_out = outward_ptr->inwards;
    auto x_pos = std_e::find(begin(ins_of_out),end(ins_of_out),&x);
    if (x_pos==end(ins_of_out)) return false;
  }
  return true;
}
template<class T> constexpr auto
is_bidirectional_adjacency(const io_adjacency<T>& x) -> bool {
  return is_reflexive_in_adjacency(x) && is_reflexive_out_adjacency(x);
}


/// redirect all edges entering "old" to now enter "new_adj"
template<class T> constexpr auto
redirect_entering_adjacencies(io_adjacency_iterator<T>& old, io_adjacency_iterator<T>& new_adj) -> void {
  // Precondition: is_reflexive_in_adjacency(old)
  // Post conditions:
  //  - all edges previously entering "old" now enter "new_adj"
  //  - !is_reflexive_in_adjacency(old):
  //    - old.inwards.size()!=0 
  //    - adj_eq.inwards should become empty

  // for each inward in old.inwards,
  //     one element in inward.outwards points to old (by the precondition)
  //         pick this one and make it point to new_adj
  for (auto inward_ptr : old->inwards) {
    auto& outs_of_in = inward_ptr->outwards;
    //auto old_pos = std_e::find(begin(outs_of_in),end(outs_of_in),&old);
    auto* old_pos = std_e::find_if(begin(outs_of_in),end(outs_of_in),[old](const auto& n_it){ return n_it->index==old->index; });
    std_e::ASSERT(old_pos!=end(outs_of_in));
    *old_pos = new_adj;
  }
};
// algorithms }


} // graph
