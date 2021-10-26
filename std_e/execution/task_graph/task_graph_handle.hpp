#pragma once


#include "std_e/execution/task_graph/task_graph.hpp"


namespace std_e {


template<class T> concept Task_graph_handle = std::remove_cvref_t<T>::enable_task_graph_handle; // TODO not really a concept


template<class R, bool one_shot>
struct task_graph_handle { // TODO make class (invariant: result points to tg result)
  static constexpr bool enable_task_graph_handle = true;
  static constexpr bool single_shot = one_shot;
  task_graph* tg;
  R* result; // points to tg.result (needed to keep this as a typed information)
  int active_node_idx;
};

template<class R> using single_shot_task_graph_handle = task_graph_handle<R,true>;
template<class R> using multi_shot_task_graph_handle = task_graph_handle<R,false>;


} // std_e
