#pragma once
// from maia.pytree.graph.utils import list_iterator_type

namespace std_e {

const int INDENT_SIZE = 4;

//// Sometimes, we want the root to just gather all branches together.
//// That is the purpose of SYMBOLIC_ROOT, whose only job is to symbolize a root with no value.
//// Of course, we could just use `None`, or '/', but these could be valid node values.
//// Hence, we create a custom type that is meant to be used for this sole purpose.
//class _SymbolicRoot {
//  public:
//    auto operator<=>(const adjacency_list_mixin& x) const = default;
//};
//const _SymbolicRoot SYMBOLIC_ROOT;




template<class T>
class Tree { //(_TreeToStrMixin,_TreeDepthFirstSearchInterfaceMixing):
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
// tree interface {
template<class T> auto first_child(      Tree<T>& x) ->       Tree<T>* { return x.children.data()                    ; }
template<class T> auto last_child (      Tree<T>& x) ->       Tree<T>* { return x.children.data() + x.children.size(); }
template<class T> auto first_child(const Tree<T>& x) -> const Tree<T>* { return x.children.data()                    ; }
template<class T> auto last_child (const Tree<T>& x) -> const Tree<T>* { return x.children.data() + x.children.size(); }

template<class T> auto first_root (      Tree<T>& x) ->       Tree<T>* { return &x  ; }
template<class T> auto last_root  (      Tree<T>& x) ->       Tree<T>* { return &x+1; }
template<class T> auto first_root (const Tree<T>& x) -> const Tree<T>* { return &x  ; }
template<class T> auto last_root  (const Tree<T>& x) -> const Tree<T>* { return &x+1; }

template<class T> auto node(      Tree<T>& x) ->       T& { return x.node; }
template<class T> auto node(const Tree<T>& x) -> const T& { return x.node; }

template<class T> auto
_to_string_impl(const Tree<T>& x, int indent_sz) {
  std::string s(' ',indent_sz);
  s += str(x.node) + '\n';
  for (const auto& c: x.children) {
    s += _to_string_impl(c, indent_sz+INDENT_SIZE);
  }
  return s;
}

template<class T> auto
to_string(const Tree<T>& x) -> std::string {
  return _to_string_impl(x, 0);
}
// tree interface }

//class _ForwardBackwardTreeList:
//  """
//  List-like class to be used by `ForwardBackwardTree`
//  Use case:
//    Suppose that we have a `t` object of type `ForwardBackwardTree`,
//    and we ask for its children: `cs = t.children`.
//    Now, if we change the first child: `cs[0] = ForwardBackwardTree(my_new_leaf)`
//    Then we would like `cs[0].parent is t`
//
//    For that to append, we need `cs.__setitem__(0, sub_tree)` to set the parent.
//    And for that to work, `cs` can't be a regular Python `list`:
//      we need to override `__setitem__` (and other methods),
//      hence we make `t.children` return a `_ForwardBackwardTreeList`
//      and adapt the methods to set the parent
//  """
//  def __init__(self, l, parent):
//    self._list = l
//    self._parent = parent
//
//  // Same methods as list {
//  //// Note: these could be delegated through inheritance,
//  ////       but on the other hand, inheritance is dangerous,
//  ////       since we might inherit methods that should be replaced
//  ////       without noticing they are present
//  def __len__(self):
//    return len(self._list)
//  def __getitem__(self, i):
//    return self._list[i]
//  def __contains__(self, x):
//    return x in self._list
//  def clear(self):
//    self._list.clear()
//  def pop(self, i):
//    return self._list.pop(i)
//  // Same methods as list }
//
//  // Methods to mutate elements to the list {
//  //// We make the `.parent` of the element be the one of `self`
//  def append(self, x):
//    assert isinstance(x, ForwardBackwardTree)
//    x.parent = self._parent
//    self._list.append(x)
//  def __setitem__(self, i, x):
//    assert isinstance(x, ForwardBackwardTree)
//    x.parent = self._parent
//    self._list[i].parent = None // the previous child is orphaned
//    self._list[i] = x
//  def __iadd__(self, other):
//    if isinstance(other, _ForwardBackwardTreeList):
//      self._list += other._list
//    else:
//      self._list += other
//    for x in other:
//      x.parent = self._parent
//    return self
//  def insert(self, i, x):
//    assert isinstance(x, ForwardBackwardTree)
//    x.parent = self._parent
//    self._list.insert(i, x)
//  // Methods to mutate elements to the list }
//
//
//class ForwardBackwardTree(_TreeToStrMixin,_TreeDepthFirstSearchInterfaceMixing):
//  """
//  `ForwardBackwardTree` means that we can go both directions within the tree:
//    - either get the `children` trees
//    - or the `parent` tree
//  """
//  def __init__(self, node, children = None, parent=None):
//    // Can't write `children = []` directly because of mutable default arguments
//    if children is None: 
//      children = []
//
//    // Precondition: all `children` and `parent` should be `ForwardBackwardTree`s
//
//    for c in children:
//      assert isinstance(c, ForwardBackwardTree)
//    if parent is not None:
//      assert isinstance(parent, ForwardBackwardTree)
//
//    self.node = node
//
//    self._sub_nodes = children
//    for sub_node in self._sub_nodes:
//      sub_node.parent = self
//
//    self.parent = parent
//
//  @property
//  def parent(self):
//    if self._parent_weakref is None:
//      return None
//    else:
//      return self._parent_weakref()
//
//  @parent.setter
//  def parent(self, p):
//    if p is None:
//      self._parent_weakref = None
//    else:
//      self._parent_weakref = weakref.ref(p)
//
//  @property
//  def children(self):
//    return _ForwardBackwardTreeList(self._sub_nodes, self)
//
//  @children.setter
//  def children(self, cs):
//    if isinstance(cs, _ForwardBackwardTreeList):
//      self._sub_nodes = cs._list
//    else:
//      self._sub_nodes = cs
//    for sub_node in self._sub_nodes:
//      sub_node.parent = self

} // std_e
