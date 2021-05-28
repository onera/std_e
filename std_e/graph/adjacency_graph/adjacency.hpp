#pragma once


#include "std_e/base/not_implemented_exception.hpp" // TODO
#include "std_e/graph/adjacency_graph/traits/traits.hpp"
#include "std_e/log.hpp" // TODO


namespace std_e {


template<class adjacency_graph_type>
class adjacency {
  public:
    using index_type = typename adjacency_graph_type::index_type;
    using node_type  = typename adjacency_graph_type::node_type;

    constexpr
    adjacency() = default;

    constexpr
    adjacency(adjacency_graph_type* g, index_type node_idx)
      : g(g)
      , node_idx(node_idx)
    {}

    constexpr auto
    node_index() const -> index_type {
      return node_idx;
    }

    constexpr auto
    node() -> node_type& {
      return g->nodes()[node_idx];
    }
    constexpr auto
    node() const -> const node_type& {
      return g->nodes()[node_idx];
    }

    constexpr auto
    degree() const -> index_type {
      return g->degree(node_idx);
    }
    constexpr auto
    edges() -> auto& {
      return g->edges()[node_idx];
    }
    constexpr auto
    edges() const -> const auto& {
      return g->edges()[node_idx];
    }

    constexpr auto
    adjacencies() -> adjacency_range<adjacency_graph_type> {
      return {g,node_idx};
    }
    constexpr auto
    adjacencies() const -> adjacency_range<const adjacency_graph_type> {
      return {g,node_idx};
    }

    template<class AGT0, class AGT1> friend constexpr auto
    operator==(const adjacency<AGT0>& x, const adjacency<AGT1>& y) -> bool;
  private:
    adjacency_graph_type* g;
    index_type node_idx;
};
template<class AGT0, class AGT1> constexpr auto
operator==(const adjacency<AGT0>& x, const adjacency<AGT1>& y) -> bool {
  return
      x.g == y.g
   && x.node_idx == y.node_idx;
}
template<class AGT0, class AGT1> constexpr auto
operator!=(const adjacency<AGT0>& x, const adjacency<AGT1>& y) -> bool {
  return !(x==y);
}


template<class adjacency_graph_type>
class io_adjacency {
  public:
    using index_type = typename adjacency_graph_type::index_type;
    //using node_type  = typename adjacency_graph_traits<adjacency_graph_type>::node_type;

    constexpr
    io_adjacency() = default;

    constexpr
    io_adjacency(adjacency_graph_type* g, index_type node_idx)
      : g(g)
      , node_idx(node_idx)
    {}
    constexpr
    io_adjacency(const io_adjacency& x) = default;
    constexpr
    io_adjacency(io_adjacency&& x) = default;

    constexpr auto
    index() const -> index_type { // TODO del
      return node_idx;
    }
    constexpr auto
    node_index() const -> index_type {
      return node_idx;
    }
    constexpr auto
    in_indices() -> auto& {
      return g->in_indices(node_idx);
    }
    constexpr auto
    in_indices() const -> const auto& {
      return g->in_indices(node_idx);
    }
    constexpr auto
    out_indices() -> auto& {
      return g->out_indices(node_idx);
    }
    constexpr auto
    out_indices() const -> const auto& {
      return g->out_indices(node_idx);
    }

    constexpr auto
    node() -> auto& {
      return g->nodes()[node_idx];
    }
    constexpr auto
    node() const -> const auto& {
      return g->nodes()[node_idx];
    }

    constexpr auto
    in_degree() const -> index_type {
      return g->in_degree(node_idx);
    }
    constexpr auto
    out_degree() const -> index_type {
      return g->out_degree(node_idx);
    }
    constexpr auto
    in_edges() -> auto& {
      return g->in_edges()[node_idx];
    }
    constexpr auto
    in_edges() const -> const auto& {
      return g->in_edges()[node_idx];
    }
    constexpr auto
    out_edges() -> auto& {
      return g->out_edges()[node_idx];
    }
    constexpr auto
    out_edges() const -> const auto& {
      return g->out_edges()[node_idx];
    }

    constexpr auto
    in_adjacencies() -> adjacency_range<adjacency_graph_type,adj_orientation::in> {
      return {g,node_idx};
    }
    constexpr auto
    in_adjacencies() const -> adjacency_range<const adjacency_graph_type,adj_orientation::in> {
      return {g,node_idx};
    }
    constexpr auto
    out_adjacencies() -> adjacency_range<adjacency_graph_type,adj_orientation::out> {
      return {g,node_idx};
    }
    constexpr auto
    out_adjacencies() const -> adjacency_range<const adjacency_graph_type,adj_orientation::out> {
      return {g,node_idx};
    }
    auto
    operator=(const io_adjacency& old) -> io_adjacency& {
      throw not_implemented_exception("op =");
      return *this;
    }
    auto
    operator=(io_adjacency&& old) -> io_adjacency& {
      throw not_implemented_exception("op =");
      return *this;
    }

