#pragma once


#include "std_e/execution/task_graph/task_graph.hpp"


namespace std_e {


template<class R> requires(!std::is_lvalue_reference_v<R>) auto
execute_seq(task_graph_handle<R>& tgh) -> std::remove_reference_t<R> {
  task_graph& tg = *tgh.tg;
  int n = tg.size();
  for (int i=0; i<n; ++i) {
    tg.node(i).execute();
  }
  return std::move(*tgh.result);
}
template<class R> requires(!std::is_lvalue_reference_v<R>) auto
execute_seq2(task_graph_handle<R>& tgh) -> std::remove_reference_t<R> {
  task_graph& tg = *tgh.tg;
  ELOG(tg.size())
  //ELOG(tg.node(0).result_ptr())
  ELOG(tg.node(2).result_ptr())
  int n = tg.size();
  for (int i=0; i<n; ++i) {
    tg.node(i).execute();
    ELOG(tg.node(2).result_ptr())
  }
  return std::move(*tgh.result);
}


} // std_e
