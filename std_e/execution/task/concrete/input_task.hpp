#pragma once


#include "std_e/execution/task/task.hpp"


namespace std_e {


template<class T>
class input_task {
  private:
    T result;
  public:
    static constexpr bool enable_task = true;

    input_task(T&& x)
      : result(FWD(x))
    {}

    auto
    execute() -> void {}

    auto
    kind() const -> task_kind {
      return task_kind::input;
    }

    auto
    result_ptr() -> void* {
      return &result;
    }
};


} // std_e
