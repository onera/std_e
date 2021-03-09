#pragma once


#include <vector>
#include <deque>
//#include <ranges>
#include "std_e/future/contract.hpp"
#include "std_e/iterator/iterator_range.hpp"
#include "std_e/utils/dereferencing_range.hpp"
#include "std_e/algorithm/iterator.hpp"


namespace graph {

// ror_adjacency_list {
// ror means range of ranges
template<template<class> class Node_range, template<class> class Adj_range = Node_range, class Int = int>
using ror_adjacency_list = Node_range<Adj_range<int>>;

/// traits {
//// index_type {
template<class adjacency_list_type>
class index_type__impl;

template<template<class> class NR, template<class> class AR, class Int>
class index_type__impl<ror_adjacency_list<NR,AR,Int>> {
  using type = Int;
}
template<class adjacency_list_type>
using index_type = typename index_type__impl<adjacency_list_type>::type;
//// index_type }

//// adjacency_index_range_type {
template<class adjacency_list_type>
class adjacency_index_range_type__impl;

template<template<class> class NR, template<class> class AR, class Int>
class adjacency_index_range_type__impl<ror_adjacency_list<NR,AR,Int>> {
  using type = AR<Int>;
}
template<class adjacency_list_type>
using adjacency_index_range_type = typename adjacency_index_range_type__impl<adjacency_list_type>::type;
//// adjacency_index_range_type }

/// traits }
// ror_adjacency_list }


template<class NT, class adj_list_type = ror_adjacency_list<std::vector>>
class adjacency_graph {
  public:
    using this_type = adjacency_graph<NT,adj_list_type>;
    using node_type = NT;
    using adjacency_list_type = adj_list_type;
    using index_type = typename adjacency_list_type::index_type;
    //using adjacency_index_range_type = typename adjacency_list_type::adjacency_index_range_type;
    using node_range_type = std::vector<NT>; // TODO use any container
    template<class T0> friend class adjacency;

    using adjacency_type = adjacency<this_type>;
    using const_adjacency_type = adjacency<const this_type>;

    // Class invariant: adj_list.size() = nodes.size()
    constexpr
    adjacency_graph() = default;

    constexpr auto
    size() const -> index_type {
      STD_E_ASSERT(adj_list.size() == nodes.size());
      return nodes.size();
    }

    constexpr auto
    operator[](index_type i) -> adjacency_type {
      return {this,i};
    }
    constexpr auto
    operator[](index_type i) const -> const_adjacency_type {
      return {this,i};
    }
  private:
    adjacency_list_type adj_list;
    node_range_type nodes;
};


template<class adjacency_graph_type>
class adjacency {
  public:
    using index_type = typename adjacency_graph_type::index_type;
    using adjacency_index_range_type = typename adjacency_graph_type::adjacency_index_range_type;
    using node_type = typename adjacency_graph_type::node_type;
    using node_range_type = typename adjacency_graph_type::node_range_type;

    constexpr
    adjacency() = default;

    constexpr
    adjacency(adjacency_graph_type* g, index_type node_idx)
      : g(g)
      , node_idx(node_idx)
    {}

    constexpr auto
    node() -> node_type& {
      return g->nodes[node_idx];
    }
    constexpr auto
    node() const -> const node_type& {
      return g->nodes[node_idx];
    }

    constexpr auto
    adjacencies() -> adjacency_range<adjacency_graph_type> {
      return {g,node_idx};
    }
    constexpr auto
    adjacencies() -> adjacency_range<const adjacency_graph_type> {
      return {g,node_idx};
    }
  private:
    adjacency_graph_type* g;
    index_type node_idx;
};

template<class adjacency_graph_type>
class adjacency_range {
  public:
    using index_type = typename adjacency_graph_type::index_type;
    using node_type = typename adjacency_graph_type::node_type;

    constexpr
    adjacency_range() = default;

    constexpr
    adjacency_range(adjacency_graph_type* g, index_type node_idx)
      , g(g)
      : node_idx(node_idx)
    {}

    constexpr auto
    size() const -> index_type {
      return g->size();
    }

    constexpr auto
    operator[](index_type i) -> adjacency_type {
      return {g,g->adj_list[node_idx][i]};
    }
    constexpr auto
    operator[](index_type i) const -> const_adjacency_type {
      return {g,g->adj_list[node_idx][i]};
    }
  private:
    adjacency_graph_type* g;
    index_type node_idx;
};

////////////////////////////////

template<class NT, class adj_list_type = ror_adjacency_list<std::vector>>
class io_adjacency_graph
  : public io_adjacency_graph<NT,adj_list_type>
{
  public:
    using base = io_adjacency_graph<NT,adj_list_type>;
    using base::base;

    using io_partition_index_range_type = std::vector<int>; // TODO use any container
  private:
    io_partition_index_range_type io_partition_idx_rng; // outward, then inward
};

enum class adj_orientation {
  out,
  in
};

template<class io_adjacency_graph_type>
class io_adjacency 
  : public adjacency<io_adjacency_graph_type>
{
  public:
    constexpr auto
    outward_adjacencies() -> io_adjacency_range<adjacency_graph_type,adj_orientation::out> {
      return {g,node_idx};
    }
    constexpr auto
    outward_adjacencies() -> io_adjacency_range<const adjacency_graph_type,adj_orientation::out> {
      return {g,node_idx};
    }
  private:
};

template<class io_adjacency_graph_type, adj_orientation orientation>
class io_adjacency_range {
    constexpr
    io_adjacency_range() = default;

