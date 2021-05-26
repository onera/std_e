#pragma once


#include "std_e/graph/adjacency_graph/traits/traits.hpp"
#include <algorithm>


namespace std_e {


template<class adjacency_graph_type, adj_orientation ori>
class adjacency_range {
  public:
    using index_type = typename adjacency_graph_type::index_type;
    using adjacency_type = adjacency_type_of<adjacency_graph_type>;
    using const_adjacency_type = typename adjacency_graph_type::const_adjacency_type;
    using adjacency_iterator_type = adjacency_connection_iterator_type_of<adjacency_graph_type,ori>;
    using const_adjacency_iterator_type = adjacency_connection_iterator<const adjacency_graph_type,ori>;

    constexpr
    adjacency_range() = default;

    constexpr
    adjacency_range(adjacency_graph_type* g, index_type node_idx)
      : g(g)
      , node_idx(node_idx)
    {}

    constexpr auto
    size() const -> index_type {
      if constexpr (ori==adj_orientation::none) {
        return g->degree(node_idx);
      } else if constexpr (ori==adj_orientation::in) {
        return g->in_degree(node_idx);
      } else {
        return g->out_degree(node_idx);
      }
    }

    constexpr auto
    begin() -> adjacency_iterator_type {
      return {g,node_idx,0};
    }
    constexpr auto
    begin() const -> const_adjacency_iterator_type {
      return {g,node_idx,0};
    }
    constexpr auto
    end() -> adjacency_iterator_type {
      return {g,node_idx,size()};
    }
    constexpr auto
    end() const -> const_adjacency_iterator_type {
      return {g,node_idx,size()};
    }

    constexpr auto
    operator[](index_type i) -> adjacency_type {
      return {g,index(i)};
    }
    constexpr auto
    operator[](index_type i) const -> const_adjacency_type {
      return {g,index(i)};
    }

    // TODO <=>
    constexpr auto
    operator==(const adjacency_range& x) const -> bool {
      return std::equal(begin(),end(),x.begin());
    }
    constexpr auto
    operator!=(const adjacency_range& x) const -> bool {
      return !(*this==x);
    }
    constexpr auto
    operator<(const adjacency_range& x) const -> bool {
      return std::lexicographical_compare(begin(),end(),x.begin(),x.end());
    }

    //constexpr auto
    //operator<=>(const adjacency_range& x) const {
    //  return std::lexicographical_compare_three_way(begin(),end(),x.begin(),x.end());
    //}
  private:
  // functions
    constexpr auto
    index(index_type i) const -> index_type {
      if constexpr (ori==adj_orientation::none) {
        return g->index(node_idx,i);
      } else if constexpr (ori==adj_orientation::in) {
        return g->in_index(node_idx,i);
      } else {
        static_assert(ori==adj_orientation::out);
        return g->out_index(node_idx,i);
      }
    }
  // data
    adjacency_graph_type* g;
    index_type node_idx;
};


} // std_e
