#pragma once


#include "std_e/execution/task/task.hpp"
#include "std_e/meta/meta.hpp"


namespace std_e {


template<bool after_single_shot, class F, class... Args>
requires
    (  after_single_shot && std::invocable<remove_rvalue_reference<F>,std::remove_reference_t<Args>&&...>)
 || ( !after_single_shot && std::invocable<remove_rvalue_reference<F>,std::remove_reference_t<Args>& ...>)
class then_task {
  private:
    task_kind kd;

    using R = std::invoke_result_t<F,Args...>;
    //static_assert(!std::is_reference_v<R>);

    remove_rvalue_reference<F> f;
    std::tuple<Args&...> args;
    R result;
  public:
    using result_type = R;
    static constexpr bool enable_task = true;

    then_task(task_kind kd, F&& f, Args&... args)
      : kd(kd)
      , f(FWD(f))
      , args(args...)
    {}

    auto
    execute() -> void {
      if constexpr (after_single_shot) {
        result = apply_move(f,args);
      } else {
        result = std::apply(f,args);
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
