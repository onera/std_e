#pragma once


#include "std_e/algorithm/for_each.hpp"
#include "std_e/graph/adjacency_graph/adjacency_graph.hpp"
#include "std_e/future/contract.hpp"
#include "std_e/graph/adjacency_graph/rearranging_algo.hpp"


namespace std_e {


constexpr auto
equal_by_level = [](const auto& x, const auto& y) -> bool {
  return height(node(x)) == height(node(y));
};
constexpr auto
less_by_level = [](const auto& x, const auto& y) -> bool {
  return height(node(x)) < height(node(y));
};

template<class T, class Bin_pred> constexpr auto
equivalent_by_node_and_outwards(const T& x, const T& y, Bin_pred eq) -> bool {
  return eq(node(x),node(y)) && out_indices(x)==out_indices(y);
}
template<class T, class Bin_pred_0, class Bin_pred_1> constexpr auto
less_by_node_and_outwards(const T& x, const T& y, Bin_pred_0 eq, Bin_pred_1 less) -> bool {
  return less(node(x),node(y)) || (eq(node(x),node(y)) && (out_indices(x)<out_indices(y)));
}

template<class I, class T> constexpr auto
redirect_inward_adjacencies(I old, I new_adj, io_adjacency_graph<T>& g) -> void {
  // Precondition: is_reflexive_in_adjacency(old)
  // Post conditions:
  //  - all edges previously entering "old" now enter "new_adj"
  //  - !is_reflexive_in_adjacency(old):
  //    - old.inwards.size()!=0
  //    - adj_eq.inwards should become empty

  // for each inward in old.inwards,
  //     one element in inward.outwards points to old (by the precondition)
  //         pick this one and make it point to new_adj
  for (auto&& in_adj : g[old].in_adjacencies()) {
    auto&& outs_of_in = in_adj.out_indices();
    auto old_pos = std::find(begin(outs_of_in),end(outs_of_in),old);
    STD_E_ASSERT(old_pos!=end(outs_of_in));
    *old_pos = new_adj;
  }
};

template<class I, class T, class Bin_pred> constexpr auto
redirect_inward_to_equivalent(I* x, I* x_eq, io_adjacency_graph<T>& g, Bin_pred eq) -> bool {
  if (eq(*x,*x_eq)) {
    redirect_inward_adjacencies(*x,*x_eq,g);
    return true;
  }
  return false;
}

template<class I, class T, class Bin_pred_0, class Bin_pred_1> constexpr auto
sort_redirect_inward_to_equivalent(I* first, I* last, io_adjacency_graph<T>& g, Bin_pred_0 eq, Bin_pred_1 less) -> void {
  std::stable_sort(first,last,less);

  auto f = [eq,&g](I* i_ptr, I* j_ptr){ return redirect_inward_to_equivalent(i_ptr,j_ptr,g,eq); };
  std_e::replace_by_first_equivalent(first,last,f);
}


template<class T>
class level_comparison_generator {
  public:
    using index_type = typename io_adjacency_graph<T>::index_type;

    constexpr
    level_comparison_generator(const io_adjacency_graph<T>& g)
      : g(g)
      , lvl(min_level(g))
    {}

    constexpr auto
    operator++() {
      ++lvl;
      return *this;
    }
    constexpr auto
    operator*() const {
      return [lvl = this->lvl,&g = this->g](index_type i){ return height(node(g[i]))==lvl; };
    }
  private:
  // function
    static constexpr auto
    min_level(const io_adjacency_graph<T>& g) -> int {
      auto min_lvl_node = std::min_element(begin(g),end(g),less_by_level);
      return height(node(*min_lvl_node));
    }
  // data members
    const io_adjacency_graph<T>& g;
    int lvl;
};

template<class T, class I, class Bin_pred_0, class Bin_pred_1> auto
merge_from_leaves(io_adjacency_graph<T>& g, std::vector<I>& perm, Bin_pred_0 eq, Bin_pred_1 less) -> void {
  STD_E_ASSERT(g.size()!=0);
  //STD_E_ASSERT(is_bidirectional_graph(g));//TODO

  // 0. sort by level
  auto less_by_level_ = [&g](I i, I j){ return less_by_level(g[i],g[j]); };
  std_e::sort(perm,less_by_level_); // TODO partition sort

  // 1. level by level, rebind nodes
  auto eq_ = [eq,&g](I i, I j){ return equivalent_by_node_and_outwards(g[i],g[j],eq); };
  auto less_ = [eq,less,&g](I i, I j){ return less_by_node_and_outwards(g[i],g[j],eq,less); };
  auto sort_redirect = [eq_,less_,&g](auto f, auto l){ sort_redirect_inward_to_equivalent(f,l,g,eq_,less_); };
  std_e::for_each_partition(
    perm.data(),perm.data()+perm.size(),
    level_comparison_generator(g),
    sort_redirect
  );

  // 2. only keep first node of equivalents
  std_e::unique(perm,eq_);
}

template<class T, class Bin_pred_0, class Bin_pred_1> auto
merge_from_leaves(io_adjacency_graph<T>& g, Bin_pred_0 eq, Bin_pred_1 less) -> io_adjacency_graph<T> {
  if (g.size()==0) return io_adjacency_graph<T>{};

  using index_type = typename io_adjacency_graph<T>::index_type;
  std::vector<index_type> perm(g.size());
  std::iota(begin(perm),end(perm),0);

  merge_from_leaves(g,perm,eq,less);
  return bidirectional_graph_from_outward_edges(g,perm);
}


} // std_e
