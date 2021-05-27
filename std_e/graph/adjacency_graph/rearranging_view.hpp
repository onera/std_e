#pragma once


#include "std_e/graph/adjacency_graph/adjacency_graph.hpp"
#include "std_e/algorithm/permutation.hpp"
#include "std_e/meta/meta.hpp"


namespace std_e {


/**
*  class "rearranging_view"
*
*  Directly changing node positions when reordering a graph has to be done in two steps:
*    - change position of the node
*    - update its position for its inward and outward adjacencies
*  This is cumbersome. Plus, doing it for each reordering can be expansive,
*  since we also need to change all ins and outs connectivities
*
*  The idea here is to keep a permutation table and to always access adjacencies through an indirect indexing
*/

template<class adjacency_graph_type, class I>
class rearranging_adjacency {
  public:
  // type traits
    //using index_type = add_other_type_constness<typename adjacency_graph_type::index_type,adjacency_graph_type>;
    using index_type = I;
    using adjacency_type = adjacency_type_of<adjacency_graph_type>;
    using const_adjacency_type = adjacency_type_of<const adjacency_graph_type>;

  // ctors
    constexpr
    rearranging_adjacency() = default;

    constexpr
    rearranging_adjacency(adjacency_graph_type* g, index_type* perm_ptr)
      : g(g)
      , perm_ptr(perm_ptr)
    {}

  // operation for Regular
    constexpr
    rearranging_adjacency(const rearranging_adjacency&) = default;
    constexpr
    rearranging_adjacency& operator=(const rearranging_adjacency&) = default;

    friend constexpr auto
    swap(rearranging_adjacency&& x, rearranging_adjacency&& y) -> void {
      STD_E_ASSERT(x.g==y.g);
      //using std::swap;
      //swap(*x.perm_ptr,*y.perm_ptr);
      // TODO
      auto tmp = *x.perm_ptr;
      *x.perm_ptr = *y.perm_ptr;
      *y.perm_ptr = tmp;
    }

  // get adjacency
    auto
    adj() -> adjacency_type {
      return {g,*perm_ptr};
    }
    auto
    adj() const -> const_adjacency_type {
      return {g,*perm_ptr};
    }
  private:
    adjacency_graph_type* g;
    index_type* perm_ptr;
};
template<class AGT, class I> auto
node(rearranging_adjacency<AGT,I>& x) -> auto& {
  return node(x.adj());
}
template<class AGT, class I> auto
node(const rearranging_adjacency<AGT,I>& x) -> const auto& {
  return node(x.adj());
}
template<class AGT, class I> auto
in_adjacencies(rearranging_adjacency<AGT,I>& x) {
  return in_adjacencies(x.adj());
}
template<class AGT, class I> auto
in_adjacencies(const rearranging_adjacency<AGT,I>& x) {
  return in_adjacencies(x.adj());
}
template<class AGT, class I> auto
out_adjacencies(rearranging_adjacency<AGT,I>& x) {
  return out_adjacencies(x.adj());
}
template<class AGT, class I> auto
out_adjacencies(const rearranging_adjacency<AGT,I>& x) {
  return out_adjacencies(x.adj());
}
template<class AGT, class I> auto
to_string(const rearranging_adjacency<AGT,I>& x) {
  return to_string(x.adj());
}


// TODO generate iterator from template
template<class adjacency_graph_type, class I>
class rearranging_iterator {
  public:
  // type traits
    using index_type = I;
    using adjacency_type = rearranging_adjacency<adjacency_graph_type,index_type>;

    /// std::iterator type traits
    using value_type = adjacency_type;
    using reference = adjacency_type;
    using difference_type = index_type;
    using iterator_category = std::random_access_iterator_tag;

  // ctors
    constexpr
    rearranging_iterator() = default;

    constexpr
    rearranging_iterator(adjacency_graph_type* g, index_type* perm_ptr)
      : g(g)
      , perm_ptr(perm_ptr)
    {}

  // iterator interface
    constexpr auto
    operator++() -> rearranging_iterator& {
      ++perm_ptr;
      return *this;
    }
    constexpr auto
    operator++(int) -> rearranging_iterator {
      rearranging_iterator it(*this);
      ++(*this);
      return it;
    }

    constexpr auto
    operator--() -> rearranging_iterator& {
      --perm_ptr;
      return *this;
    }
    constexpr auto
    operator--(int) -> rearranging_iterator {
      rearranging_iterator it(*this);
      --(*this);
      return it;
    }

