#pragma once


#include "std_e/algorithm/for_each.hpp"
#include "std_e/graph/adjacency_graph/adjacency_graph.hpp"
#include "std_e/future/contract.hpp"
#include "std_e/graph/adjacency_graph/rearranging_view.hpp"


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

template<class Adj> constexpr auto
redirect_inward_adjacencies(Adj&& old, Adj&& new_adj) -> void {
  // Precondition: is_reflexive_in_adjacency(old)
  // Post conditions:
  //  - all edges previously entering "old" now enter "new_adj"
  //  - !is_reflexive_in_adjacency(old):
  //    - old.inwards.size()!=0
  //    - adj_eq.inwards should become empty

  // for each inward in old.inwards,
  //     one element in inward.outwards points to old (by the precondition)
  //         pick this one and make it point to new_adj
  for (auto&& in_adj : in_adjacencies(old)) {
    auto&& outs_of_in = out_adjacencies(in_adj);
    auto old_pos = std::find(begin(outs_of_in),end(outs_of_in),old.adj());
    STD_E_ASSERT(old_pos!=end(outs_of_in));
    *old_pos = new_adj.adj();
  }
};

template<class T, class Bin_pred> constexpr auto
redirect_inward_to_equivalent(T x, T x_eq, Bin_pred eq) -> bool {
  LOG("bla0:",to_string((*x)));
  LOG("bla1:",to_string((*x_eq)));
  if (eq(*x,*x_eq)) {
    LOG("eq")
    redirect_inward_adjacencies(*x,*x_eq);
    return true;
  }
  LOG("not eq")
  return false;
}

template<class Random_it, class Bin_pred_0, class Bin_pred_1> constexpr auto
sort_redirect_inward_to_equivalent(Random_it first, Random_it last, Bin_pred_0 eq, Bin_pred_1 less) -> void {
  std::stable_sort(first,last,less);

  auto f = [eq](auto x, auto y){ return redirect_inward_to_equivalent(x,y,eq); };
  std_e::for_each_equivalent(first,last,f);
}


class level_comparison_generator {
  public:
    constexpr
    level_comparison_generator(int lvl)
      : lvl(lvl)
    {}

    constexpr auto
    operator++() {
      ++lvl;
      return *this;
    }
    constexpr auto
    operator*() const {
      return [lvl = this->lvl](const auto& n){ return height(node(n))==lvl; };
    }
  private:
    int lvl;
};

template<class AGT, class Bin_pred_0, class Bin_pred_1> auto
merge_from_leaves(rearranging_view<AGT>& g, Bin_pred_0 eq, Bin_pred_1 less) -> void {
  STD_E_ASSERT(g.size()!=0);
  //STD_E_ASSERT(is_bidirectional_graph(g));//TODO

  // adding outwards to comparison checks
  auto eq_ = [eq](const auto& x, const auto& y){ return equivalent_by_node_and_outwards(x,y,eq); };
  auto less_ = [eq,less](const auto& x, const auto& y){ return less_by_node_and_outwards(x,y,eq,less); };

  // 0. sort by level
  LOG("aaaaa00");
  //std::sort(begin(g),end(g),less_by_level);

  // 1. level by level, rebind nodes
  LOG("aaaaa01");
  auto sort_redirect = [eq_,less_](auto f, auto l){ sort_redirect_inward_to_equivalent(f,l,eq_,less_); };
  int first_level = height(node(g[0]));
  LOG("aaaaa02");
  std_e::for_each_partition(
    begin(g),end(g),
    level_comparison_generator(first_level),
    sort_redirect
  );
  LOG("aaaaa03");

  // 2. only keep first node of equivalents
  auto new_last = std::unique(begin(g),end(g),eq_);
  auto new_sz = new_last-begin(g);
  ELOG(new_sz);
  g.resize(new_sz); // TODO!!!
}

template<class T, class Bin_pred_0, class Bin_pred_1> auto
merge_from_leaves2(const io_adjacency_graph<T>& g, Bin_pred_0 eq, Bin_pred_1 less) -> io_adjacency_graph<T> {
  LOG("aaaaa10");
  if (g.size()==0) return io_adjacency_graph<T>{};
  LOG("aaaaa11");
  rearranging_view g_view(g);
  LOG("aaaaa13");
  merge_from_leaves(g_view,eq,less);
  LOG("aaaaa14");
  return bidirectional_graph_from_outward_edges(g_view);
}


} // std_e
