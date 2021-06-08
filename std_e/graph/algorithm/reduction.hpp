#pragma once


#include "std_e/graph/algorithm/algo_base.hpp"


namespace std_e {


/**
*  concept Graph_reduction
*    node_type
*    reducted_type
*    init(node_type) -> reducted_type
*    finish(reducted_type& parent, reducted_type& child) -> void
*    accumulate(reducted_type& parent, reducted_type& child) -> void
*/


template<class Graph_reduction>
struct visitor_for_reduction {
  public:
  // type traits
    using graph_reduction_type = std::decay_t<Graph_reduction>;
    using node_type = typename graph_reduction_type::node_type;
    using reducted_type = typename graph_reduction_type::reducted_type;

  // ctor
    template<class Graph_reduction_0> constexpr
    visitor_for_reduction(Graph_reduction_0&& graph_red)
      : graph_red(FWD(graph_red))
    {}

  // Graph_node_visitor interface
    template<class node_type_0> constexpr auto
    // requires std::decay_t<node_type_0>==node_type
    pre(node_type_0&& x) {
      S.push_level(graph_red.init(FWD(x)));
    }
    constexpr auto
    post(node_type) {
      if (!S.is_at_root_level()) {
        graph_red.finish(S.parent_level(),S.current_level());
      }
    }
    constexpr auto
    up(node_type,node_type) -> void {
      if (!S.is_at_root_level()) {
        graph_red.accumulate(S.parent_level(),S.current_level());
        S.pop_level();
      }
    }
    constexpr auto
    down(node_type,node_type) -> void {}

  // retrieve state when done
    constexpr auto
    final_value() -> auto& {
      STD_E_ASSERT(S.is_at_root_level());
      return S.current_level();
    }

  private:
    remove_rvalue_reference<Graph_reduction> graph_red;
    graph_stack<reducted_type> S;
};


template<class Graph, class Graph_reduction> FORCE_NO_INLINE constexpr auto
depth_first_reduce(Graph&& g, Graph_reduction&& f) {
  visitor_for_reduction<Graph_reduction&&> vis_red(FWD(f));
  depth_first_scan(g,vis_red);
  return vis_red.final_value();
}


} // std_e 
