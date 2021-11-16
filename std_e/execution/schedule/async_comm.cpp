#include "std_e/execution/schedule/async_comm.hpp"


#include <queue>


namespace std_e {


#if __cpp_lib_atomic_wait
enum task_state {
  not_started,
  finished
};
struct comm_task_state {
  int id;
  std::unique_ptr<std::atomic_bool> done_flag;
};


auto
push_to_queue_if_deps_satisfied(const task_graph& tg, int node_i, std::queue<int>& q, const std::vector<task_state>& state_of_tasks) -> void {
  for (auto& next_i : tg[node_i].out_indices()) {
    auto next = tg[next_i];
    auto& deps = next.in_indices();
    auto task_is_finished = [&g=state_of_tasks](int i){ return g[i] == finished; };
    bool deps_are_satisfied = all_of(begin(deps),end(deps),task_is_finished);
    if (deps_are_satisfied) {
      q.push(next_i);
    }
  }
}

auto
wait_one_comm_to_finish(
  std::deque<comm_task_state>& flying_tasks, std::vector<task_state>& state_of_tasks,
  const task_graph& tg, std::queue<int>& q, int& n_task_remaining,
  const std::atomic_uint& current_comm, unsigned& last_recorded_comm
) -> void
{
  int n_flying_task = flying_tasks.size();
  STD_E_ASSERT(n_flying_task>0);

  current_comm.wait(last_recorded_comm,std::memory_order_relaxed);
  for (int i=0; i<n_flying_task; ++i) {
    int ii = flying_tasks[i].id;
    bool done_flag = flying_tasks[i].done_flag->load(std::memory_order_acquire);
    if (done_flag) {
      // take into account that the task is done
      state_of_tasks[ii] = finished;
      ++last_recorded_comm;
      --n_task_remaining;
      flying_tasks.erase(flying_tasks.begin()+i);

      push_to_queue_if_deps_satisfied(tg,ii,q,state_of_tasks);
      //STD_E_ASSERT(!q.empty()); // is this really a post-condition? Not sure...
      return; // need to break from loop because we called flying_tasks.erase
    }
  }
}

auto
execute_async_comm(task_graph& tg, thread_pool& comm_tp) -> void {

  // Associate a completion state to each task of the graph
  std::vector<task_state> state_of_tasks(tg.size(),not_started);

  // List of comm_tasks that have been launched, are maybe executing or even finished, but whose completion is not acknowledged yet
  std::deque<comm_task_state> flying_tasks;

  // Variables to record the fact that a comm_task has newly completed
  // (Use of unsigned integers to give modular arithmetic ids to comm_tasks)
  std::atomic_uint current_comm = 0;
  unsigned last_recorded_comm = 0;

  // Queue of task indices to be executed
  std::queue<int> q;
  // init with the queue with roots (tasks that do not depend on others)
  for (int i=0; i<tg.size(); ++i) {
    if (tg[i].in_degree()==0) {
      STD_E_ASSERT(tg.node(i).kind()==task_kind::input);
      q.push(i);
    }
  }

  int n_task_remaining = tg.size();
  while (n_task_remaining) {
    if (!q.empty()) {
      int i = q.front();
      q.pop();
      any_task& t = tg.node(i);
      if (t.kind()==task_kind::communication) {
        auto& i_status = flying_tasks.emplace_back(comm_task_state{i,std::make_unique<std::atomic_bool>(false)});
        auto& done_flag = *i_status.done_flag;
        comm_tp.push_task([&t,&done_flag,&current_comm](){
            t.execute();
            done_flag.store(true,std::memory_order_release);
            current_comm.fetch_add(1,std::memory_order_release); // memory_order_relaxed should be enougth
            current_comm.notify_one();
        });
      } else {
        t.execute();
        state_of_tasks[i] = finished;
        --n_task_remaining;

        push_to_queue_if_deps_satisfied(tg,i,q,state_of_tasks);
      }
    } else {
      wait_one_comm_to_finish(flying_tasks,state_of_tasks,tg,q,n_task_remaining,current_comm,last_recorded_comm);
    }
  }
}

#else // __cpp_lib_atomic_wait not available
auto
execute_async_comm(task_graph& tg, thread_pool& comm_tp) -> void {
  throw not_implemented_exception("execute_async_comm not supported because your compiler does not implement std::atomic waiting (__cpp_lib_atomic_wait)");
}
#endif

} // std_e