    constexpr
    io_adjacency_range(io_adjacency_graph_type* g, index_type node_idx)
      , g(g)
      : node_idx(node_idx)
    {}

    constexpr auto
    size() const -> index_type {
      constexpr if (orientation==adj_orientation::out) {
        return out_size();
      } else {
        return in_size();
      }
    }

    constexpr auto
    operator[](index_type i) -> adjacency_type {
      constexpr if (orientation==adj_orientation::out) {
        return {g,g->adj_list[node_idx][i]};
      } else {
        return {g,g->adj_list[node_idx][out_size()+i]};
      }
    }
    constexpr auto
    operator[](index_type i) const -> const_adjacency_type {
      constexpr if (orientation==adj_orientation::out) {
        return {g,g->adj_list[node_idx][i]};
      } else {
        return {g,g->adj_list[node_idx][out_size()+i]};
      }
    }
  private:
  // functions
    constexpr auto
    out_size() const -> index_type {
      return g->io_partition_idx_rng[node_idx];
    }
    constexpr auto
    in_size() const -> index_type {
      return g->io_partition_idx_rng[node_idx].size() - g->io_partition_idx_rng[node_idx];
    }
  // data
    io_adjacency_graph_type* g;
    index_type node_idx;
}


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
children(adjacency<AGT>& adj) {
  return
}
template<class T> constexpr auto
children(const adjacency<AGT>& adj) {
  return std_e::make_dereferencing_range(adj.outwards);
}
template<class T> constexpr auto
first_child(const adjacency<AGT>& adj) {
  return std_e::make_dereferencing_iterator(begin(adj.outwards));
}
template<class T> constexpr auto
first_child(adjacency<AGT>& adj) {
  return std_e::make_dereferencing_iterator(begin(adj.outwards));
}
template<class T> constexpr auto
last_child(const adjacency<AGT>& adj) {
  return std_e::make_dereferencing_iterator(end(adj.outwards));
}
template<class T> constexpr auto
last_child(adjacency<AGT>& adj) {
  return std_e::make_dereferencing_iterator(end(adj.outwards));
}


// rooted graph view {
template<class T> auto
rooted_graph_view(adjacency<AGT>& root) {
  return io_adj_ptr_vector<T>{&root};
}
// TODO
//template<class T> auto
//rooted_graph_view(const adjacency<AGT>& root) {
//  return io_adj_ptr_vector<T>{&root};
//}

template<class T> constexpr auto
first_root(const io_adj_ptr_vector<T>& root_rng) {
  return std_e::make_dereferencing_iterator(begin(root_rng));
}
template<class T> constexpr auto
first_root(io_adj_ptr_vector<T>& root_rng) {
  return std_e::make_dereferencing_iterator(begin(root_rng));
}
template<class T> constexpr auto
last_root(const io_adj_ptr_vector<T>& root_rng) {
  return std_e::make_dereferencing_iterator(end(root_rng));
}
template<class T> constexpr auto
last_root(io_adj_ptr_vector<T>& root_rng) {
  return std_e::make_dereferencing_iterator(end(root_rng));
}
// rooted graph view }


// TODO new interface names => report in concept + nested_tree...
template<class T> constexpr auto
out_adjacencies(adjacency<AGT>& adj) {
  return std_e::make_dereferencing_range(adj.outwards);
}
template<class T> constexpr auto
out_adjacencies(const adjacency<AGT>& adj) {
  return std_e::make_dereferencing_range(adj.outwards);
}
template<class T> constexpr auto
in_adjacencies(adjacency<AGT>& adj) {
  return std_e::make_dereferencing_range(adj.inwards);
}
template<class T> constexpr auto
in_adjacencies(const adjacency<AGT>& adj) {
  return std_e::make_dereferencing_range(adj.inwards);
}
// Node_adjacency interface }

template<class T> constexpr auto
equal(const adjacency<AGT>& x, const adjacency<AGT>& y, const adjacency<AGT>* x_start, const adjacency<AGT>* y_start) -> bool {
  return
      x.node == y.node
   && std_e::equal_iterator_shifts(x.inwards,y.inwards,x_start,y_start)
   && std_e::equal_iterator_shifts(x.outwards,y.outwards,x_start,y_start);
}
// io_adjacency }




// algorithms {
template<class T> constexpr auto
is_reflexive_in_adjacency(const adjacency<AGT>& x) -> bool {
  for (auto inward_ptr : x.inwards) {
    auto& outs_of_in = inward_ptr->outwards;
    auto x_pos = std::find(begin(outs_of_in),end(outs_of_in),&x);
    if (x_pos==end(outs_of_in)) return false;
  }
  return true;
}
template<class T> constexpr auto
is_reflexive_out_adjacency(const adjacency<AGT>& x) -> bool {
  for (auto outward_ptr : x.outwards) {
    auto& ins_of_out = outward_ptr->inwards;
    auto x_pos = std::find(begin(ins_of_out),end(ins_of_out),&x);
    if (x_pos==end(ins_of_out)) return false;
  }
  return true;
}
template<class T> constexpr auto
is_bidirectional_adjacency(const adjacency<AGT>& x) -> bool {
  return is_reflexive_in_adjacency(x) && is_reflexive_out_adjacency(x);
}


/// redirect all edges entering "old" to now enter "new_adj"
template<class T> constexpr auto
redirect_entering_adjacencies(adjacency<AGT>* old, adjacency<AGT>* new_adj) -> void {
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
