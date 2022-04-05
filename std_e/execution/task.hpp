#pragma once


#include "std_e/execution/task/concrete/input_task.hpp"
#include "std_e/execution/task/concrete/then_task.hpp"
#include "std_e/execution/task/any_task.hpp"
#include "std_e/execution/future/future.hpp"
#include "std_e/functional/pipeable.hpp"


namespace std_e {


template<class T> auto //requires(!std::is_lvalue_reference_v<T>) auto
input_data(task_graph& tg, T&& x) -> future<T> {
  input_task<T&&> t(FWD(x));

  int n = tg.size();
  tg.emplace_back(std::move(t));

  return {&tg,n};
}


template<class F, Future Fut>
requires (std::invocable<F,future_result_type<Fut&&>>)
auto
generic_then_task(task_kind tk, Fut&& fut, F&& f) {
  using task_t = then_task<F,future_stored_result_type<Fut&&>>;
  task_t t(
    tk,
    FWD(f),
    *fut.result()
  );
  int last_active = fut.task_id();

  auto* tg = fut.graph();
  int n = tg->size();
  tg->emplace_back(std::move(t)); // Add the new task to the graph...

  tg->out_indices(last_active).push_back(n); // ... tell the previous task that this one is following ...
  tg->in_indices(n).push_back(last_active); // ... and symmetrically tell the new task that it depends one the previous one

  using R = typename task_t::result_type;
  return future<R>{tg,n};
}

template<class F, class Tuple, size_t... Is> auto
generic_then_task__impl(task_kind tk, Tuple&& futs, F&& f, std::index_sequence<Is...>) {
  auto* tg = std::get<0>(futs).graph();
  ( STD_E_ASSERT(std::get<Is>(futs).graph() == tg) , ... ); // Dependencies are supposed to be built on the same graph!

  using task_t = then_task< F, future_stored_result_type<std::tuple_element_t<Is,std::remove_cvref_t<Tuple>>>...>;
  task_t t(
    tk,
    FWD(f),
    *std::get<Is>(futs).result()...
  );

  int n = tg->size();
  tg->emplace_back(std::move(t)); // Add the new task to the graph...

  ( tg->out_indices(std::get<Is>(futs).task_id()).push_back(n) , ... ); // ... tell the previous tasks that this one is following ...
  ( tg->in_indices(n).push_back(std::get<Is>(futs).task_id()) , ... ); // ... and symmetrically tell the new task that it depends one these previous ones

  using R = typename task_t::result_type;
  return future<R>{tg,n};
}

// TODO there is a risk that the user gives a std::tuple, and it is handled here and exploded
// (whereas the user does not want that)
// the solution would be to use a private tuple-like type
template<class F, Future... Futs>
requires (std::invocable<F,future_result_type<Futs&&>...>)
auto
generic_then_task(task_kind tk, std::tuple<Futs...>&& futs, F&& f) {
  constexpr int N = sizeof...(Futs);
  return generic_then_task__impl(tk,std::move(futs),FWD(f),std::make_index_sequence<N>{});
}


// then {
template<Future Fut, class F>
requires (std::invocable<F,future_result_type<Fut&&>>)
auto
then(Fut&& fut, F&& f) {
  return generic_then_task(task_kind::computation,FWD(fut),FWD(f));
}

template<Future... Futs, class F>
requires (std::invocable<F,future_result_type<Futs&&>...>)
auto
then(std::tuple<Futs...>&& futs, F&& f) {
  return generic_then_task(task_kind::computation,std::move(futs),FWD(f));
}
// then }

// then_comm {
template<Future Fut, class F>
requires (std::invocable<F,future_result_type<Fut&&>>)
auto
then_comm(Fut&& fut, F&& f) {
  return generic_then_task(task_kind::communication,FWD(fut),FWD(f));
}

template<Future... Futs, class F>
requires (std::invocable<F,future_result_type<Futs&&>...>)
auto
then_comm(std::tuple<Futs...>&& futs, F&& f) {
  return generic_then_task(task_kind::communication,std::move(futs),FWD(f));
}
// then_comm }


// make pipeable {
template<class F> auto
then(F&& f) {
  return make_pipeable([f0=std::move(f)](auto&& fut) {
    return then(FWD(fut),std::move(f0));
  });
}
template<class F> auto
then_comm(F&& f) {
  return make_pipeable([f0=std::move(f)](auto&& fut) {
    return then_comm(FWD(fut),std::move(f0));
  });
}
// make pipeable }

template<class F, class T> auto
then_comm2(F&& f, T&& capture_arg) {
  return then_comm([f0=std::move(f),arg=std::move(capture_arg)](auto&& x){ return f0(FWD(x),arg); });
}

template<Future... Futs> auto
join(Futs&&... futs) {
  return make_non_temp_tuple(FWD(futs)...);
}


} // std_e
