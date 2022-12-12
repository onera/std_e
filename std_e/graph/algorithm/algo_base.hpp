#pragma once


#include <vector>
#include "std_e/iterator/iterator_range.hpp"
#include "std_e/meta/meta.hpp"
#include "std_e/graph/algorithm/step_enum.hpp"


namespace std_e {


// graph_traversal_stack {
template<class T>
class graph_stack {
  public:
  // ctors
    constexpr
    graph_stack() {}

    constexpr
    graph_stack(T root_value)
      : S{std::move(root_value)}
    {}

  // basic query
    constexpr auto
    size() const -> int {
      return S.size();
    }
    constexpr auto
    is_valid() const -> bool {
      return size()>0;
    }
    constexpr auto
    is_at_root_level() const -> bool {
      return size()==1;
    }

  // stack functions
    constexpr auto
    push_level(const T& x) -> void {
      S.push_back(x);
    }
    constexpr auto
    pop_level() -> void {
      S.pop_back();
    }

  // accessors
    constexpr auto
    current_level() -> T& {
      STD_E_ASSERT(is_valid());
      return S[S.size()-1];
    }
    constexpr auto
    current_level() const -> const T& {
      STD_E_ASSERT(is_valid());
      return S[S.size()-1];
    }
    constexpr auto
    parent_level() -> T& {
      STD_E_ASSERT(is_valid() && !is_at_root_level());
      return S[S.size()-2];
    }
    constexpr auto
    parent_level() const -> const T& {
      STD_E_ASSERT(is_valid() && !is_at_root_level());
      return S[S.size()-2];
    }

  private:
    std::vector<T> S;
};

template<class iterator>
// requires iterator is a graph iterator
class graph_traversal_stack {
  public:
    constexpr
    graph_traversal_stack(iterator f, iterator l)
      : S({f,l})
    {}

    constexpr auto
    current_node() -> iterator& {
      return S.current_level().first;
    }
    constexpr auto
    current_node() const -> const iterator& {
      return S.current_level().first;
    }

    constexpr auto
    last_node() -> iterator& {
      return S.current_level().last;
    }
    constexpr auto
    last_node() const -> const iterator& {
      return S.current_level().last;
    }

    constexpr auto
    parent_node() -> iterator& {
      return S.parent_level().first;
    }
    constexpr auto
    parent_node() const -> const iterator& {
      return S.parent_level().first;
    }

    constexpr auto
    push_level(iterator f, iterator l) -> void {
      S.push_level({f,l});
    }
    constexpr auto
    pop_level() -> void {
      S.pop_level();
    }

    constexpr auto
    level_is_done() const -> bool {
      return current_node()==last_node();
    }
    constexpr auto
    is_at_root_level() const -> bool {
      return S.is_at_root_level();
    }
    constexpr auto
    is_done() const -> bool {
      return is_at_root_level() && level_is_done();
    }
  private:
    graph_stack<iterator_range<iterator>> S;
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
      S.push_level(first_child(v),last_child(v));
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
      S.push_level(first_child(v),last_child(v));
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
        S.push_level(first_child(v),last_child(v));
      } else { // prune
        S.push_level(first_child(v),last_child(v));
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
    auto&& v = *S.current_node();
    auto&& parent = *S.parent_node();
    f.post(v);
    f.up(v,parent);
    S.pop_level();
  }
  auto&& v = *S.current_node();
  f.post(v);
}

template<class Graph_iterator_stack, class Graph_adjacency_visitor> auto
// requires Graph_iterator_stack is Array<Iterator_range<Graph>>
depth_first_search_adjacency_stack(Graph_iterator_stack& S, Graph_adjacency_visitor&& f) {
  while (!S.is_done()) {
    if (!S.level_is_done()) {
      auto&& v = *S.current_node();
      auto next_step = f.pre(v);
      switch (next_step) {
        case step::out: { // stop
          auto matching_node = S.current_node();
          unwind(S,f);
          return matching_node; // note: alternative interface: return S, do not unwind (let the caller decide)
        }
        case step::over: { // prune
          S.push_level(first_child(v),last_child(v));
          S.current_node() = S.last_node();
          break;
        }
        case step::into: { // go down
          S.push_level(first_child(v),last_child(v));
          if (!S.level_is_done()) f.down(v,*first_child(v));
          break;
        }
      }
    } else {
      S.pop_level();
      auto&& v = *S.current_node();
      f.post(v);
      auto&& w_it = ++S.current_node();
      if (!S.is_at_root_level()) {
        auto&& parent = *S.parent_node();
        f.up(v,parent);
        if (!S.level_is_done()) f.down(parent,*w_it);
      }
    }
  }
  return S.last_node();
}
// general algorithm }


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
