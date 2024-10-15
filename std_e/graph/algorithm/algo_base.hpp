#pragma once


#include <vector>
#include "std_e/meta/meta.hpp"
#include "std_e/graph/algorithm/step_enum.hpp"
#include "std_e/future/span.hpp"


namespace std_e {


template<class iterator>
// requires iterator is a graph iterator
class graph_traversal_stack {
  public:
    constexpr
    graph_traversal_stack(iterator f, iterator l)
      : S_first({f})
      , S_last({l})
    {}

    constexpr auto
    current_node() -> iterator& {
      STD_E_ASSERT(is_valid());
      return S_first[size()-1];
    }
    constexpr auto
    current_node() const -> const iterator& {
      STD_E_ASSERT(is_valid());
      return S_first[size()-1];
    }

    constexpr auto
    last_node() -> iterator& {
      STD_E_ASSERT(is_valid());
      return S_last[size()-1];
    }
    constexpr auto
    last_node() const -> const iterator& {
      STD_E_ASSERT(is_valid());
      return S_last[size()-1];
    }

    constexpr auto
    parent_node() -> iterator& {
      STD_E_ASSERT(is_valid() && !is_at_root_level());
      return S_first[size()-2];
    }
    constexpr auto
    parent_node() const -> const iterator& {
      STD_E_ASSERT(is_valid() && !is_at_root_level());
      return S_first[size()-2];
    }

    constexpr auto
    nodes() const {
      return std_e::span<iterator>(S_first.begin(), S_first.end());
    }
    constexpr auto
    nodes() {
      return std_e::span<iterator>(S_first.begin(), S_first.end());
    }
    constexpr auto
    parent_stack() const {
      return std_e::span<iterator>(S_first.begin(), S_first.end()-1);
    }
    constexpr auto
    parent_stack() {
      return std_e::span<iterator>(S_first.begin(), S_first.end()-1);
    }

    constexpr auto
    push_level() -> void {
      auto&& v = *current_node();
      S_first.push_back(first_child(v));
      S_last .push_back(last_child(v));
    }
    constexpr auto
    push_done_level() -> void {
      auto&& v = *current_node();
      S_first.push_back(last_child(v));
      S_last .push_back(last_child(v));
    }
    constexpr auto
    pop_level() -> void {
      S_first.pop_back();
      S_last .pop_back();
    }

