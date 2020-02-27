#pragma once


#include <vector>
#include <deque>
#include "std_e/future/contract.hpp" 
#include "std_e/utils/iterator_range.hpp"
#include "std_e/utils/dereferencing_range.hpp"
#include "std_e/algorithm/iterator.hpp"


namespace graph {


// connection containers {
using connection_indices_container = std::vector<int>;

template<class T> struct io_adjacency;
template<class T> using io_adj_ptr_vector = std::vector<io_adjacency<T>*>;
// connection containers }


// io_index_adjacency {
template<class T>
struct io_index_adjacency {
  T node;
  connection_indices_container inwards;
  connection_indices_container outwards;
};
template<class T> using io_index_adjacency_vector = std::vector<io_index_adjacency<T>>;
// io_index_adjacency }


// io_adjacency {
template<class T>
struct io_adjacency {
  constexpr io_adjacency() = default;
  constexpr io_adjacency(io_adjacency&&) = default;
  constexpr io_adjacency(const io_adjacency&) = default;
  constexpr io_adjacency& operator=(io_adjacency&&) = default;
  constexpr io_adjacency& operator=(const io_adjacency&) = default;

  constexpr
  io_adjacency(T node, io_adj_ptr_vector<T> inwards, io_adj_ptr_vector<T> outwards)
    : node(std::move(node))
    , inwards(std::move(inwards))
    , outwards(std::move(outwards))
  {}
  constexpr
  io_adjacency(const io_index_adjacency<T>& x, io_adjacency<T>* start)
    : node(x.node)
    , inwards(std_e::indices_to_iterators(x.inwards,start))
    , outwards(std_e::indices_to_iterators(x.outwards,start))
  {}
  constexpr
  io_adjacency(const io_adjacency<T>& x, const io_adjacency<T>* old_start, io_adjacency<T>* start)
    : node(x.node)
    , inwards(std_e::iterators_to_iterators(x.inwards,old_start,start))
    , outwards(std_e::iterators_to_iterators(x.outwards,old_start,start))
  {}

  T node;
  io_adj_ptr_vector<T> inwards;
  io_adj_ptr_vector<T> outwards;
};
template<class T> using io_adjacency_vector = std::vector<io_adjacency<T>>;
template<class T> using io_adjacency_deque = std::deque<io_adjacency<T>>;

// Node_adjacency interface {
template<class T> constexpr auto
node(io_adjacency<T>& adj) -> T& {
  return adj.node;
}
template<class T> constexpr auto
node(const io_adjacency<T>& adj) -> const T& {
  return adj.node;
}
// TODO new interface names => report in concept + nested_tree...
template<class T> constexpr auto
in_ptrs(io_adjacency<T>& adj) -> io_adj_ptr_vector<T>& {
  return adj.inwards;
}
template<class T> constexpr auto
in_ptrs(const io_adjacency<T>& adj) -> const io_adj_ptr_vector<T>& {
  return adj.inwards;
}
template<class T> constexpr auto
out_ptrs(io_adjacency<T>& adj) -> io_adj_ptr_vector<T>& {
  return adj.outwards;
}
template<class T> constexpr auto
out_ptrs(const io_adjacency<T>& adj) -> const io_adj_ptr_vector<T>& {
  return adj.outwards;
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

// TODO new interface names => report in concept + nested_tree...
template<class T> constexpr auto
out_adjacencies(io_adjacency<T>& adj) {
  return std_e::make_dereferencing_range(adj.outwards);
}
template<class T> constexpr auto
out_adjacencies(const io_adjacency<T>& adj) {
  return std_e::make_dereferencing_range(adj.outwards);
}
template<class T> constexpr auto
in_adjacencies(io_adjacency<T>& adj) {
  return std_e::make_dereferencing_range(adj.inwards);
}
template<class T> constexpr auto
in_adjacencies(const io_adjacency<T>& adj) {
  return std_e::make_dereferencing_range(adj.inwards);
}
// Node_adjacency interface }

template<class T> constexpr auto
equal(const io_adjacency<T>& x, const io_adjacency<T>& y, const io_adjacency<T>* x_start, const io_adjacency<T>* y_start) -> bool {
  return 
      x.node == y.node
   && std_e::equal_iterator_shifts(x.inwards,y.inwards,x_start,y_start)
   && std_e::equal_iterator_shifts(x.outwards,y.outwards,x_start,y_start);
}
// io_adjacency }




// algorithms {
template<class T> constexpr auto
is_reflexive_in_adjacency(const io_adjacency<T>& x) -> bool {
  for (auto inward_ptr : x.inwards) {
    auto& outs_of_in = inward_ptr->outwards;
    auto x_pos = std::find(begin(outs_of_in),end(outs_of_in),&x);
    if (x_pos==end(outs_of_in)) return false;
  }
  return true;
}
template<class T> constexpr auto
is_reflexive_out_adjacency(const io_adjacency<T>& x) -> bool {
  for (auto outward_ptr : x.outwards) {
    auto& ins_of_out = outward_ptr->inwards;
    auto x_pos = std::find(begin(ins_of_out),end(ins_of_out),&x);
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
redirect_entering_adjacencies(io_adjacency<T>* old, io_adjacency<T>* new_adj) -> void {
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
    auto first = outs_of_in.data();
    auto last = outs_of_in.data()+outs_of_in.size();
    auto old_pos = std::find(first,last,old);
    STD_E_ASSERT(old_pos!=last);
    *old_pos = new_adj;
  }
};
// algorithms }


} // graph
