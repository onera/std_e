#pragma once


#include <vector>
#include "std_e/utils/meta.hpp"
#include "std_e/iterator/iterator_range.hpp"


namespace graph {


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
    is_at_root_level() const -> bool {
      return S.size()==1;
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
      return S[S.size()-1];
    }
    constexpr auto
    current_level() const -> const T& {
      return S[S.size()-1];
    }
    constexpr auto
    parent_level() -> T& {
      return S[S.size()-2];
    }
    constexpr auto
    parent_level() const -> const T& {
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
    push_children(iterator f, iterator l) -> void {
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
    is_done() const -> bool {
      return S.is_at_root_level() && level_is_done();
    }
  private:
    graph_stack<std_e::iterator_range<iterator>> S;
};

template<class iterator_type> constexpr auto
make_graph_traversal_stack(iterator_type f, iterator_type l) {
  return graph_traversal_stack<iterator_type>(f,l);
}
// graph_traversal_stack }




// base algorithms {
template<class Graph_iterator_stack, class F> constexpr auto
// requires Graph_iterator_stack is Array<Iterator_range<Graph>>
preorder_depth_first_find_adjacency_stack(Graph_iterator_stack& S, F&& f) -> void {
  while (!S.is_done()) {
    if (!S.level_is_done()) {
      auto v = S.current_node();
      if (!f(*v)) return; // TODO opposite (like find_if)
      S.push_children(first_child(*v),last_child(*v));
    } else {
      S.pop_level();
      ++S.current_node();
    }
  }
}

template<class Graph_iterator_stack, class Graph_adjacency_visitor> constexpr auto
// requires Graph_iterator_stack is Array<Iterator_range<Graph>>
prepostorder_depth_first_scan_adjacency_stack(Graph_iterator_stack& S, Graph_adjacency_visitor&& f) -> void {
  while (!S.is_done()) {
    if (!S.level_is_done()) {
      auto v = S.current_node();
      f.pre(*v);
      S.push_children(first_child(*v),last_child(*v));
    } else {
      S.pop_level();
      f.post(*S.current_node());
      ++S.current_node();
    }
  }
}

template<class Graph_iterator_stack, class Graph_adjacency_visitor> constexpr auto
// requires Graph_iterator_stack is Array<Iterator_range<Graph>>
depth_first_scan_adjacency_stack(Graph_iterator_stack& S, Graph_adjacency_visitor&& f) -> void {
  while (!S.is_done()) {
    if (!S.level_is_done()) {
      auto v = S.current_node();
      f.pre(*v);
      S.push_children(first_child(*v),last_child(*v));
      if (!S.level_is_done()) f.down(*v,child(*v,0));
    } else {
      S.pop_level();
      auto v = S.current_node();
      f.post(*v);
      auto w = ++S.current_node();
      if (!S.is_done()) {
        auto parent = S.parent_node();
        f.up(*v,*parent);
        if (!S.level_is_done()) f.down(*parent,*w);
      }
    }
  }
}

template<class Graph_iterator_stack, class Graph_adjacency_visitor> constexpr auto
// requires Graph_iterator_stack is Array<Iterator_range<Graph>>
prepostorder_depth_first_prune_adjacency_stack(Graph_iterator_stack& S, Graph_adjacency_visitor&& f) -> void {
  while (!S.is_done()) {
    if (!S.level_is_done()) {
      auto v = S.current_node();
      if (f.pre(*v)) { // go down
        S.push_children(first_child(*v),last_child(*v));
      } else { // prune
        S.push_children(first_child(*v),last_child(*v));
        S.current_node() = S.last_node();
      }
    } else {
      S.pop_level();
      f.post(*S.current_node());
      ++S.current_node();
    }
  }
}
template<class Graph_iterator_stack, class Graph_adjacency_visitor> auto
// requires Graph_iterator_stack is Array<Iterator_range<Graph>>
depth_first_prune_adjacency_stack(Graph_iterator_stack& S, Graph_adjacency_visitor&& f) -> void {
  while (!S.is_done()) {
    if (!S.level_is_done()) {
      auto v = S.current_node();
      if (f.pre(*v)) { // go down
        S.push_children(first_child(*v),last_child(*v));
        if (!S.level_is_done()) f.down(*v,child(*v,0));
      } else { // prune
        S.push_children(first_child(*v),last_child(*v));
        S.current_node() = S.last_node();
      }
    } else {
      S.pop_level();
      auto v = S.current_node();
      f.post(*v);
      auto w = ++S.current_node();
      if (!S.is_done()) {
        auto parent = S.parent_node();
        f.up(*v,*parent);
        if (!S.level_is_done()) f.down(*parent,*w);
      }
    }
  }
}
// base algorithms }


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
    std_e::remove_rvalue_reference<F> f;
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
    std_e::remove_rvalue_reference<F> f;
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
// only pre or post scans }


} // graph
