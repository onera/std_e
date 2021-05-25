#pragma once


#include "std_e/algorithm/for_each.hpp"
#include "std_e/graph/adjacency_graph/adjacency_graph.hpp"
#include "std_e/future/contract.hpp"


namespace std_e {


constexpr auto
equal_by_level = [](const auto& x, const auto& y) -> bool {
  return height(x)==height(y);
};
constexpr auto
less_by_level = [](const auto& x, const auto& y) -> bool {
  return height(x)<height(y);
};

template<class T, class Bin_pred> constexpr auto
equivalent_by_node_and_outwards(const T& x, const T& y, Bin_pred eq) -> bool {
  return eq(node(x),node(y)) && out_adjacencies(x)==out_adjacencies(y);
}
template<class T, class Bin_pred_0, class Bin_pred_1> constexpr auto
less_by_node_and_outwards(const T& x, const T& y, Bin_pred_0 eq, Bin_pred_1 less) -> bool {
  return less(node(x),node(y)) || (eq(node(x),node(y)) && (out_adjacencies(x)<out_adjacencies(y)));
}

template<class Adj> constexpr auto
redirect_inward_adjacencies(Adj& old, Adj& new_adj) -> void {
  // Precondition: is_reflexive_in_adjacency(old)
  // Post conditions:
  //  - all edges previously entering "old" now enter "new_adj"
  //  - !is_reflexive_in_adjacency(old):
  //    - old.inwards.size()!=0
  //    - adj_eq.inwards should become empty

  // for each inward in old.inwards,
  //     one element in inward.outwards points to old (by the precondition)
  //         pick this one and make it point to new_adj
  for (auto& in_adj : old.in_adjacencies()) {
    auto& outs_of_in = in_adj.out_adjacencies();
    auto first = outs_of_in.data();
    auto last = outs_of_in.data()+outs_of_in.size();
    auto old_pos = std::find(first,last,old);
    STD_E_ASSERT(old_pos!=last);
    *old_pos = new_adj;
  }
};

template<class T, class Bin_pred> constexpr auto
redirect_inward_to_equivalent(T x, T x_eq, Bin_pred eq) -> bool {
  if (eq(*x,*x_eq)) {
    redirect_inward_adjacencies(*x,*x_eq);
    return true;
  }
  return false;
}

template<class Random_it, class Bin_pred_0, class Bin_pred_1> constexpr auto
sort_redirect_inward_to_equivalent(Random_it first, Random_it last, Bin_pred_0 eq, Bin_pred_1 less) {
  std::sort(first,last,less);

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

template<class T, class Bin_pred_0, class Bin_pred_1> auto
merge_from_leaves(io_adjacency_graph<T>& g, Bin_pred_0 eq, Bin_pred_1 less) -> void {
  STD_E_ASSERT(g.size()!=0);
  //STD_E_ASSERT(is_bidirectional_graph(g));//TODO

  // adding outwards to comparison checks
  auto eq_ = [eq](const auto& x, const auto& y){ return equivalent_by_node_and_outwards(x,y,eq); };
  auto less_ = [eq,less](const auto& x, const auto& y){ return less_by_node_and_outwards(x,y,eq,less); };

  // 0. sort by level
  std::sort(begin(g),end(g),less_by_level);

  // 1. level by level, rebind nodes
  auto sort_redirect = [eq_,less_](auto f, auto l){ sort_redirect_inward_to_equivalent(f,l,eq_,less_); };
  int first_level = height(node(g[0]));
  std_e::for_each_partition(
    begin(g),end(g),
    level_comparison_generator(first_level),
    sort_redirect
  );

  // 2. only keep first node of equivalents
  auto new_last = std::unique(begin(g),end(g),eq_);
  //auto new_sz = new_last-begin(g); // TODO !!!
  auto new_sz = std::distance(begin(g),new_last);
  //g.resize(new_sz); // TODO!!!
}

template<class T, class Bin_pred_0, class Bin_pred_1> auto
merge_from_leaves2(io_adjacency_graph<T>& g, Bin_pred_0 eq, Bin_pred_1 less) -> io_adjacency_graph<T> {
  if (g.size()==0) return io_adjacency_graph<T>{};
  merge_from_leaves(g,eq,less);
  make_bidirectional_from_outward_edges(g);
  return g; // TODO del
}


} // std_e