    constexpr auto
    operator+=(index_type i) -> rearranging_iterator& {
      perm_ptr += i;
      return *this;
    }
    constexpr auto
    operator-=(index_type i) -> rearranging_iterator& {
      perm_ptr -= i;
      return *this;
    }
    constexpr auto
    operator+(index_type i) const -> rearranging_iterator {
      return {g,perm_ptr+i};
    }
    constexpr auto
    operator-(index_type i) const -> rearranging_iterator {
      return {g,perm_ptr-i};
    }
    constexpr auto
    operator-(const rearranging_iterator& x) const -> index_type {
      STD_E_ASSERT(g==x.g);
      return perm_ptr - x.perm_ptr;
    }

    constexpr auto
    operator*() const -> adjacency_type {
      return {g,perm_ptr};
      //return *perm_ptr;
    }

    constexpr auto
    operator==(const rearranging_iterator& x) const {
      STD_E_ASSERT(g==x.g);
      return perm_ptr == x.perm_ptr;
    }
    constexpr auto
    operator<=>(const rearranging_iterator& x) const {
      STD_E_ASSERT(g==x.g);
      return perm_ptr <=> x.perm_ptr;
    }
  private:
    adjacency_graph_type* g;
    index_type* perm_ptr;
};

template<class AGT>
class rearranging_view {
  public:
    using index_type = typename AGT::index_type;
    using adjacency_type = rearranging_adjacency<AGT,index_type>;
    using const_adjacency_type = rearranging_adjacency<AGT,const index_type>;
    using iterator_type = rearranging_iterator<AGT,index_type>;
    using const_iterator_type = rearranging_iterator<AGT,const index_type>;

    constexpr
    rearranging_view(AGT& g)
      : g(&g)
      , perm(g.size())
    {
      std::iota(perm.begin(),perm.end(),0);
    }

    constexpr auto
    size() const -> index_type {
      return perm.size();
    }
    constexpr auto
    resize(index_type sz) -> void {
      STD_E_ASSERT(sz <= size());
      perm.resize(sz);
    }

    constexpr auto
    begin() -> iterator_type {
      return {g,perm.data()};
    }
    constexpr auto
    begin() const -> const_iterator_type {
      return {g,perm.data()};
    }
    constexpr auto
    end() -> iterator_type {
      return {g,perm.data()+perm.size()};
    }
    constexpr auto
    end() const -> const_iterator_type {
      return {g,perm.data()+perm.size()};
    }
    constexpr auto
    operator[](index_type i) -> adjacency_type {
      return {g,perm.data()+i};
    }
    constexpr auto
    operator[](index_type i) const -> const_adjacency_type {
      return {g,perm.data()+i};
    }

    auto
    old_to_new_node_indices() const {
      return inverse_partial_permutation(perm,initial_sz(),-1);
    }
  private:
    auto
    initial_sz() const -> index_type {
      return g->size();
    }
    AGT* g;
    std::vector<index_type> perm;
};

template<class AGT> constexpr auto
begin(rearranging_view<AGT>& x) {
  return x.begin();
}
template<class AGT> constexpr auto
begin(const rearranging_view<AGT>& x) {
  return x.begin();
}
template<class AGT> constexpr auto
end(rearranging_view<AGT>& x) {
  return x.end();
}
template<class AGT> constexpr auto
end(const rearranging_view<AGT>& x) {
  return x.end();
}


template<class rearranging_view_type, class index_type, class AGT> constexpr auto
propagate_outward_edges(const rearranging_view_type& x, const std::vector<index_type>& old_to_new_node_indices, AGT& g) {
  for (index_type i=0; i<x.size(); ++i) {
    for (auto&& old_adj_idx : x[i].adj().out_indices()) {
      g[i].out_indices().push_back( old_to_new_node_indices[old_adj_idx] );
    }
  }
}

template<class AGT> constexpr auto
bidirectional_graph_from_outward_edges(const rearranging_view<AGT>& x) -> AGT {
  int sz = x.size();

  using res_type = std::remove_const_t<AGT>;
  res_type res(sz);
  for (int i=0; i<sz; ++i) {
    res[i].node() = x[i].adj().node();
  }

  propagate_outward_edges(x,x.old_to_new_node_indices(),res);
  make_bidirectional_from_outward_edges(res);
  return res;
}


} // std_e


template<class AGT, class I>
struct std::iterator_traits<std_e::rearranging_iterator<AGT,I>> {
  using type = std_e::rearranging_iterator<AGT,I>;
  using value_type = typename type::value_type;
  using reference = typename type::reference;
  using difference_type = typename type::difference_type;
  using iterator_category = typename type::iterator_category;
};
