#include "std_e/execution/schedule/async_comm.hpp"


#include <queue>
//#include <list>
#include "std_e/log.hpp" // TODO


namespace std_e {


const char not_started = 0;
const char started = 1;
const char finished = 2;

auto
push_to_queue_if_deps_satisfied(auto& tg, auto& node_i, auto& q, auto& graph_state) -> void {
  for (auto& next_i : tg[node_i].out_indices()) {
    auto next = tg[next_i];
    auto& deps = next.in_indices();
    bool deps_are_satisfied = all_of(begin(deps),end(deps),[&g=graph_state](int i){ return g[i] == finished; });
    if (deps_are_satisfied) {
      q.push(next_i);
    }
  }
}

auto
execute_async_comm(task_graph& tg, thread_pool& comm_tp) -> void {
  STD_E_ASSERT(tg.node(0).kind()==task_kind::input);
  std::deque<std::pair<int,std::unique_ptr<std::atomic_bool>>> comm_status;
  std::vector<char> graph_state(tg.size(),not_started);

  std::queue<int> q; q.push(0);

  int n_task_remaining = tg.size();

  while (n_task_remaining) {
    if (!q.empty()) {
      int i = q.front();
      q.pop();
      any_task& t = tg.node(i);
      if (t.kind()==task_kind::communication) {
        auto& i_done_flag = comm_status.emplace_back(std::pair{i,std::make_unique<std::atomic_bool>(false)});
        auto& done_flag = *i_done_flag.second;
        graph_state[i] = started;
        comm_tp.push_task([&t,&done_flag](){ t.execute(); done_flag = true; });
      } else {
        t.execute();
        graph_state[i] = finished;
        --n_task_remaining;

        push_to_queue_if_deps_satisfied(tg,i,q,graph_state);
      }
    } else { // TODO optimize busy wait with condition variable
      int n_flying_comm = comm_status.size();
      STD_E_ASSERT(n_flying_comm>0);
      while (true) {
        for (int i=0; i<n_flying_comm; ++i) {
          int& ii = comm_status[i].first;
          if (*comm_status[i].second) {
            // take into account that the task is done
            graph_state[ii] = finished;
            --n_task_remaining;
            comm_status.erase(comm_status.begin()+i);

            push_to_queue_if_deps_satisfied(tg,ii,q,graph_state);
            STD_E_ASSERT(!q.empty());

            goto end_spinloop;
          }
        }
      }
      end_spinloop: ;
    }
  }
          //ELOG(graph_state);
}


} // std_e