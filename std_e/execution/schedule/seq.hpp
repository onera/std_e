#pragma once


#include "std_e/execution/future/future.hpp"


namespace std_e {


template<class R> requires(!std::is_lvalue_reference_v<R>) auto
execute_seq(future<R>& fut) -> std::remove_reference_t<R> {
  task_graph& tg = *fut.tg;
  int n = tg.size();
  for (int i=0; i<n; ++i) {
    tg.node(i).execute();
  }
  return std::move(*fut.result); // TODO do not force move (user can use e.g. extract_result_fn)
}


} // std_e
