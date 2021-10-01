#pragma once


#include "std_e/execution/task/concrete/input_task.hpp"
#include "std_e/execution/task/concrete/then_task.hpp"
#include "std_e/execution/task/concrete/join_task.hpp"
#include "std_e/execution/task/any_task.hpp"
#include "std_e/execution/pipeable.hpp"
#include "std_e/meta/meta.hpp"
#include <deque>
#include "std_e/graph/adjacency_graph/graph.hpp"


namespace std_e {


using task_graph_range_type = range_of_ranges<int,std::deque>;
using task_graph = io_graph</*node_type=*/any_task,/*edge_type=*/void,/*container=*/task_graph_range_type>;


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
    then_task<F,std::remove_cvref_t<decltype(*tgh.result)>,/*after_single_shot=*/true>,
    then_task<F,decltype(*tgh.result)&,/*after_single_shot=*/false>
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

inline auto
split() {
  return make_pipeable([](auto&& tgh) {
    return split(FWD(tgh));
  });
}


template<class F> auto
then(task_graph_handle auto&& tgh, F&& f) {
  return generic_then_task(task_kind::computation,FWD(tgh),FWD(f));
}

template<class F> auto
then(F&& f) {
  return make_pipeable([f0=std::move(f)](auto&& tgh) {
    return then(FWD(tgh),std::move(f0));
  });
}

template<class F> auto
then_comm(task_graph_handle auto&& tgh, F&& f) {
  return generic_then_task(task_kind::communication,FWD(tgh),FWD(f));
}

template<class F> auto
then_comm(F&& f) {
  return make_pipeable([f0=std::move(f)](auto&& tgh) {
    return then_comm(FWD(tgh),std::move(f0));
  });
}


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
