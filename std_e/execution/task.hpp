#pragma once


#include "std_e/execution/task/concrete/input_task.hpp"
#include "std_e/execution/task/concrete/then_task.hpp"
#include "std_e/execution/task/any_task.hpp"
#include "std_e/execution/future/future.hpp"
#include "std_e/execution/future/pipeable.hpp"


namespace std_e {


template<class T> auto //requires(!std::is_lvalue_reference_v<T>) auto
input_data(task_graph& tg, T&& x) -> future<T> {
  input_task<T&&> t(FWD(x));

  int n = tg.size();
  auto& emplaced_t = tg.emplace_back(std::move(t));
  auto* typed_result = static_cast<task_result_stored_type<T>*>(emplaced_t.result_ptr());

  return {&tg,typed_result,n};
}


template<class F, Future Fut>
requires (std::invocable<F,future_result_type<Fut&&>>)
auto
generic_then_task(task_kind tk, Fut&& fut, F&& f) {
  using task_t = then_task<F,future_stored_result_type<Fut&&>>;
  task_t t(
    tk,
    FWD(f),
    *fut.result
  );
  using R = task_t::result_type;
  int last_active = fut.active_node_idx;

  auto* tg = fut.tg;
  int n = tg->size();
  auto& emplaced_t = tg->emplace_back(std::move(t)); // ... but we add the new task ...
  auto* typed_result = static_cast<task_result_stored_type<R>*>(emplaced_t.result_ptr());

  //future<R> ttg = {tg,typed_result,n};

  tg->out_indices(last_active).push_back(n); // ... and tell the previous task that this one is following ...
  tg->in_indices(n).push_back(last_active); // ... and symmetrically tell the new task that it depends one the previous one

  //return ttg;
  return future<R>{tg,typed_result,n};
}

template<class F, class Tuple, size_t... Is> auto
generic_then_task__impl(task_kind tk, Tuple&& futs, F&& f, std::index_sequence<Is...>) {
  LOG(" titi 00");
  using task_t = then_task< F, future_stored_result_type<std::tuple_element_t<Is,std::remove_cvref_t<Tuple>>>...>;
  task_t t(
    tk,
    FWD(f),
    *std::get<Is>(futs).result...
  );
  LOG(" titi 01");
  using R = task_t::result_type;

  LOG(" titi 02");
  auto* tg = std::get<0>(futs).tg; // at this point .tg is supposed to refer to the same graph for all fut in futs // TODO test

  ( LOG("Is = ",Is," , tg=",std::get<Is>(futs).tg) , ... );
  LOG(" titi 03");
  int n = tg->size();
  LOG(" titi 04");
  auto& emplaced_t = tg->emplace_back(std::move(t)); // ... but we add the new task ...
  LOG(" titi 05");
  auto* typed_result = static_cast<task_result_stored_type<R>*>(emplaced_t.result_ptr());
  LOG(" titi 06");


  LOG(" titi 07");
  ( tg->out_indices(std::get<Is>(futs).active_node_idx).push_back(n) , ... ); // ... and tell the previous task that this one is following ...
  LOG(" titi 08");
  ( tg->in_indices(n).push_back(std::get<Is>(futs).active_node_idx) , ... ); // ... and symmetrically tell the new task that it depends one the previous one
  LOG(" titi 09");

  return future<R>{tg,typed_result,n};
}

// TODO there is a risk that the user gives a std::tuple, and it is handled here and exploded
// (whereas the user does not want that)
// the solution would be to use a private tuple-like type
//    in "generic_then_task", "then_task", "then_comm_task",
//    in "pipeable" and in "join"
template<class F, Future... Futs>
requires (std::invocable<F,future_result_type<Futs&&>...>)
auto
generic_then_task(task_kind tk, std::tuple<Futs...>&& futs, F&& f) {
  constexpr int N = sizeof...(Futs);
  return generic_then_task__impl(tk,FWD(futs),FWD(f),std::make_index_sequence<N>{});
}
//template<class F, Future... Futs>
//requires (std::invocable<F,future_result_type<Futs&&>...>)
//auto
//generic_then_task(task_kind tk, std::tuple<Futs...>& futs, F&& f) {
//  constexpr int N = sizeof...(Futs);
//  return generic_then_task__impl(tk,FWD(futs),FWD(f),std::make_index_sequence<N>{});
//}
//template<class F, Future... Futs>
//requires (std::invocable<F,future_result_type<Futs&&>...>)
//auto
//generic_then_task(task_kind tk, const std::tuple<Futs...>& futs, F&& f) {
//  constexpr int N = sizeof...(Futs);
//  return generic_then_task__impl(tk,FWD(futs),FWD(f),std::make_index_sequence<N>{});
//}


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
  return generic_then_task(task_kind::computation,FWD(futs),FWD(f));
}

//template<Future... Futs, class F>
//requires (std::invocable<F,future_result_type<Futs&&>...>)
//auto
//then(std::tuple<Futs...>& futs, F&& f) {
//  return generic_then_task(task_kind::computation,FWD(futs),FWD(f));
//}
//
//template<Future... Futs, class F>
//requires (std::invocable<F,future_result_type<Futs&&>...>)
//auto
//then(const std::tuple<Futs...>& futs, F&& f) {
//  return generic_then_task(task_kind::computation,FWD(futs),FWD(f));
//}
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
  return generic_then_task(task_kind::communication,FWD(futs),FWD(f));
}

//template<Future... Futs, class F>
//requires (std::invocable<F,future_result_type<Futs&&>...>)
//auto
//then_comm(std::tuple<Futs...>& futs, F&& f) {
//  return generic_then_task(task_kind::communication,FWD(futs),FWD(f));
//}
//
//template<Future... Futs, class F>
//requires (std::invocable<F,future_result_type<Futs&&>...>)
//auto
//then_comm(const std::tuple<Futs...>& futs, F&& f) {
//  return generic_then_task(task_kind::communication,FWD(futs),FWD(f));
//}
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


template <class... Ts> constexpr auto
make_non_temp_tuple(Ts&&... xs) -> std::tuple<remove_rvalue_reference<Ts>...> {
    return {FWD(xs)...};
}

template<Future... Futs> auto
join(Futs&&... futs) {
  //( LOG("        futs =",futs.tg) , ... );
  //return forward_as_tuple(FWD(futs)...);
  return make_non_temp_tuple(FWD(futs)...);
  //return std::make_tuple(FWD(futs)...); // AAAAAA
}


} // std_e