    constexpr auto
    level_is_done() const -> bool {
      return current_node()==last_node();
    }
    constexpr auto
    is_at_root_level() const -> bool {
      return size()==1;
    }
    constexpr auto
    is_done() const -> bool {
      return is_at_root_level() && level_is_done();
    }
  private:
  // member functions
    constexpr auto
    size() const -> int {
      STD_E_ASSERT(S_first.size() == S_last.size());
      return S_first.size();
    }
    constexpr auto
    is_valid() const -> bool {
      return size()>0;
    }
  // data members
    std::vector<iterator> S_first;
    std::vector<iterator> S_last;
};

template<class iterator_type>
graph_traversal_stack(iterator_type f, iterator_type l) -> graph_traversal_stack<iterator_type>;
// graph_traversal_stack }




// simple preorder find algorithm {
template<class Graph_iterator_stack, class F> constexpr auto
// requires Graph_iterator_stack is Array<Iterator_range<Graph>>
preorder_depth_first_find_adjacency_stack(Graph_iterator_stack& S, F&& f) {
  while (!S.is_done()) {
    if (!S.level_is_done()) {
      auto&& v = *S.current_node();
      if (f(v)) return S.current_node();
      S.push_level();
    } else {
      S.pop_level();
      ++S.current_node();
    }
  }
  return S.last_node();
}
// simple preorder find algorithm }


// pre-post algorithms { // TODO factor
template<class Graph_iterator_stack, class Graph_adjacency_visitor> constexpr auto
// requires Graph_iterator_stack is Array<Iterator_range<Graph>>
prepostorder_depth_first_scan_adjacency_stack(Graph_iterator_stack& S, Graph_adjacency_visitor&& f) -> void {
  while (!S.is_done()) {
    if (!S.level_is_done()) {
      auto&& v = *S.current_node();
      f.pre(v);
      S.push_level();
    } else {
      S.pop_level();
      auto&& v = *S.current_node();
      f.post(v);
      ++S.current_node();
    }
  }
}

template<class Graph_iterator_stack, class Graph_adjacency_visitor> constexpr auto
// requires Graph_iterator_stack is Array<Iterator_range<Graph>>
prepostorder_depth_first_prune_adjacency_stack(Graph_iterator_stack& S, Graph_adjacency_visitor&& f) -> void {
  while (!S.is_done()) {
    if (!S.level_is_done()) {
      auto&& v = *S.current_node();
      if (!f.pre(v)) { // go down
        S.push_level();
      } else { // prune
        S.push_level();
        S.current_node() = S.last_node();
      }
    } else {
      S.pop_level();
      auto&& v = *S.current_node();
      f.post(v);
      ++S.current_node();
    }
  }
}
// pre-post algorithms }


// general algorithm {
template<class Graph_iterator_stack, class Graph_adjacency_visitor> constexpr auto
// requires Graph_iterator_stack is Array<Iterator_range<Graph>>
unwind(Graph_iterator_stack& S, Graph_adjacency_visitor&& f) -> void {
  while (!S.is_at_root_level()) {
    f.post(S.nodes());
    f.up(S.nodes());
    S.pop_level();
  }
  f.post(S.nodes());
}

template<class Graph_iterator_stack, class Graph_adjacency_visitor> auto
// requires Graph_iterator_stack is Array<Iterator_range<Graph>>
_depth_first_search_adjacency_stack(Graph_iterator_stack& S, Graph_adjacency_visitor&& f) {
  while (!S.is_done()) {
    if (!S.level_is_done()) {
      auto next_step = f.pre(S.nodes());
      switch (next_step) {
        case step::out: { // stop
          auto matching_node = S.current_node();
          unwind(S,f);
          return matching_node; // note: alternative interface: return S, do not unwind (let the caller decide)
        }
        case step::over: { // prune
          S.push_done_level();
          break;
        }
        case step::into: { // go down
          S.push_level();
          if (!S.level_is_done()) f.down(S.nodes());
          break;
        }
      }
    } else {
      S.pop_level();
      f.post(S.nodes());
      if (!S.is_at_root_level()) {
        f.up(S.nodes());
        ++S.current_node();
        if (!S.level_is_done()) f.down(S.nodes());
      } else {
        ++S.current_node();
      }
    }
  }
  return S.last_node();
}
// general algorithm }

namespace depth {
  struct All   {}; constexpr All    all   ;
  struct Node  {}; constexpr Node   node  ;
  struct Parent{}; constexpr Parent parent;

  template<class Vis>
  class node_visitor_adaptor {
    public:
      constexpr
      node_visitor_adaptor(auto&& f)
        : f(FWD(f))
      {}

      constexpr auto
      pre(auto&& ancestors) -> step {
        return f.pre(*ancestors.back());
      }
      constexpr auto
      down(auto&& ancestors) -> void {
        auto n = ancestors.size();
        f.down(*ancestors[n-2],*ancestors[n-1]);
      }
      constexpr auto
      up(auto&& ancestors) -> void {
        auto n = ancestors.size();
        f.up  (*ancestors[n-1],*ancestors[n-2]);
      }
      constexpr auto
      post(auto&& ancestors) -> void {
        return f.post(*ancestors.back());
      }
    private:
      remove_rvalue_reference<Vis> f;
  };
}

// primary template: declared, but never defined (but tells that the default is depth::node)
template<class Graph_iterator_stack, class Graph_adjacency_visitor, class Depth = depth::Node> auto
// requires Graph_iterator_stack is Array<Iterator_range<Graph>>
depth_first_search_adjacency_stack(Graph_iterator_stack& S, Graph_adjacency_visitor&& f, Depth = depth::node) {
  if constexpr (std::is_same_v<Depth, depth::All>) {
  return _depth_first_search_adjacency_stack(S, Graph_adjacency_visitor(FWD(f)));
  } else if constexpr (std::is_same_v<Depth, depth::Node>) {
    return _depth_first_search_adjacency_stack(S, depth::node_visitor_adaptor<Graph_adjacency_visitor>(FWD(f)));
  } else {
    throw std_e::msg_exception("depth_first_search_adjacency_stack: unknown depth type");
  }
}

// adaptation of general algorithm to find, prune and scan {

/// The general algorithm asks if it should step out/over/into
/// So the visitor's `pre` function must return a value of `step` type
/// Here, we handle simpler `pre` functions:
/// - find: return true to step out, return false to continue
/// - prune: return true to step over, return false to continue
/// - scan : always continue
template<class F, auto convert_to_step>
class depth_first_visitor_adaptor {
  public:
    constexpr
    depth_first_visitor_adaptor(auto&& f)
      : f(FWD(f))
    {}

