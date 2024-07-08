#pragma once


#include "std_e/graph/algorithm/algo_adjacencies.hpp"
#include "std_e/graph/algorithm/graph_stack.hpp"


namespace std_e {


/**
*  concept Graph_reduction
*    adj_type
*    reduced_type
*    init(adj_type) -> reduced_type
*    finish(reduced_type& parent, reduced_type& child) -> void
*    accumulate(reduced_type& parent, reduced_type& child) -> void
*/


template<class Graph_reduction>
struct visitor_for_reduction {
  public:
  // type traits
    using graph_reduction_type = std::decay_t<Graph_reduction>;
    using adj_type = typename graph_reduction_type::adj_type;
    using reduced_type = typename graph_reduction_type::reduced_type;

  // ctor
    template<class Graph_reduction_0> constexpr
    visitor_for_reduction(Graph_reduction_0&& graph_red)
      : graph_red(FWD(graph_red))
    {}

  // Graph_node_visitor interface
    template<class adj_type_0> constexpr auto
    // requires std::decay_t<adj_type_0>==adj_type
    pre(adj_type_0&& x) {
      S.push_level(graph_red.init(FWD(x)));
    }
    constexpr auto
    post(adj_type) {
      if (!S.is_at_root_level()) {
        graph_red.finish(S.parent_level(),S.current_level());
      }
    }
    constexpr auto
    up(adj_type,adj_type) -> void {
      if (!S.is_at_root_level()) {
        graph_red.accumulate(S.parent_level(),S.current_level());
        S.pop_level();
      }
    }
    constexpr auto
    down(adj_type,adj_type) -> void {}

  // retrieve state when done
    constexpr auto
    final_value() -> auto& {
      STD_E_ASSERT(S.is_at_root_level());
      return S.current_level();
    }

  private:
    remove_rvalue_reference<Graph_reduction> graph_red;
    graph_stack<reduced_type> S;
};


template<class Graph, class Graph_reduction> FORCE_NO_INLINE constexpr auto
depth_first_reduce(Graph&& g, Graph_reduction&& f) {
  visitor_for_reduction<Graph_reduction&&> vis_red(FWD(f));
  depth_first_scan_adjacencies(g,vis_red);
  return vis_red.final_value();
}


} // std_e 
