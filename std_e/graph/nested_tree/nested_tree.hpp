#pragma once


#include "std_e/base/macros.hpp"
#include "std_e/future/algorithm.hpp"
#include "std_e/algorithm/algorithm.hpp"
#include "std_e/future/contract.hpp"
#include "std_e/utils/string.hpp"
#include "std_e/iterator/iterator_range.hpp"
#include "std_e/meta/meta.hpp"
#include "std_e/future/span.hpp"


namespace std_e {


/**
  A tree is a particular type of graph where all children of a node are independent from each other
  A nested_tree takes advantage of this property: it stores linearly
    - a parent node,
    - then successively all its children, themselves concatenated the same way, recursively
  More precisely,
    - all nodes are stored in the "nodes" data member in depth-first order
    - in order to know what is the shape of the tree, i.e. when a sub-tree stops,
      the "sizes" data member stores, at position "i", the size of the sub-tree which root is "nodes[i]"
  Properties:
    - contiguous for depth-first search
    - random access for breadth-first search
    - a sub-tree is simply a sub-range
    - since everything must be stored contiguously, insertions are O(n)...
    - ...except for the special case where a new sub-tree is appended to root (could be std::vector push_back-like efficient)
*/
template<class T> using std_alloc_vector = std::vector<T>;
template<class T> using dyn_span = std_e::span<T>;

template<class T, template<class> class Memory_ressource> class nested_tree_base;
template<class T> using nested_tree = nested_tree_base<T,std_alloc_vector>;
template<class T> using nested_tree_view = nested_tree_base<T,dyn_span>;
template<class T> using const_nested_tree_view = nested_tree_base<const T,dyn_span>;

template<class T_ptr, class I_ptr> class child_iterator;

template<class T, template<class> class Memory_ressource>
// requires T is Regular, Memory_ressource with max_size
class nested_tree_base {
  public:
  // traits
    using node_type = T;
    using index_type = std_e::add_other_type_constness<int,T>;

    using nodes_mem_type = Memory_ressource<T>;
    using sizes_mem_type = Memory_ressource<index_type>;

    using iterator = child_iterator<T*,index_type*>;
    using const_iterator = child_iterator<const T*,const index_type*>;

    using range_type = std_e::iterator_range<iterator>;
    using const_range_type = std_e::iterator_range<const_iterator>;

    using node_adj_type = const_nested_tree_view<T>; // TODO DEL

    static constexpr size_t is_nested_tree = true;
    template<class T0, template<class> class M0> friend class nested_tree_base;

  // ctors
    FORCE_NO_INLINE constexpr
    nested_tree_base()
    {}

    FORCE_NO_INLINE explicit constexpr
    nested_tree_base(T x)
      : nodes{std::move(x)}
      , sizes{1}
    {}
    FORCE_NO_INLINE constexpr
    nested_tree_base(const nodes_mem_type& nodes, const sizes_mem_type& sizes)
      : nodes(nodes)
      , sizes(sizes)
    {}

    template<class T0, template<class> class M0> FORCE_NO_INLINE constexpr
    nested_tree_base(const nested_tree_base<T0,M0>& t)
      : nodes(t.nodes.data(),t.nodes.data()+t.nodes.size())
      , sizes(t.sizes.data(),t.sizes.data()+t.sizes.size())
    {}

    template<class T0, template<class> class M0> FORCE_NO_INLINE constexpr auto
    operator=(const nested_tree_base<T0,M0>& t) -> nested_tree_base& {
      nodes = nodes_mem_type(t.nodes.data(),t.nodes.data()+t.nodes.size());
      sizes = sizes_mem_type(t.sizes.data(),t.sizes.data()+t.sizes.size());
      return *this;
    }

  // comparisons
    template<class T0, template<class> class M0, template<class> class M1> friend constexpr auto
    operator==(const nested_tree_base<T0,M0>& x, const nested_tree_base<T0,M1>& y) -> bool;
    template<class T0, template<class> class M0, template<class> class M1> friend constexpr auto
    operator<(const nested_tree_base<T0,M0>& x, const nested_tree_base<T0,M1>& y) -> bool;

