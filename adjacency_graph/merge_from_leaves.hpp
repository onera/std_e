#pragma once


#include "std_e/algorithm/for_each.hpp"
#include "graph/adjacency_graph/io_graph_rearranging.hpp"
#include "std_e/future/constexpr_quick_sort.hpp"


namespace graph {


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
  return eq(node(x),node(y)) && out_ptrs(x)==out_ptrs(y);
}
template<class T, class Bin_pred_0, class Bin_pred_1> constexpr auto
less_by_node_and_outwards(const T& x, const T& y, Bin_pred_0 eq, Bin_pred_1 less) -> bool {
  return less(node(x),node(y)) || (eq(node(x),node(y)) && (out_ptrs(x)<out_ptrs(y)));
}


template<class T_ref, class Bin_pred> constexpr auto
// requires T_ref is std_e::reference_wrapper<...> TODO
redirect_super_expressions_to_equivalent(T_ref x_p, T_ref y_p, Bin_pred eq) -> bool {
  auto x = get_pointer(*x_p);
  auto x_eq = get_pointer(*y_p);
  if (eq(*x,*x_eq)) {
    redirect_entering_adjacencies(x,x_eq);
    return true;
  } 
  return false;
}

template<class Random_it, class Bin_pred_0, class Bin_pred_1> constexpr auto
sort_redirect_super_expressions_to_equivalent(Random_it first, Random_it last, Bin_pred_0 eq, Bin_pred_1 less) {
  std_e::quick_sort(first,last,less);

  auto f = [eq](auto x, auto y){ return redirect_super_expressions_to_equivalent(x,y,eq); };
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
      return [lvl = this->lvl](const auto& n){ return height(n)==lvl; };
    }
  private:
    int lvl;
};

template<class T, class Bin_pred_0, class Bin_pred_1> auto
merge_from_leaves(io_graph_rearranging_view<T>& g, Bin_pred_0 eq, Bin_pred_1 less) -> void {
  STD_E_ASSERT(g.size()!=0);
  //STD_E_ASSERT(is_bidirectional_graph(g));//TODO

  // adding outwards to comparison checks
  auto eq_ = [eq](const auto& x, const auto& y){ return equivalent_by_node_and_outwards(x,y,eq); };
  auto less_ = [eq,less](const auto& x, const auto& y){ return less_by_node_and_outwards(x,y,eq,less); };

  // 0. sort by level
  std::sort(begin(g),end(g),less_by_level);

  // 1. level by level, rebind nodes
  auto sort_redirect = [eq_,less_](auto f, auto l){ sort_redirect_super_expressions_to_equivalent(f,l,eq_,less_); };
  int first_level = height(g[0]);
  std_e::for_each_partition(
    begin(g),end(g),
    level_comparison_generator(first_level),
    sort_redirect
  );

  // 2. only keep first node of equivalents 
  auto new_last = std::unique(begin(g),end(g),eq_);
  auto new_sz = new_last-begin(g);
  g.resize(new_sz);
}

template<class T, class Bin_pred_0, class Bin_pred_1> auto
merge_from_leaves(io_graph<T>& g, Bin_pred_0 eq, Bin_pred_1 less) -> io_graph<T> {
  if (g.size()==0) return io_graph<T>{};
  auto g_view = make_rearranging_view(g);
  merge_from_leaves(g_view,eq,less);
  return bidirectional_graph_from_outward_edges(g_view);
}


} // graph
