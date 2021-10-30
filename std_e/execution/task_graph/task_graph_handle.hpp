#pragma once


#include "std_e/execution/task_graph/task_graph.hpp"
#include "std_e/execution/task/utils.hpp"


namespace std_e {


template<class T> concept Task_graph_handle = std::remove_cvref_t<T>::enable_task_graph_handle; // TODO not really a concept

template<class R>
using task_result_stored_type2 = task_ref_result_wrapper<std::remove_const_t<R>>;


template<class R>
struct task_graph_handle { // TODO make class (invariant: result points to tg result)
  static constexpr bool enable_task_graph_handle = true;
  using result_type = R;
  using result_stored_type = task_result_stored_type<R>;
  task_graph* tg;
  result_stored_type* result; // points to tg.result (needed to keep this as a typed information)
  int active_node_idx;

  task_graph_handle() = default;
  task_graph_handle(task_graph* tg, result_stored_type* result, int active_node_idx)
    : tg(tg)
    , result(result)
    , active_node_idx(active_node_idx)
  {}

  // non-const to const conversion
  template<class T>
  operator T() const {
    return {tg,result,active_node_idx};
  }
};

template<class R>
requires (std::is_same_v<R,std::remove_const_t<R>>)
struct task_graph_handle<R&> : task_graph_handle<const R&> {
  using base = task_graph_handle<const R&>;
  using base::base;
  using result_type = R&; // delegate everything to task_graph_handle<const R&> except for the result_type!
};


// if a task_graph_handle is temporary, then it has to be single_shot (i.e. its result is used only once, hence it can be moved)
// if it is not a temporary, it is assumed multi-shot, that is, its result won't be moved by default
template<Task_graph_handle TGH> constexpr bool is_single_shot = std::is_rvalue_reference_v<TGH>;

template<class T> using future = task_graph_handle<T>;


} // std_e
