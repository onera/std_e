#pragma once


#include "std_e/execution/task/task.hpp"
#include "std_e/meta/meta.hpp"


namespace std_e {


namespace detail {
  template<class T>
  struct replace_by_pointer_if_ref_impl {
    using type = T;
  };
  template<class T>
  struct replace_by_pointer_if_ref_impl<T&> {
    using type = T*;
  };
}
template<class T>
using replace_by_pointer_if_ref = typename detail::replace_by_pointer_if_ref_impl<T>::type;


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
    replace_by_pointer_if_ref<R> result; // if R is a reference, we store a pointer here
                                         // since we can't initialize it at construction-time
  public:
    using result_type = R;
    static constexpr bool enable_task = true;

    then_task(task_kind kd, F&& f, Args&... args)
      : kd(kd)
      , f(FWD(f))
      , args(args...)
    {
      if constexpr (std::is_reference_v<R>) {
        result = nullptr;
      }
    }

    auto
    execute() -> void {
      if constexpr (std::is_reference_v<R>) {
        result = &apply_forward_as<Args...>(f,args);
      } else {
        result = apply_forward_as<Args...>(f,args);
      }
    }

    auto
    kind() const -> task_kind {
      return kd;
    }

    auto
    result_ptr() -> void* {
      if constexpr (std::is_reference_v<R>) {
        return result;
      } else {
        return &result;
      }
    }
};


} // std_e