    constexpr auto
    pre(auto&& na) -> step {
      return convert_to_step(f,na);
    }
    constexpr auto
    down(auto&& na_above, auto&& na_below) -> void {
      f.down(na_above,na_below);
    }
    constexpr auto
    up(auto&& na_below, auto&& na_above) -> void {
      f.up(na_below,na_above);
    }
    constexpr auto
    post(auto&& na) -> void {
      f.post(na);
    }
  private:
    remove_rvalue_reference<F> f;
};

template<class Graph_iterator_stack, class F> constexpr auto
// requires Graph_iterator_stack is Array<Iterator_range<Graph>>
depth_first_find_adjacency_stack(Graph_iterator_stack& S, F&& f) {
  constexpr auto convert_to_step = [](auto&& f, auto&& na){ return f.pre(na) ? step::out : step::into; };
  depth_first_visitor_adaptor<F,convert_to_step> vis(FWD(f));
  return depth_first_search_adjacency_stack(S,vis);
}
template<class Graph_iterator_stack, class F> auto
// requires Graph_iterator_stack is Array<Iterator_range<Graph>>
depth_first_prune_adjacency_stack(Graph_iterator_stack& S, F&& f) -> void {
  constexpr auto convert_to_step = [](auto&& f, auto&& na){ return f.pre(na) ? step::over: step::into; };
  depth_first_visitor_adaptor<F,convert_to_step> vis(FWD(f));
  depth_first_search_adjacency_stack(S,vis);
}
template<class Graph_iterator_stack, class F> auto
// requires Graph_iterator_stack is Array<Iterator_range<Graph>>
depth_first_scan_adjacency_stack(Graph_iterator_stack& S, F && f) -> void {
  constexpr auto convert_to_step = [](auto&& f, auto&& na){ f.pre(na); return step::into; };
  depth_first_visitor_adaptor<F,convert_to_step> vis(FWD(f));
  depth_first_search_adjacency_stack(S,vis);
}
// adaptation of general algorithm to find, prune and scan }


// only pre or post scans {
template<class F>
class preorder_visitor_adaptor {
  public:
    template<class F_0> constexpr
    preorder_visitor_adaptor(F_0&& f)
      : f(FWD(f))
    {}

    template<class Node_adjacency> constexpr auto
    pre(Node_adjacency&& na) {
      return f(na);
    }

    template<class Node_adjacency> constexpr auto
    down(Node_adjacency&&, Node_adjacency&&) {}
    template<class Node_adjacency> constexpr auto
    up(Node_adjacency&&, Node_adjacency&&) {}
    template<class Node_adjacency> constexpr auto
    post(Node_adjacency&&) {}
  private:
    remove_rvalue_reference<F> f;
};

template<class F>
class postorder_visitor_adaptor {
  public:
    template<class F_0> constexpr
    postorder_visitor_adaptor(F_0&& f)
      : f(FWD(f))
    {}

    template<class Node_adjacency> constexpr auto
    pre(Node_adjacency&&) {}
    template<class Node_adjacency> constexpr auto
    down(Node_adjacency&&, Node_adjacency&&) {}
    template<class Node_adjacency> constexpr auto
    up(Node_adjacency&&, Node_adjacency&&) {}

    template<class Node_adjacency> constexpr auto
    post(Node_adjacency&& na) {
      return f(na);
    }
  private:
    remove_rvalue_reference<F> f;
};

template<class Graph_iterator_stack, class F> constexpr auto
preorder_depth_first_scan_adjacency_stack(Graph_iterator_stack& S, F&& f) -> void {
  preorder_visitor_adaptor<F> pre_vis(FWD(f));
  prepostorder_depth_first_scan_adjacency_stack(S,pre_vis);
}
template<class Graph_iterator_stack, class F> constexpr auto
postorder_depth_first_scan_adjacency_stack(Graph_iterator_stack& S, F&& f) -> void {
  postorder_visitor_adaptor<F> post_vis(FWD(f));
  prepostorder_depth_first_scan_adjacency_stack(S,post_vis);
}

template<class Graph_iterator_stack, class F> constexpr auto
preorder_depth_first_prune_adjacency_stack(Graph_iterator_stack& S, F&& f) -> void {
  preorder_visitor_adaptor<F> pre_vis(FWD(f));
  prepostorder_depth_first_prune_adjacency_stack(S,pre_vis);
}
// only pre or post scans }


} // std_e