  // member functions
    FORCE_INLINE constexpr auto
    size() const -> size_t {
      return sizes[0];
    }
    FORCE_INLINE constexpr auto
    root() -> T& {
      STD_E_ASSERT(size()>0);
      return nodes[0];
    }
    FORCE_INLINE constexpr auto
    root() const -> const T& {
      STD_E_ASSERT(size()>0);
      return nodes[0];
    }
    FORCE_INLINE constexpr auto
    view() -> nested_tree_view<T> {
      return {std_e::make_span(nodes.data()),std_e::make_span(sizes.data())};
    }
    FORCE_INLINE constexpr auto
    const_view() const -> const_nested_tree_view<T> {
      return {std_e::make_span(nodes.data()),std_e::make_span(sizes.data())};
    }

  // iteration / ranges
    constexpr auto
    first_root() -> iterator {
      return {nodes.data(),sizes.data()};
    }
    constexpr auto
    first_root() const -> const_iterator {
      return {nodes.data(),sizes.data()};
    }
    constexpr auto
    last_root() -> iterator {
      return {nodes.data()+size(),sizes.data()+size()}; // TODO why end() does not work ?
    }
    constexpr auto
    last_root() const -> const_iterator {
      return {nodes.data()+size(),sizes.data()+size()}; // TODO why end() does not work ?
    }

    constexpr auto
    first_child() -> iterator {
      return {nodes.data()+1,sizes.data()+1}; // the first sub-tree of a tree
    }                                         // begins just after the first node
    constexpr auto
    first_child() const -> const_iterator {
      return {nodes.data()+1,sizes.data()+1}; // the first sub-tree of a tree
    }                                         // begins just after the first node
    constexpr auto
    last_child() -> iterator {
      return last_root();
    }
    constexpr auto
    last_child() const -> const_iterator {
      return last_root();
    }

    constexpr auto
    children() -> range_type {
      return {first_child(),last_child()};
    }
    constexpr auto
    children() const -> const_range_type {
      return {first_child(),last_child()};
    }

    template<class nested_tree_type, class nested_tree_visitor_type> friend auto
    nested_tree_dfs_with_preordering(nested_tree_type&& t, nested_tree_visitor_type&& f) -> void;

    constexpr auto
    append_child(const nested_tree_base& t) -> auto& {
      //static_assert(Memory_ressource::owns_memory);
      size_t old_size = size();
      size_t new_size = old_size+t.size();
      nodes.resize(new_size);
      sizes.resize(new_size);

      std_e::copy_n(begin(t.nodes),t.size(),begin(nodes)+old_size);
      std_e::copy_n(begin(t.sizes),t.size(),begin(sizes)+old_size);
      sizes[0] += t.size();
      return *this;
    }

  private:
  public: // TODO
    nodes_mem_type nodes;
    sizes_mem_type sizes;
};

template<class T0, template<class> class M0, template<class> class M1> constexpr auto
operator==(const nested_tree_base<T0,M0>& x, const nested_tree_base<T0,M1>& y) -> bool {
  if (x.size() != y.size()) return false;
  auto n = x.size();
  return std_e::equal_n(begin(x.nodes),begin(y.nodes),n) && std_e::equal_n(begin(x.sizes),begin(y.sizes),n);
}
template<class T0, template<class> class M0, template<class> class M1> constexpr auto
operator!=(const nested_tree_base<T0,M0>& x, const nested_tree_base<T0,M1>& y) -> bool {
  return !(x==y);
}
template<class T0, template<class> class M0, template<class> class M1> constexpr auto
operator<(const nested_tree_base<T0,M0>& x, const nested_tree_base<T0,M1>& y) -> bool {
  return (x.sizes<y.sizes) || (x.sizes==y.sizes && x.nodes<y.nodes);
}


// child_iterator {
template<class T_ptr, class I_ptr>
// requires T_ptr is (const)T*
// requires I_ptr is (const)Integer*
class child_iterator {
  public:
    using node_type = std::remove_pointer_t<T_ptr>;
  // std::iterator type traits
    using value_type = nested_tree<node_type>;
    using difference_type = int;
    using iterator_category = std::forward_iterator_tag;

