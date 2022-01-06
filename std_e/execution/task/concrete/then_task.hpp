#pragma once


#include "std_e/execution/task/task.hpp"
#include "std_e/execution/task/task_result.hpp"
#include "std_e/meta/meta.hpp"


namespace std_e {

namespace detail {
  template<class F, class Tuple, size_t... Is, class Args_types> constexpr auto
  apply_ref_unwrap_and_forward_impl(F&& f, Tuple&& t, std::index_sequence<Is...>, Args_types) -> decltype(auto) {
    return std::invoke(FWD(f), unwrap_task_result(std::forward<types_element_t<Is,Args_types>>(std::get<Is>(t)))...);
  }
}
// apply f with t elements as arguments
// elements are moved, unless their corresponding type in "Args..." is an lvalue reference
// and, if they are of type task_ref_result_wrapper<T&>, the T& is extracted
// TODO test
template<class... Args, class F, class Tuple> constexpr auto
apply_ref_unwrap_and_forward(F&& f, Tuple&& t) -> decltype(auto) {
  static_assert(sizeof...(Args)==std::tuple_size_v<std::remove_cvref_t<Tuple>>);
  constexpr auto N = std::tuple_size_v<std::remove_reference_t<Tuple>>;
  return detail::apply_ref_unwrap_and_forward_impl(FWD(f), FWD(t), std::make_index_sequence<N>{}, std_e::types<Args...>{});
}

// TODO
//    Distinguish pointers coming from ptr-returning functions, and pointers coming from ref-returning functions
//    The former should not be deref, but currently are
template<class T>
struct remove_ref_wrapper__impl {
  using type = T;
};
template<class T>
struct remove_ref_wrapper__impl<T*> {
  using type = T&;
};
template<class T>
struct remove_ref_wrapper__impl<T*&> {
  using type = T&;
};
template<class T>
struct remove_ref_wrapper__impl<T* const&> {
  using type = T&;
};
template<class T>
struct remove_ref_wrapper__impl<T* &&> {
  using type = T&;
};
template<class T>
using remove_ref_wrapper = typename remove_ref_wrapper__impl<T>::type;


template<class F, class... Args>
requires (std::invocable<F,remove_ref_wrapper<Args>...>)
class then_task {
  private:
    task_kind kd;

    using R = std::invoke_result_t<F,remove_ref_wrapper<Args>...>;

    remove_rvalue_reference<F> f;
    std::tuple<Args&...> args; // note: we store lvalue references
                               // because the values are not ready (to be computed by another task)
                               // while their address is (and won't move)
    task_result_stored_type<R> result;
    task_result_stored_type<R>* result_p;
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
      result_p = &result;
    }

    then_task(const then_task& x) = delete;
    then_task operator=(const then_task& x) = delete;
    //then_task(then_task&& x) = delete;
    //then_task operator=(then_task&& x) = delete;

    then_task(then_task&& x)
      : kd(x.kd)
      , f(std::move(x.f))
      , args(std::move(x.args))
      , result(std::move(x.result))
      , result_p(&result)
    {}
    then_task& operator=(then_task&& x) {
      kd = x.kd;
      f = std::move(x.f);
      args = std::move(x.args);
      result = std::move(x.result);
      result_p = &result;
      return *this;
    }

    auto
    execute() -> void {
      if constexpr (std::is_reference_v<R>) {
        // see const_cast explanation in "task_result_stored_type" documentation
        result = const_cast<std::remove_cvref_t<R>*>(&apply_ref_unwrap_and_forward<Args...>(f,args));
      } else {
        result = apply_ref_unwrap_and_forward<Args...>(f,args);
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
    auto
    result_ptr_ptr() -> void* {
      return &result_p;
    }
};


} // std_e
