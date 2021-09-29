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
    using R = std::invoke_result_t<F,Arg>;
    F f;
    Arg* arg;
    R result;
    task_kind kd;
  public:
    task(task_kind kd, F&& f, Arg* arg)
      : f(std::move(f))
      , arg(arg)
      , kd(kd)
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
    void run() -> void {
      std::thread comp_th;
      std::thread comm_th;
    }
};


} // std_e
