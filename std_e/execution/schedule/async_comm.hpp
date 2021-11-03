#pragma once


#include "std_e/execution/future/future.hpp"
#include "std_e/execution/thread_pool/thread_pool.hpp"
#include <mpi.h>


namespace std_e {

auto
execute_async_comm(task_graph& tg, thread_pool& comm_tp) -> void;

template<class R> requires(!std::is_lvalue_reference_v<R>) auto
execute_async_comm(future<R>& fut, thread_pool& comm_tp) -> std::remove_reference_t<R> {
  int provided_thread_support;
  MPI_Query_thread(&provided_thread_support);
  STD_E_ASSERT(provided_thread_support==MPI_THREAD_MULTIPLE);

  execute_async_comm(*fut.graph(),comm_tp);
  return *fut.result();
}


} // std_e
