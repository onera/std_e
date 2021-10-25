#pragma once


#include "std_e/execution/task/task.hpp"
#include "std_e/meta/meta.hpp"


namespace std_e {


template<class F, class Arg, bool after_single_shot = true>
requires
    (  after_single_shot && std::invocable<remove_rvalue_reference<F>,std::remove_reference_t<Arg>&&>)
 || ( !after_single_shot && std::invocable<remove_rvalue_reference<F>,std::remove_reference_t<Arg>& >)
class then_task {
  private:
    task_kind kd;

    using R = std::invoke_result_t<F,Arg>;
    static_assert(!std::is_reference_v<R>);

    remove_rvalue_reference<F> f;
    Arg& arg;
    R result;
  public:
    using result_type = R;
    static constexpr bool enable_task = true;

    then_task(task_kind kd, F&& f, Arg& arg)
      : kd(kd)
      , f(FWD(f))
      , arg(arg)
    {}

    auto
    execute() -> void {
      if constexpr (after_single_shot) {
        result = f(std::move(arg));
      } else {
        result = f(arg);
      }
    }

    auto
    kind() const -> task_kind {
      return kd;
    }

    auto
    result_ptr() -> void* {
      return &result;
    }
};


} // std_e
