#pragma once


#include "std_e/execution/task_graph/task_graph.hpp"


namespace std_e {


auto
execute_seq(Task_graph_handle auto& tgh) {
  task_graph& tg = *tgh.tg;
  int n = tg.size();
  for (int i=0; i<n; ++i) {
    tg.node(i).execute();
  }
  return *tgh.result;
}


} // std_e
