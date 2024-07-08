#pragma once

#include "std_e/utils/to_string_fwd.hpp"
#include "std_e/utils/functional.hpp"

namespace std_e {

const int INDENT_SIZE = 4;

template<class T>
class Tree {
  public:
    /// Nested tree structure
    /// A nested tree is a recursive structure: it has
    ///   - a `node` attribute,
    ///   - a `children` attribute that is a sequence of `Tree`s.
    ///
    Tree() = default;
    Tree(T node, std::vector<Tree<T>> children = {})
      : node{std::move(node)}
      , children{std::move(children)}
    {}

    T node;
    std::vector<Tree<T>> children;
};

// rooted graph interface {
template<class T> auto first_child(      Tree<T>& x) ->       Tree<T>* { return x.children.data()                    ; }
template<class T> auto first_child(const Tree<T>& x) -> const Tree<T>* { return x.children.data()                    ; }
template<class T> auto last_child (      Tree<T>& x) ->       Tree<T>* { return x.children.data() + x.children.size(); }
template<class T> auto last_child (const Tree<T>& x) -> const Tree<T>* { return x.children.data() + x.children.size(); }

template<class T> auto first_root (      Tree<T>& x) ->       Tree<T>* { return &x  ; }
template<class T> auto first_root (const Tree<T>& x) -> const Tree<T>* { return &x  ; }
template<class T> auto last_root  (      Tree<T>& x) ->       Tree<T>* { return &x+1; }
template<class T> auto last_root  (const Tree<T>& x) -> const Tree<T>* { return &x+1; }

template<class T> auto first_root (      std::vector<Tree<T>>& tc) -> auto { return tc.data()            ; }
template<class T> auto first_root (const std::vector<Tree<T>>& tc) -> auto { return tc.data()            ; }
template<class T> auto  last_root (      std::vector<Tree<T>>& tc) -> auto { return tc.data() + tc.size(); }
template<class T> auto  last_root (const std::vector<Tree<T>>& tc) -> auto { return tc.data() + tc.size(); }
// rooted graph interface }

template<class T> auto node(      Tree<T>& x) ->       T& { return x.node; }
template<class T> auto node(const Tree<T>& x) -> const T& { return x.node; }

template<class T> auto
_to_string_impl(const Tree<T>& x, int indent_sz) -> std::string {
  std::string s(indent_sz, ' ');
  using std::to_string;
  using std_e::to_string;
  s += to_string(x.node) + '\n';
  for (const auto& c: x.children) {
    s += _to_string_impl(c, indent_sz+INDENT_SIZE);
  }
  return s;
}


template<class T> auto
to_string(const Tree<T>& x, auto is_symbolic_root = always_false_predicate) -> std::string {
  if (is_symbolic_root(x.node)) {
    std::string s = "";
    for (const auto& child : x.children) {
      s += _to_string_impl(child, 0);
    }
    return s;
  } else {
    return _to_string_impl(x, 0);
  }
}
// tree interface }

} // std_e
