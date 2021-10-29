#pragma once


#include "std_e/execution/task/task.hpp"
#include "std_e/execution/task/utils.hpp"
#include "std_e/meta/meta.hpp"


namespace std_e {


template<class T>
class input_task {
  private:
    using R = task_result_stored_type<std_e::remove_rvalue_reference<T>>;
    R result;
  public:
    static constexpr bool enable_task = true;
    using result_type = R;

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
