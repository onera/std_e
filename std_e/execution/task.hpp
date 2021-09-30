#pragma once


#include "std_e/execution/any_task.hpp"
#include "std_e/execution/pipeable.hpp"
#include "std_e/meta/meta.hpp"
#include <deque>
#include "std_e/graph/adjacency_graph/graph.hpp"


namespace std_e {


enum class task_kind {
  no_op, computation, communication
};


template<class T>
class input_task {
  private:
    T result;
  public:
    static constexpr bool enable_task = true;

    input_task(T&& x)
      : result(std::move(x))
    {}

    auto
    execute() -> void {}

    auto
    kind() -> task_kind {
      return task_kind::no_op;
    }

    auto
    result_ptr() -> void* {
      return &result;
    }
};

template<class F, class Arg, bool after_single_shot = true>
class task {
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

    task(task_kind kd, F&& f, Arg& arg)
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
    kind() -> task_kind {
      return kd;
    }

    auto
    result_ptr() -> void* {
      return &result;
    }
};

using task_graph_range_type = range_of_ranges<int,std::deque>;
using task_graph = io_graph</*node_type=*/any_task,/*edge_type=*/void,/*container=*/task_graph_range_type>;

template<class T> concept task_graph_handle = std::remove_cvref_t<T>::enable_task_graph_handle; // TODO not really a concept

template<class R, bool one_shot>
struct x_shot_task_graph_handle { // TODO make class (invariant: result points to tg result)
  static constexpr bool enable_task_graph_handle = true;
  static constexpr bool single_shot = one_shot;
  task_graph* tg;
  R* result; // points to tg.result (needed to keep this as a typed information)
  int active_node_idx;
};

template<class R> using single_shot_task_graph_handle = x_shot_task_graph_handle<R,true>;
template<class R> using multi_shot_task_graph_handle = x_shot_task_graph_handle<R,false>;

template<class T> requires(!std::is_lvalue_reference_v<T>) auto
input_data(task_graph& tg, T&& x) {
  input_task t(std::move(x));
  int n = tg.size();
  single_shot_task_graph_handle<T> ttg;
  ttg.tg = &tg;
  auto& emplaced_t = ttg.tg->emplace_back(std::move(t));
  ttg.result = static_cast<T*>(emplaced_t.result_ptr());
  ttg.active_node_idx = n;
  return ttg;
}


template<class F> auto
generic_then_task(task_kind tk, task_graph_handle auto&& tgh, F f) { // TODO F&&
  using task_t = std::conditional_t<
    std::remove_cvref_t<decltype(tgh)>::single_shot,
    task<F,std::remove_cvref_t<decltype(*tgh.result)>,/*after_single_shot=*/true>,
    task<F,decltype(*tgh.result)&,/*after_single_shot=*/false>
  >;
  task_t t(
    tk,
    FWD(f),
    *tgh.result
  );
  //decltype(t) x = "";
  using R = decltype(t)::result_type;
  single_shot_task_graph_handle<R> ttg;
  int n = tgh.tg->size();
  int last_active = tgh.active_node_idx;
  ttg.tg = tgh.tg; // the new graph is the old one...
  auto& emplaced_t = ttg.tg->emplace_back(std::move(t)); // ... but we add the new task ...
  ttg.tg->out_indices(last_active).push_back(n); // ... and tell the previous task that this one is following ...
  ttg.tg->in_indices(n).push_back(last_active); // ... and symmetrically tell the new task that it depends one the previous one
  ttg.result = static_cast<R*>(emplaced_t.result_ptr());
  ttg.active_node_idx = n; // the active task is the one we just added
  return ttg;
}

template<class R> auto
split(single_shot_task_graph_handle<R>&& tgh) {
  return multi_shot_task_graph_handle<R>{tgh.tg,tgh.result,tgh.active_node_idx}; // this is the same data, only seen as multi-shot now
}


template<class F> auto
then(task_graph_handle auto&& tgh, F&& f) {
  return generic_then_task(task_kind::computation,FWD(tgh),FWD(f));
}

template<class F> auto
then(F&& f) {
  return make_pipeable([f0=std::move(f)](auto&& s) {
    return then(FWD(s),std::move(f0));
  });
}

template<class F> auto
then_comm(task_graph_handle auto&& tgh, F&& f) {
  return generic_then_task(task_kind::communication,FWD(tgh),FWD(f));
}

template<class F> auto
then_comm(F&& f) {
  return make_pipeable([f0=std::move(f)](auto&& s) {
    return then_comm(FWD(s),std::move(f0));
  });
}


template<class R> auto
move_result_if_single_shot(R& r, std::true_type) -> R {
  return std::move(r);
}
template<class R> auto
move_result_if_single_shot(R& r, std::false_type) -> R& {
  return r;
}

template<task_graph_handle... tghs>
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
    kind() -> task_kind {
      return task_kind::computation;
    }

    auto
    result_ptr() -> void* {
      return &result;
    }
};


template<task_graph_handle TGH, task_graph_handle... TGHs, class... Ts> auto
join(TGH&& tgh, TGHs&&... tghs) {
  // assert all tgh.tg is the same ptr
  join_task<std::remove_cvref_t<TGH>,std::remove_cvref_t<TGHs>...> t(
    *tgh.result,*tghs.result...
  );
  using R = typename decltype(t)::result_type;
  single_shot_task_graph_handle<R> ttg;
  ttg.tg = tgh.tg;
  int n = tgh.tg->size();
  auto& emplaced_t = ttg.tg->emplace_back(std::move(t));
  ttg.result = static_cast<R*>(emplaced_t.result_ptr());

  ttg.tg->out_indices(tgh.active_node_idx).push_back(n);
  ttg.tg->in_indices(n).push_back(tgh.active_node_idx);
  ( ttg.tg->out_indices(tghs.active_node_idx).push_back(n) , ... );
  ( ttg.tg->in_indices(n).push_back(tghs.active_node_idx) , ... );

  ttg.active_node_idx = n;
  return ttg;
}



} // std_e