    using iterator = child_iterator<T_ptr,I_ptr>;
    using const_iterator = child_iterator<const T_ptr,const I_ptr>;
    using reference = nested_tree_view<node_type>;
    using const_reference = const_nested_tree_view<node_type>;
  // ctors
    constexpr
    child_iterator()
      : node_ptr{nullptr}
      , size_ptr{nullptr}
    {}
    constexpr
    child_iterator(T_ptr node_ptr, I_ptr size_ptr)
      : node_ptr{node_ptr}
      , size_ptr{size_ptr}
    {}

    template<class T_ptr0, class I_ptr0> friend class child_iterator;
    template<
      class T_ptr0, class I_ptr0 // , TODO enable_if<false> --> why?
      //std::enable_if_t<
      // (   std::is_same_v<const T_ptr0,T_ptr>
      //   && std::is_same_v<const I_ptr0,I_ptr> )
      // ,int
      //> =0
    > constexpr
    child_iterator(const child_iterator<T_ptr0,I_ptr0>& nc_it)
      : node_ptr{nc_it.node_ptr}
      , size_ptr{nc_it.size_ptr}
    {}

    constexpr auto
    size() const -> size_t {
      return *size_ptr;
    }

    constexpr auto
    operator*() const -> reference {
      auto n = size();
      return {std_e::make_span(node_ptr,n),std_e::make_span(size_ptr,n)};
    }
    constexpr auto
    operator->() const {
      return std_e::arrow_proxy<reference>{**this};
    }

    constexpr auto
    operator++() -> child_iterator& {
      node_ptr += size();
      size_ptr += size();
      return *this;
    }

  // comparison
    template<class T_ptr0, class I_ptr0, class T_ptr1, class I_ptr1> friend constexpr auto
    operator==(const child_iterator<T_ptr0,I_ptr0>& x, const child_iterator<T_ptr1,I_ptr1>& y);
  private:
    T_ptr node_ptr;
    I_ptr size_ptr;
};

template<class T_ptr0, class I_ptr0, class T_ptr1, class I_ptr1> constexpr auto
// requires T_ptr0 is T* or const T*
// requires T_ptr1 is T* or const T*
operator==(const child_iterator<T_ptr0,I_ptr0>& x, const child_iterator<T_ptr1,I_ptr1>& y) {
  return x.node_ptr==y.node_ptr && x.size_ptr==y.size_ptr;
}
template<class T_ptr0, class I_ptr0, class T_ptr1, class I_ptr1> constexpr auto
// requires T_ptr0 is T* or const T*
// requires T_ptr1 is T* or const T*
operator!=(const child_iterator<T_ptr0,I_ptr0>& x, const child_iterator<T_ptr1,I_ptr1>& y) {
  return !(x==y);
}
} // std_e

namespace std {

template<class T_ptr, class I_ptr>
struct iterator_traits<std_e::child_iterator<T_ptr,I_ptr>> {
  using type = std_e::child_iterator<T_ptr,I_ptr>;
  using difference_type = int;
  using iterator_category = std::forward_iterator_tag;

  using value_type = typename type::value_type;
  using iterator = typename type::iterator;
  using const_iterator = typename type::const_iterator;
  using reference = typename type::reference;
  using const_reference = typename type::const_reference;
};

} // std
// child_iterator }


