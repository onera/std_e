#pragma once

#include "std_e/graph/algorithm/algo_nodes.hpp"


namespace graph {


template<class Unary_op, class Container>
class preorder_recorder {
  public:
  // traits
    using unary_op_type = std::decay_t<Unary_op>;
    using recorded_type = typename Container::value_type;
  // ctor
    template<class Unary_op_0>
    preorder_recorder(Unary_op_0&& op, Container& recorded_values)
      : op(FWD(op))
      , recorded_values(recorded_values)
    {}
  // Graph_node_visitor interface
    template<class node_type> auto
    // requires std::invoke_result_t<Unary_op,node_type>==recorded_type
    pre(const node_type& n) -> void {
      recorded_values.push_back(op(n));
    }
    template<class node_type> auto
    // requires std::invoke_result_t<Unary_op,node_type>==recorded_type
    post(const node_type&) -> void {}

  private:
    std_e::remove_rvalue_reference<Unary_op> op;
    Container& recorded_values;
};

template<class Unary_op, class Container>
class postorder_recorder {
  public:
  // traits
    using unary_op_type = std::decay_t<Unary_op>;
    using recorded_type = typename Container::value_type;
  // ctor
    template<class Unary_op_0>
    postorder_recorder(Unary_op_0&& op, Container& recorded_values)
      : op(FWD(op))
      , recorded_values(recorded_values)
    {}
  // Graph_node_visitor interface
    template<class node_type> auto
    // requires std::invoke_result_t<Unary_op,node_type>==recorded_type
    pre(const node_type&) -> void {}
    template<class node_type> auto
    // requires std::invoke_result_t<Unary_op,node_type>==recorded_type
    post(const node_type& n) -> void {
      recorded_values.push_back(op(n));
    }

  private:
    std_e::remove_rvalue_reference<Unary_op> op;
    Container& recorded_values;
};

template<class Unary_op_pre, class Container_pre, class Unary_op_post, class Container_post>
class prepostorder_recorder {
  public:
  // traits
    using preorder_recorder_type = preorder_recorder<Unary_op_pre,Container_pre>;
    using postorder_recorder_type = postorder_recorder<Unary_op_post,Container_post>;
  // ctor
    template<class Unary_op_pre_0, class Unary_op_post_0>
    prepostorder_recorder(
      Unary_op_pre_0&& op_pre, Container_pre& c_pre,
      Unary_op_post_0&& op_post, Container_post& c_post
    )
      : pre_rec(FWD(op_pre),c_pre)
      , post_rec(FWD(op_post),c_post)
    {}
  // Graph_node_visitor interface
    template<class node_type> auto
    // requires std::invoke_result_t<Unary_op,node_type>==recorded_type
    pre(const node_type& n) -> void {
      pre_rec.pre(n);
    }
    template<class node_type> auto
    // requires std::invoke_result_t<Unary_op,node_type>==recorded_type
    post(const node_type& n) -> void {
      post_rec.post(n);
    }

  private:
    preorder_recorder_type pre_rec;
    postorder_recorder_type post_rec;
};


template<class Graph, class Unary_op>
using record_container_type = 
  std::vector<
    std::remove_reference_t<
      std::invoke_result_t<
        Unary_op,
        typename Graph::node_type
      >
    >
  >;

template<class Graph, class Unary_op> FORCE_NO_INLINE constexpr auto
preorder_record_depth_first(const Graph& g, Unary_op&& op) {
  using container_type = record_container_type<Graph,Unary_op>;
  container_type recorded_values;

  preorder_recorder<Unary_op&&,container_type> recorder(FWD(op),recorded_values);
  prepostorder_depth_first_scan(g,recorder);

  return recorded_values;
}
template<class Graph, class Unary_op> FORCE_NO_INLINE constexpr auto
postorder_record_depth_first(const Graph& g, Unary_op&& op) {
  using container_type = record_container_type<Graph,Unary_op>;
  container_type recorded_values;

  postorder_recorder<Unary_op&&,container_type> recorder(FWD(op),recorded_values);
  prepostorder_depth_first_scan(g,recorder);

  return recorded_values;
}
template<class Graph, class Unary_op_pre, class Unary_op_post> FORCE_NO_INLINE constexpr auto
prepostorder_record_depth_first(const Graph& g, Unary_op_pre&& op_pre, Unary_op_post&& op_post) {
  using container_pre_type = record_container_type<Graph,Unary_op_pre>;
  using container_post_type = record_container_type<Graph,Unary_op_post>;
  container_pre_type c_pre;
  container_post_type c_post;

  using recorder_type = prepostorder_recorder<Unary_op_pre&&,container_pre_type,Unary_op_post&&,container_post_type>;
  recorder_type recorder(FWD(op_pre),c_pre,FWD(op_post),c_post);
  prepostorder_depth_first_scan(g,recorder);

  return make_pair(std::move(c_pre),std::move(c_post));
}


} // graph 
