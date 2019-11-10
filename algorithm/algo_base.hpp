#pragma once


#include "graph/constexpr_max_sizes.hpp"
#include "std_e/future/constexpr_vector.hpp"
#include "std_e/utils/meta.hpp"


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
    std_e::constexpr_vector<T,Max_graph_depth> S;
};

template<class range_type>
class graph_traversal_stack : public graph_stack<range_type> {
  public:
    using base = graph_stack<range_type>;
    using graph_iterator_type = typename range_type::iterator;

    using base::base;
    using base::current_level;
    using base::parent_level;
    using base::is_at_root_level;

    constexpr auto
    current_node() -> graph_iterator_type& {
      return current_level().begin();
    }
    constexpr auto
    current_node() const -> graph_iterator_type {
      return current_level().begin();
    }
    constexpr auto
    last_node() -> graph_iterator_type& {
      return current_level().end();
    }
    constexpr auto
    last_node() const -> graph_iterator_type {
      return current_level().end();
    }
    constexpr auto
    parent_node() -> graph_iterator_type& {
      return parent_level().begin();
    }
    constexpr auto
    parent_node() const -> graph_iterator_type {
      return parent_level().begin();
    }

    constexpr auto
    level_is_done() const -> bool {
      return current_node()==last_node();
    }
    constexpr auto
    is_done() const -> bool {
      return is_at_root_level() && level_is_done();
    }
};

template<class range_type> constexpr auto
make_graph_traversal_stack(range_type root_range) -> graph_traversal_stack<range_type> {
  return {std::move(root_range)};
}
// graph_traversal_stack }




// base algorithms {
template<class Graph_iterator_stack, class F> constexpr auto
// requires Graph_iterator_stack is Array<Iterator_range<Graph>>
preorder_depth_first_find_adjacency_stack(Graph_iterator_stack& S, F&& f) -> void {
  do {
    if (!S.level_is_done()) {
      auto v = S.current_node();
      if (!f(*v)) return;// TODO opposite (like find_if)
      S.push_level(children(*v));
    } else {
      S.pop_level();
      ++S.current_node();
    }
  } while (!S.is_done());
}

template<class Graph_iterator_stack, class Graph_adjacency_visitor> constexpr auto
// requires Graph_iterator_stack is Array<Iterator_range<Graph>>
prepostorder_depth_first_scan_adjacency_stack(Graph_iterator_stack& S, Graph_adjacency_visitor&& f) -> void {
  do {
    if (!S.level_is_done()) {
      auto v = S.current_node();
      f.pre(*v);
      S.push_level(children(*v));
    } else {
      S.pop_level();
      f.post(*S.current_node());
      ++S.current_node();
    }
  } while (!S.is_done());
}

template<class Graph_iterator_stack, class Graph_adjacency_visitor> constexpr auto
// requires Graph_iterator_stack is Array<Iterator_range<Graph>>
depth_first_scan_adjacency_stack(Graph_iterator_stack& S, Graph_adjacency_visitor&& f) -> void {
  do {
    if (!S.level_is_done()) {
      auto v = S.current_node();
      f.pre(*v);
      S.push_level(children(*v));
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
  } while (!S.is_done());
}

template<class Graph_iterator_stack, class Graph_adjacency_visitor> constexpr auto
// requires Graph_iterator_stack is Array<Iterator_range<Graph>>
prepostorder_depth_first_prune_adjacency_stack(Graph_iterator_stack& S, Graph_adjacency_visitor&& f) -> void {
  do {
    if (!S.level_is_done()) {
      auto v = S.current_node();
      if (f.pre(*v)) { // go down
        S.push_level(children(*v));
      } else { // prune
        S.push_level(children(*v));
        S.current_node() = S.last_node();
      }
    } else {
      S.pop_level();
      f.post(*S.current_node());
      ++S.current_node();
    }
  } while (!S.is_done());
}
template<class Graph_iterator_stack, class Graph_adjacency_visitor> auto
// requires Graph_iterator_stack is Array<Iterator_range<Graph>>
depth_first_prune_adjacency_stack(Graph_iterator_stack& S, Graph_adjacency_visitor&& f) -> void {
  do {
    if (!S.level_is_done()) {
      auto v = S.current_node();
      if (f.pre(*v)) { // go down
        S.push_level(children(*v));
        if (!S.level_is_done()) f.down(*v,child(*v,0));
      } else { // prune
        S.push_level(children(*v));
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
  } while (!S.is_done());
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
