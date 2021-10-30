#pragma once


#include "std_e/execution/task/task.hpp"
#include "std_e/execution/task/task_result.hpp"
#include "std_e/meta/meta.hpp"


namespace std_e {


template<class T>
class input_task {
  private:
    using R = std_e::remove_rvalue_reference<T>;
    task_result_stored_type<R> result;
  public:
    static constexpr bool enable_task = true;
    using result_type = R;

    input_task(T&& x)
    requires (std::is_rvalue_reference_v<T&&>)
      : result(std::move(x))
    {}

    input_task(T& x)
      // see const_cast explanation in "task_result_stored_type" documentation
      : result(const_cast<std::remove_cvref_t<T>&>(x))
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
