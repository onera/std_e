#pragma once


#include "std_e/graph/algorithm/algo_adjacencies.hpp"
#include "std_e/debug.hpp"


namespace std_e {


template<class Out, class Graph, class Node_ctor, class F>
class _build_tree_visitor {
  public:
    using adjacency_type = typename std::decay_t<Graph>::adjacency_type;

    _build_tree_visitor(Node_ctor&& node_ctor, F&& pre)
      : _node_ctor(FWD(node_ctor))
      , _pre(FWD(pre))
      , _sub_tree_stack({{}})
    {}

    auto 
    pre(const adjacency_type& node) { // TODO
      _sub_tree_stack.push_back({}); // add a level
      return _pre(node);
    }

    auto
    post(const adjacency_type& from_node) { // TODO
      auto& to_children = _sub_tree_stack.back(); // get children from stack
      auto to_node = _node_ctor(from_node, to_children);

      _sub_tree_stack.pop_back(); // we are done with the children
      _sub_tree_stack.back().push_back(std::move(to_node)); // one level up, we can now deposit the sub_tree that we just built
    }

    auto
    down(auto&&, auto&&) { // TODO
    }
    auto
    up(auto&&, auto&&) { // TODO
    }

    auto 
    retrieve_composition_term() { // supposed to be called at the end
      STD_E_ASSERT(_sub_tree_stack.size() == 1); // there should be only one level left since we finished the search
      return std::move(_sub_tree_stack[0]);
    }

  private:
    std_e::remove_rvalue_reference<Node_ctor> _node_ctor;
    std_e::remove_rvalue_reference<F> _pre;
    std::vector<std::vector<Out>> _sub_tree_stack;
};


constexpr auto enter_all_pre = [](auto&&) -> step {
  return step::into;
};
template<class Out, class Graph, class Node_ctor, class F = const decltype(enter_all_pre)&>
auto depth_first_build_trees(Graph&& g, Node_ctor&& node_ctor, F&& pre = enter_all_pre) {
  auto v = _build_tree_visitor<Out,Graph,Node_ctor,F>(FWD(node_ctor), FWD(pre));
  depth_first_search_adjacencies(g, v);
  return v.retrieve_composition_term();
}

template<class Out, class Graph, class Node_ctor, class F = const decltype(enter_all_pre)&>
auto depth_first_build(Graph&& g, Node_ctor&& node_ctor, F&& pre = enter_all_pre) {
  /**
    Depth first traversal of graph `g` in order to create another graph `g_out`.

  Args:
    g: Graph object that should conform to the depth-first search interface. See :func:`dfs_interface_report` for full documentation.

    node_ctor: tell the algorithm how to construct a node from its children and the current traversed value in the original graph `g`
        More precisely, `node_ctor` is a function with two arguments `from_node` and `to_children` and returning `to_node`
        `from_node` is the current node that the depth-first algorithm is traversing
        `to_children` is a list of the previous sub-graphs of `g_out`
        `to_node` is the new graph node that is to be created.

    pre: tells the algorithm what to do on each node traversed (step.into, step.over, step.out)
        More precisely, `pre` is a function of argument `node` and returning a `graph.algo.step` value.
  **/
  auto ts = depth_first_build_trees<Out>(g, FWD(node_ctor), FWD(pre));
  STD_E_ASSERT(ts.size() == 1); // precondition: len(g.root_iterator()) == 1
  return ts[0];
}


} // std_e
