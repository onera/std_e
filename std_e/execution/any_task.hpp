#pragma once


#include <utility>


namespace std_e {


namespace detail {

template<class T> concept is_task = std::remove_cvref_t<T>::enable_task; // TODO not really a concept

struct any_task_internal_base {
  virtual auto execute() -> void = 0;
  virtual auto result_ptr() -> void* = 0;

  virtual ~any_task_internal_base() {}
};

template<is_task Task>
struct any_task_internal_impl: any_task_internal_base {
  public:
    any_task_internal_impl(Task t)
      : t(std::move(t))
    {}

    auto execute() -> void {
      return t.execute();
    }
    auto result_ptr() -> void* {
      return t.result_ptr();
    }
  private:
    Task t;
};

} // detail


struct null_task {
  static constexpr bool enable_task = true;
  auto execute() const -> void {}
  auto result_ptr() const -> void* { return nullptr; }
};

class any_task {
  public:
    any_task()
      : any_task(null_task{})
    {}

    template<detail::is_task Task>
    any_task(Task&& x):
      impl(std::make_unique<detail::any_task_internal_impl<std::decay_t<Task>>>(FWD(x)))
    {}

    auto execute() {
      impl->execute();
    }

    auto result_ptr() -> void* { 
      return impl->result_ptr();
    }
  private:
    std::unique_ptr<detail::any_task_internal_base> impl;
};

inline auto
to_string(const any_task&) -> std::string {
  return "";
}


} // std_e
