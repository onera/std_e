#pragma once


#include "std_e/execution/task/task.hpp"
#include "std_e/execution/task_graph/task_graph_handle.hpp"


namespace std_e {


template<class R> auto
move_result_if_single_shot(R& r, std::true_type) -> R {
  return std::move(r);
}
template<class R> auto
move_result_if_single_shot(R& r, std::false_type) -> R& {
  return r;
}

template<Task_graph_handle... tghs> // TODO do not depend on task_g_h
class join_task {
  private:
    using dep_result_refs = std::tuple<decltype(*std::declval<tghs>().result)&...>;
    dep_result_refs deps;
    using R = std::tuple<decltype(move_result_if_single_shot(*std::declval<tghs>().result,std::bool_constant<tghs::single_shot>{}))...>;
    R result;
  public:
    using result_type = R;
    static constexpr size_t N = std::tuple_size_v<R>;
    static constexpr bool enable_task = true;

    template<class... Args>
    join_task(Args&... args)
      : deps({args...})
    {}

    template<size_t... Is> auto
    execute__impl(std::index_sequence<Is...>) -> void {
      result = R{move_result_if_single_shot(std::get<Is>(deps),std::bool_constant<tghs::single_shot>{})...};
    }
    auto
    execute() -> void {
      execute__impl(std::make_index_sequence<N>{});
    }

    auto
    kind() const -> task_kind {
      return task_kind::computation;
    }

    auto
    result_ptr() -> void* {
      return &result;
    }
};


} // std_e
