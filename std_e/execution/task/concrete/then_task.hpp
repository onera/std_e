#pragma once


#include "std_e/execution/task/task.hpp"
#include "std_e/meta/meta.hpp"


namespace std_e {


template<class F, class... Args>
requires (std::invocable<remove_rvalue_reference<F>,Args...>)
class then_task {
  private:
    task_kind kd;

    using R = std::invoke_result_t<F,Args...>;
    //static_assert(!std::is_reference_v<R>);

    remove_rvalue_reference<F> f;
    std::tuple<Args&...> args; // note: we store lvalue references
                               // because the values are to be computed by another task
                               // and while their address is ready (and won't move)
                               // the value is not (and hence, can't be stored!)
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
     result = apply_forward_as<Args...>(f,args);
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