    template<class AGT0, class AGT1> friend constexpr auto
    operator==(const io_adjacency<AGT0>& x, const io_adjacency<AGT1>& y) -> bool;
  private:
    adjacency_graph_type* g;
    index_type node_idx;
};
template<class AGT0, class AGT1> constexpr auto
operator==(const io_adjacency<AGT0>& x, const io_adjacency<AGT1>& y) -> bool {
  // TODO TEST!!! TODO in_adjacencies !!!!!
  LOG("\n");
  ELOG(x.node());
  ELOG(y.node());
  ELOG(x.out_indices());
  ELOG(y.out_indices());
  LOG("\n");
  return x.node()==y.node() && x.out_indices()==y.out_indices();
//  return
//      x.g == y.g
//   && x.node_idx == y.node_idx;
}
template<class AGT0, class AGT1> constexpr auto
operator!=(const io_adjacency<AGT0>& x, const io_adjacency<AGT1>& y) -> bool {
  return !(x==y);
}
// TODO
template<class AGT0, class AGT1> constexpr auto
operator<(const io_adjacency<AGT0>& x, const io_adjacency<AGT1>& y) -> bool {
  return
       x.node()<y.node()
   || (x.node()==y.node() && x.out_adjacencies()<y.out_adjacencies());
}

template<class AGT> constexpr auto
in_indices(io_adjacency<AGT>& x) -> auto& {
  return x.in_indices();
}
template<class AGT> constexpr auto
in_indices(io_adjacency<AGT>&& x) -> auto& {
  return x.in_indices();
}
template<class AGT> constexpr auto
in_indices(const io_adjacency<AGT>& x) -> const auto& {
  return x.in_indices();
}
template<class AGT> constexpr auto
out_indices(io_adjacency<AGT>& x) -> auto& {
  return x.out_indices();
}
template<class AGT> constexpr auto
out_indices(io_adjacency<AGT>&& x) -> auto& {
  return x.out_indices();
}
template<class AGT> constexpr auto
out_indices(const io_adjacency<AGT>& x) -> const auto& {
  return x.out_indices();
}
template<class AGT> constexpr auto
node(io_adjacency<AGT>& x) -> auto& {
  return x.node();
}
template<class AGT> constexpr auto
node(io_adjacency<AGT>&& x) -> auto& {
  return x.node();
}
template<class AGT> constexpr auto
node(const io_adjacency<AGT>& x) -> const auto& {
  return x.node();
}
template<class AGT> constexpr auto
in_adjacencies(io_adjacency<AGT>& x) {
  return x.in_adjacencies();
}
template<class AGT> constexpr auto
in_adjacencies(io_adjacency<AGT>&& x) {
  return x.in_adjacencies();
}
template<class AGT> constexpr auto
in_adjacencies(const io_adjacency<AGT>& x) {
  return x.in_adjacencies();
}
template<class AGT> constexpr auto
out_adjacencies(io_adjacency<AGT>& x) {
  return x.out_adjacencies();
}
template<class AGT> constexpr auto
out_adjacencies(io_adjacency<AGT>&& x) {
  return x.out_adjacencies();
}
template<class AGT> constexpr auto
out_adjacencies(const io_adjacency<AGT>& x) {
  return x.out_adjacencies();
}

// tree interface {
template<class AGT> constexpr auto
first_child(io_adjacency<AGT>& x) {
  return out_adjacencies(x).begin();
}
template<class AGT> constexpr auto
first_child(io_adjacency<AGT>&& x) {
  return out_adjacencies(x).begin();
}
template<class AGT> constexpr auto
first_child(const io_adjacency<AGT>& x) {
  return out_adjacencies(x).begin();
}
template<class AGT> constexpr auto
last_child(io_adjacency<AGT>& x) {
  return out_adjacencies(x).end();
}
template<class AGT> constexpr auto
last_child(io_adjacency<AGT>&& x) {
  return out_adjacencies(x).end();
}
template<class AGT> constexpr auto
last_child(const io_adjacency<AGT>& x) {
  return out_adjacencies(x).end();
}

template<class AGT> constexpr auto
children(io_adjacency<AGT>& x) {
  return out_adjacencies(x);
}
template<class AGT> constexpr auto
children(io_adjacency<AGT>&& x) {
  return out_adjacencies(x);
}
template<class AGT> constexpr auto
children(const io_adjacency<AGT>& x) {
  return out_adjacencies(x);
}

template<class AGT> auto
to_string(const io_adjacency<AGT>& x) -> std::string {
  using std::to_string;
  return to_string(x.node())+" : "+range_to_string(x.in_indices())+" | "+range_to_string(x.out_indices());
}
// tree interface}


} // std_e
