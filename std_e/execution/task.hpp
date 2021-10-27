#pragma once


#include "std_e/execution/task/concrete/input_task.hpp"
#include "std_e/execution/task/concrete/then_task.hpp"
#include "std_e/execution/task/concrete/join_task.hpp"
#include "std_e/execution/task/any_task.hpp"
#include "std_e/execution/task_graph/task_graph_handle.hpp"
#include "std_e/execution/task_graph/pipeable.hpp"


namespace std_e {


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

//template<class T> requires(!std::is_lvalue_reference_v<T>) auto
//input_data(T&& x) {
//  return make_pipeable([x0=std::move(x)](task_graph& tg) {
//    return input_data(tg,std::move(x0));
//  });
//}

template<Task_graph_handle TGH>
struct task_graph_handle_result_type__impl;

template<Task_graph_handle TGH>
requires (is_single_shot<TGH>)
struct task_graph_handle_result_type__impl<TGH> {
  using type = std::remove_cvref_t<decltype(*std::declval<TGH>().result)>;
};

template<Task_graph_handle TGH>
requires (!is_single_shot<TGH>)
struct task_graph_handle_result_type__impl<TGH> {
  using type = decltype(*std::declval<TGH>().result)&;
};

template<Task_graph_handle TGH>
using task_graph_handle_result_type = typename task_graph_handle_result_type__impl<TGH>::type;


template<class F, Task_graph_handle TGH> auto
generic_then_task(task_kind tk, TGH&& tgh, F&& f) {
  using task_t = then_task<is_single_shot<TGH&&>,F,task_graph_handle_result_type<TGH&&>>;
  task_t t(
    tk,
    FWD(f),
    *tgh.result
  );
  using R = decltype(t)::result_type;
  task_graph_handle<R> ttg;
  int n = tgh.tg->size();
  ttg.tg = tgh.tg; // the new graph is the old one...
  auto& emplaced_t = ttg.tg->emplace_back(std::move(t)); // ... but we add the new task ...
  ttg.result = static_cast<R*>(emplaced_t.result_ptr());
  ttg.active_node_idx = n; // the active task is the one we just added

  int last_active = tgh.active_node_idx;
  ttg.tg->out_indices(last_active).push_back(n); // ... and tell the previous task that this one is following ...
  ttg.tg->in_indices(n).push_back(last_active); // ... and symmetrically tell the new task that it depends one the previous one

  return ttg;
}

//template<class F, class Tuple, size_t... Is> auto
//generic_then_task__impl(task_kind tk, Tuple&& tghs, F&& f, std::index_sequence<Is...>) {
//  using task_t = then_task<is_single_shot<TGH>,F,task_graph_handle_result_type<TGH>>;
//  task_t t(
//    tk,
//    FWD(f),
//    *std::get<Is>(tghs>.result)...
//  );
//  using R = decltype(t)::result_type;
//  task_graph_handle<R> ttg;
//  auto* tg = std::get<0>(tghs).tg; // at this point .tg is supposed to refer to the same graph for all tgh in tghs
//  int n = tg->size();
//  ttg.tg = tg; // the new graph is the old one...
//  auto& emplaced_t = ttg.tg->emplace_back(std::move(t)); // ... but we add the new task ...
//  ttg.result = static_cast<R*>(emplaced_t.result_ptr());
//  ttg.active_node_idx = n; // the active task is the one we just added
//
//  int last_active = tgh.active_node_idx;
//  ttg.tg->out_indices(std::get<0>(tghs).active_node_idx).push_back(n)...; // ... and tell the previous task that this one is following ...
//  ttg.tg->in_indices(n).push_back(std::get<0>(tghs).active_node_idx)...; // ... and symmetrically tell the new task that it depends one the previous one
//
//  return ttg;
//}
//
//template<class F, Task_graph_handle... TGHs> auto
//generic_then_task(task_kind tk, std::tuple<TGHs...>&& tghs, F&& f) {
//  constexpr int N = sizeof...(TGHs);
//  return generic_then_task__impl(tk,FWD(tghs),FWD(f),std::make_index_sequence<N>{});
//}
//template<class F, Task_graph_handle... TGHs> auto
//generic_then_task(task_kind tk, std::tuple<TGHs...>& tghs, F&& f) {
//  constexpr int N = sizeof...(TGHs);
//  return generic_then_task__impl(tk,FWD(tghs),FWD(f),std::make_index_sequence<N>{});
//}
//template<class F, Task_graph_handle... TGHs> auto
//generic_then_task(task_kind tk, const std::tuple<TGHs...>& tghs, F&& f) {
//  constexpr int N = sizeof...(TGHs);
//  return generic_then_task__impl(tk,FWD(tghs),FWD(f),std::make_index_sequence<N>{});
//}


template<class F> auto
then(Task_graph_handle auto&& tgh, F&& f) {
  return generic_then_task(task_kind::computation,FWD(tgh),FWD(f));
}

template<class F> auto
then(F&& f) {
  return make_pipeable([f0=std::move(f)](auto&& tgh) {
    return then(FWD(tgh),std::move(f0));
  });
}

template<Task_graph_handle TGH, class F> auto
then_comm(TGH&& tgh, F&& f) {
  return generic_then_task(task_kind::communication,FWD(tgh),FWD(f));
}

template<class F> auto
then_comm(F&& f) {
  return make_pipeable([f0=std::move(f)](auto&& tgh) {
    return then_comm(FWD(tgh),std::move(f0));
  });
}

template<class F, class T> auto
then_comm(F&& f, T&& capture_arg) {
  return then_comm([f0=std::move(f),arg=std::move(capture_arg)](auto&& x){ return f0(FWD(x),arg); });
}


template<Task_graph_handle TGH, Task_graph_handle... TGHs, class... Ts> auto
join(TGH&& tgh, TGHs&&... tghs) {
  // assert all tgh.tg is the same ptr
  join_task<TGH&&,TGHs&&...> t(
    *tgh.result,*tghs.result...
  );
  using R = typename decltype(t)::result_type;
  task_graph_handle<R> ttg;
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
