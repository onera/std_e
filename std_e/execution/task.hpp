#pragma once


#include "std_e/execution/any_task.hpp"
#include "std_e/execution/pipeable.hpp"
#include "std_e/meta/meta.hpp"
#include <deque>
#include "std_e/graph/adjacency_graph/graph.hpp"


namespace std_e {


enum class task_kind {
  computation, communication
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
      return task_kind::computation; // TODO
    }

    auto
    result_ptr() -> void* {
      return &result;
    }
};

template<class F, class Arg>
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
      result = f(std::move(arg));
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

template<class R>
struct task_graph_handle { // TODO make class (invariant: result points to tg result
  task_graph* tg;
  R* result; // points to tg.result (needed to keep this as a typed information)
  int active_node_idx;
};

template<class T> requires(!std::is_lvalue_reference_v<T>) auto
input_data(task_graph& tg, T&& x) {
  input_task t(std::move(x));
  int n = tg.size();
  task_graph_handle<T> ttg;
  ttg.tg = &tg;
  auto& emplaced_t = ttg.tg->emplace_back(std::move(t));
  ttg.result = static_cast<T*>(emplaced_t.result_ptr());
  ttg.active_node_idx = n;
  return ttg;
}


template<class T, class U> auto
move_if_non_lvalue_ref(U&& x) -> auto&& {
  //if constexpr (std::is_lvalue_reference_v<T>) {
  //  return FWD(x);
  //} else {
  //  return std::move(x);
  //}
  return std::move(x);
}


template<class R, class F> auto
generic_then_task(task_kind tk, task_graph_handle<R>&& tgh, F f) {
  task t(
    tk,
    std::move(f),
    //move_if_non_lvalue_ref<R>(*tgh.result) // TODO std::forward<R>(...) should do the right thing
    *tgh.result // TODO std::forward<R>(...) should do the right thing
  );
  using R0 = decltype(t)::result_type;
  task_graph_handle<R0> ttg;
  int n = tgh.tg->size();
  int last_active = tgh.active_node_idx;
  ttg.tg = tgh.tg; // the new graph is the old one...
  auto& emplaced_t = ttg.tg->emplace_back(std::move(t)); // ... but we add the new task ...
  ttg.tg->out_indices(last_active).push_back(n); // ... and tell the previous task that this one is following ...
  ttg.tg->in_indices(n).push_back(last_active); // ... and symmetrically tell the new task that it depends one the previous one
  ttg.result = static_cast<R0*>(emplaced_t.result_ptr());
  ttg.active_node_idx = n; // the active task is the one we just added
  return ttg;
}


template<class T, class F> auto
then(T&& s, F&& f) {
  return generic_then_task(task_kind::computation,FWD(s),FWD(f));
}

template<class F> auto
then(F&& f) {
  return make_pipeable([f0=std::move(f)](auto&& s) {
    return then(FWD(s),std::move(f0));
  });
}

template<class T, class F> auto
then_comm(T&& s, F&& f) {
  return generic_then_task(task_kind::communication,FWD(s),FWD(f));
}

template<class F> auto
then_comm(F&& f) {
  return make_pipeable([f0=std::move(f)](auto&& s) {
    return then_comm(FWD(s),std::move(f0));
  });
}


//template<class T, class... Ts> auto
//join(task_graph_handle<T>&& tgh, task_graph_handle<Ts>&&... tghs) {
//  task t(
//    task_kind::computation,
//    [](auto& args_tuple) -> auto& { return args_tuple; }, // just transfer the value
//    std::forward_as_tuple(std::move(tgh).result,std::move(tghs).result...)
//  );
//  task_graph_handle<T&> ttg;
//  int n = tgh.tg->size();
//  ttg.tg->nodes().push_back(std::move(t));
//  ttg.result = ttg.tg->back().result_ptr();
//
//  ttg.tg->out_indices(tgh.active_node_idx).push_back(n);
//  ttg.tg->in_indices(n).push_back(tgh.active_node_idx);
//  ( ttg.tg->out_indices(tghs.active_node_idx).push_back(n) , ... );
//  ( ttg.tg->in_indices(n).push_back(tghs.active_node_idx) , ... );
//
//  ttg.active_node_idx = n;
//  return ttg;
//}



} // std_e