namespace std_e {


// public tree interface {
template<class T, class... tree_types> constexpr auto
// requires T regular, tree_types are all tree<T>
create_tree(const T& root, const tree_types&... ts) {
  using tree_type = std::common_type_t<tree_types...>;
  tree_type res(root);
  ( res.append_child(ts) , ... );
  return res;
}
template<class T, class tree_type> constexpr auto
// requires T regular, tree_types are all tree<T>
create_tree(const T& root, const std::vector<tree_type>& ts) { // TODO .append_children() ?
  tree_type res(root);
  for (const auto& t : ts) {
    res.append_child(t);
  }
  return res;
}

template<
  class nested_tree_type,
  std::enable_if_t<std::decay_t<nested_tree_type>::is_nested_tree,int> =0
> constexpr auto
root(nested_tree_type&& t) -> decltype(auto) {
  return FWD(t).root();
}
template<
  class nested_tree_type,
  std::enable_if_t<std::decay_t<nested_tree_type>::is_nested_tree,int> =0
> constexpr auto
first_root(nested_tree_type&& t) {
  return FWD(t).first_root();
}
template<
  class nested_tree_type,
  std::enable_if_t<std::decay_t<nested_tree_type>::is_nested_tree,int> =0
> constexpr auto
last_root(nested_tree_type&& t) {
  return FWD(t).last_root();
}
template<
  class nested_tree_type,
  std::enable_if_t<std::decay_t<nested_tree_type>::is_nested_tree,int> =0
> constexpr auto
children(nested_tree_type&& t) {
  return FWD(t).children();
}
template<
  class nested_tree_type,
  std::enable_if_t<std::decay_t<nested_tree_type>::is_nested_tree,int> =0
> constexpr auto
first_child(nested_tree_type&& t) {
  return FWD(t).first_child();
}
template<
  class nested_tree_type,
  std::enable_if_t<std::decay_t<nested_tree_type>::is_nested_tree,int> =0
> constexpr auto
last_child(nested_tree_type&& t) {
  return FWD(t).last_child();
}
template<class T, template<class> class M> constexpr auto
nb_children(const nested_tree_base<T,M>& t) -> int {
  return children(t).size();
}
template<
  class nested_tree_type,
  std::enable_if_t<std::decay_t<nested_tree_type>::is_nested_tree,int> =0
> constexpr auto
child(nested_tree_type&& t, int i) {
  return children(FWD(t))[i];
}

template<class T, template<class> class M> constexpr auto
is_leaf(const nested_tree_base<T,M>& t) -> bool {
  return t.size()==1;
}
// public tree interface }


// Node_adjacency interface {
template<
  class nested_tree_type,
  std::enable_if_t<std::decay_t<nested_tree_type>::is_nested_tree,int> =0
> constexpr auto
node(nested_tree_type&& t) -> decltype(auto) {
  return root(FWD(t));
}
template<
  class nested_tree_type,
  std::enable_if_t<std::decay_t<nested_tree_type>::is_nested_tree,int> =0
> constexpr auto
out_nodes(nested_tree_type&& t) {
  return children(FWD(t));
}
// Node_adjacency interface }


//
// Note: this dfs is "optimized" for a nested tree,
// but it is intrusive and can't do post-ordering
template<class nested_tree_type, class nested_tree_visitor_type> auto
// requires nested_tree_type is nested_tree<T,M>
// requires nested_tree_visitor_type has .pre(T), .up(), .down()
nested_tree_dfs_with_preordering(nested_tree_type&& t, nested_tree_visitor_type&& f) -> void {
  std::vector<int> remaining_nodes_stack = {};

  remaining_nodes_stack.push_back(t.sizes[0]);
  f.pre(t.nodes[0]);
  f.down();
  for (size_t i=1; i<t.size(); ++i) {
    while (remaining_nodes_stack.back()==1) { // only top node remaining, i.e. all children of this level have been visited
      remaining_nodes_stack.pop_back();
      f.up();
    }
    remaining_nodes_stack.back() -= t.sizes[i];

    f.pre(t.nodes[i]);

    if (not is_leaf(t)) {
      remaining_nodes_stack.push_back(t.sizes[i]);
      f.down();
    }
  }
}


// to_string {
const std::string unit_indent = "  ";
class to_string_dfs_visitor {
  public:
    template<class T> auto
    pre(const T& node){
      using std::to_string;
      s += indent+to_string(node)+"\n";
    }
    auto
    down() {
      indent += unit_indent;
    }
    auto
    up(){
      std_e::remove_back_n(indent,unit_indent.size());
    }

    auto
    str() -> const std::string& {
      return s;
    }
  private:
    std::string s = "";
    std::string indent = "";
};
template<class T, template<class> class M> auto
to_string(const nested_tree_base<T,M>& t) -> std::string {
  to_string_dfs_visitor f;
  nested_tree_dfs_with_preordering(t,f);
  return f.str();
}
// to_string }


/// C++ hack {
/// (give linkage to constexpr value so it can be passed as template reference)
template<const auto& t, int i>
// requires decltype(t)==nested_tree<T>
struct child__with_linkage {
  static constexpr auto value = child(t,i);
};
/// C++ hack }


} // std_e
