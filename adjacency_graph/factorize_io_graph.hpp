#pragma once


#include "std_e/algorithm/for_each.hpp"
#include "graph/adjacency_graph/io_graph_rearranging.hpp"


namespace graph {


constexpr auto
height(const io_adjacency<operation_eval_node>& x) -> int {
  return node(x).height;
}


template<class T> constexpr auto 
equal_by_level = [](const io_adjacency<T>& x, const io_adjacency<T>& y) -> bool {
  return height(x)==height(y);
};
template<class T> constexpr auto
less_by_level = [](const io_adjacency<T>& x, const io_adjacency<T>& y) -> bool {
  return height(x)<height(y);
};

template<class T, class Bin_pred> constexpr auto
equivalent_by_node_and_outwards(const io_adjacency<T>& x, const io_adjacency<T>& y, Bin_pred eq) -> bool {
  return eq(x,y) && x.outwards==y.outwards;
};
template<class T, , class Bin_pred_0, class Bin_pred_1> constexpr auto
less_by_node_and_outwards(const io_adjacency<T>& x, const io_adjacency<T>& y, Bin_pred_0 eq, Bin_pred_1 less) -> bool {
  return less(x,y) || (eq(x,y) && x.outwards<y.outwards);
};


template<class T_ref, class Bin_pred> constexpr auto
// requires T_ref is std_e::reference_wrapper<...> TODO
redirect_super_expressions_to_equivalent(T_ref x_p, T_ref y_p, Bin_pred eq) {
  auto x = get_pointer(*x_p);
  auto x_eq = get_pointer(*y_p);
  if (equivalent_by_node_and_outwards(*x,*x_eq,eq)) {
    redirect_entering_adjacencies(x,x_eq);
    return true;
  } 
  return false;
};

template<class Random_it, class Bin_pred_0, class Bin_pred_1> constexpr auto
sort_redirect_super_expressions_to_equivalent(Random_it first, Random_it last, Bin_pred_0 eq, Bin_pred_1 less) {
  auto less_cmp = [eq,less](const auto& x, const auto& y){ return less_by_node_and_outwards(x,y,eq,less); };
  std::sort(first,last,less_cmp);

  std_e::for_each_equivalent(first,last,redirect_super_expressions_to_equivalent);
};


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
factorize(io_graph_rearranging_view<T>& g, Bin_pred_0 eq, Bin_pred_1 less) -> void {
  STD_E_ASSERT(g.size()!=0);
  //STD_E_ASSERT(is_bidirectional_graph(g));//TODO

  // 0. sort by level
  std::sort(begin(g),end(g),less_by_level);

  // 1. level by level, rebind nodes
  int first_level = height(g[0]);
  std_e::for_each_partition(
    begin(g),end(g),
    level_comparison_generator(first_level),
    sort_redirect_super_expressions_to_equivalent
  );

  // 2. only keep first node of equivalents 
  auto equiv = [](const auto& x, const auto& y){ equivalent_by_node_and_outwards(x,y,eq); };
  auto new_last = std_e::unique(begin(g),end(g),equiv);
  auto new_sz = new_last-begin(g);
  g.resize(new_sz);
}

template<class T, class Bin_pred_0, class Bin_pred_1> auto
factorize(io_graph& g, Bin_pred_0 eq, Bin_pred_1 less) -> io_graph {
  if (g.size()==0) return io_graph{};
  auto g_view = make_rearranging_view(g);
  factorize(g_view,eq,less);
  return bidirectional_graph_from_outward_edges(g_view);
}


} // graph
