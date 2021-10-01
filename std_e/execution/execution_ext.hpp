#pragma once


#include "std_e/execution/sender.hpp"
#include "std_e/execution/execution.hpp"


namespace std_e {


enum class task_kind {
  computation, communication
};


class any_task {
  virtual auto execute() -> void = 0;
  virtual ~any_task() = default;
};


template<class F, class Arg>
class task : any_task {
  private:
    task_kind kd;
    std::atomic<int> remaining_deps;

    using R = std::invoke_result_t<F,Arg>;
    F f;
    Arg* arg;
    R result;
  public:
    task(task_kind kd, int n_deps, F&& f, Arg* arg)
      : kd(kd)
      , remaining_deps(n_deps)
      , f(std::move(f))
      , arg(arg)
    {}

    auto
    execute() -> void {
      result = f(*arg);
    }

    auto
    kind() -> task_kind {
      return kd;
    }
};


struct comm_overlap_thread_pool {
  std::thread comp_th;
  std::thread comm_th;
};

struct scheduler {
  comm_overlap_thread_pool tp;
}

class execution {
  private:
    std::deque<std::unique_ptr<any_task>> tasks;
  public:
    auto run() -> void {
      std::thread comp_th;
      std::thread comm_th;
    }
};

class execution_graph {
  private:
    thread_pool& calc_tp;
    thread_pool& comm_tp;
  public:
    execution_graph(thread_pool& calc_tp, thread_pool& comm_tp)
      : calc_tp(calc_tp)
      , comm_tp(comm_tp)
    {}

    template<sender S> auto
    run(S&& s) {
      using R = decltype(::std_e::execute(s));
      R res = 
      return res;
    }
};

main() {
  thread_pool calc_tp(1);
  thread_pool comm_tp(8);

  auto s0 = input_sender(std::vector{3,0,1,2}) | then(sort_vec) | split();
  auto s1 = s0 | then_comm(get_remote_info);
  auto s2 = s0 | then(max_vec);
  auto s3 = wait_all(s1,s2);

  execution_graph eg(calc_tp,comm_tp);
  auto res = eg.run(std::move(s3));

  CHECK( res == std::tuple{std::vector{0,1,2,3, 6,5,4,7}, 3} );
}




} // std_e
