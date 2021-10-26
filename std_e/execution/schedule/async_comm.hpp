#pragma once


#include "std_e/execution/task_graph/task_graph_handle.hpp"
#include "std_e/execution/thread_pool.hpp"


namespace std_e {

auto
execute_async_comm(task_graph& tg, thread_pool& comm_tp) -> void;

auto
execute_async_comm(Task_graph_handle auto& tgh, thread_pool& comm_tp) {
  execute_async_comm(*tgh.tg,comm_tp);
  return *tgh.result;
}


} // std_e
