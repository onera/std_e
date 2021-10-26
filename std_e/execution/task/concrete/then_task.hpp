#pragma once


#include "std_e/execution/task/task.hpp"
#include "std_e/meta/meta.hpp"


namespace std_e {

namespace detail {
  template <class F, class Tuple, std::size_t... I> constexpr auto
  apply_move_impl(F&& f, Tuple&& t, std::index_sequence<I...>) -> decltype(auto) {
    return std::invoke(std::forward<F>(f), std::move(std::get<I>(t))...);
  }
}  // namespace detail
 
template <class F, class Tuple> constexpr auto
apply_move(F&& f, Tuple&& t) -> decltype(auto) {
  constexpr auto N = std::tuple_size_v<std::remove_reference_t<Tuple>>;
  return detail::apply_move_impl(std::forward<F>(f), std::forward<Tuple>(t), std::make_index_sequence<N>{});
}


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
        //result = std::apply(f,std::move(args));
        result = apply_move(f,args);
        //result = f(std::move(std::get<0>(args)));
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
