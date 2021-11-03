#pragma once


#include <memory>
#include "std_e/execution/task/task.hpp"
#include "std_e/base/macros.hpp"


namespace std_e {


namespace detail {

struct any_task_internal_base {
  virtual auto execute() -> void = 0;
  virtual auto result_ptr() -> void* = 0;
  virtual auto result_ptr_ptr() -> void* = 0;
  virtual auto kind() const -> task_kind = 0;

  virtual ~any_task_internal_base() {}
};

template<is_task Task>
struct any_task_internal_impl: any_task_internal_base {
  public:
    any_task_internal_impl(Task t)
      : t(std::move(t))
    {}

    auto execute() -> void override {
      return t.execute();
    }
    auto result_ptr() -> void* override {
      return t.result_ptr();
    }
    auto result_ptr_ptr() -> void* override {
      return t.result_ptr_ptr();
    }

    auto kind() const -> task_kind override {
      return t.kind();
    }
  private:
    Task t;
};

} // detail


struct null_task {
  static constexpr bool enable_task = true;
  auto execute() const -> void {}
  auto result_ptr() const -> void* { return nullptr; }
  auto result_ptr_ptr() const -> void* { return nullptr; }
  auto kind() const -> task_kind { return task_kind::no_op; }
};

class any_task {
  public:
    any_task()
      : any_task(null_task{})
    {}

    template<is_task Task>
    any_task(Task&& x):
      impl(std::make_unique<detail::any_task_internal_impl<std::decay_t<Task>>>(FWD(x)))
    {}

    auto execute() {
      impl->execute();
    }

    auto result_ptr() -> void* {
      return impl->result_ptr();
    }
    auto result_ptr_ptr() -> void* {
      return impl->result_ptr_ptr();
    }
    auto kind() const -> task_kind {
      return impl->kind();
    }
  private:
    std::unique_ptr<detail::any_task_internal_base> impl;
};

inline auto
to_string(const any_task&) -> std::string {
  return "";
}


} // std_e
