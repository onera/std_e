#pragma once


#include "std_e/execution/task/task_graph.hpp"
#include "std_e/execution/task/task_result.hpp"


namespace std_e {

template<class R>
struct future;

template<class T> concept Future = std::remove_cvref_t<T>::enable_future; // TODO not really a concept

template<class T, class R> concept Future_of = Future<T> && std::is_convertible_v<typename std::remove_cvref_t<T>::result_type,R>; // TODO not really a concept


// A future is just a handle over a node of a task_graph
// Its primary role is to transfer information between task graph construction functions (then,then_comm,join)
//    it has
//        - a pointer to the task_graph, so that construction function can add nodes to it
//        - the id of the task the future is representing
//        - a *typed* pointer to where the result of the task will be
template<class R>
struct future; // TODO make it a class (invariant: result points to tg result)

template<class R>
struct future { // TODO make it a class (invariant: result points to tg result)
    static constexpr bool enable_future = true;
    using result_type = R;
    using result_stored_type = task_result_stored_type<R>;
  //private:
    task_graph* tg;
    result_stored_type* result; // points to tg.result (needed to keep this as a typed information)
    int active_node_idx;

  public:
    future() = default;
    future(task_graph* tg, result_stored_type* result, int active_node_idx)
      : tg(tg)
      , result(result)
      , active_node_idx(active_node_idx)
    {}
};

// Specialization for non-const ref:
//   Publicly inherit from the const ref version for implicit conversions
//   Then only "result_type" has to be overridden (contrary to the const ref version, it is not const!)
template<class R>
requires (!std::is_const_v<R>)
struct future<R&> : future<const R&> {
  using base = future<const R&>;
  using base::base;
  using result_type = R&;
};


// if a future is temporary, then it has to be single_shot (i.e. its result is used only once, hence it can be moved)
// if it is not a temporary, it is assumed multi-shot, that is, its result won't be moved by default
//template<Future Fut> constexpr bool is_single_shot = std::is_rvalue_reference_v<Fut>; // AAAAAA
template<Future Fut> constexpr bool is_single_shot = !std::is_lvalue_reference_v<Fut>;


// future_result_type {
template<Future Fut>
struct future_result_type__impl;

template<Future Fut>
requires (is_single_shot<Fut>)
struct future_result_type__impl<Fut> {
  using stored_type = typename std::decay_t<Fut>::result_stored_type;
  using        type = typename std::decay_t<Fut>::result_type;
};

template<Future Fut>
requires (!is_single_shot<Fut>)
struct future_result_type__impl<Fut> {
  using stored_type = typename std::decay_t<Fut>::result_stored_type&;
  using        type = typename std::decay_t<Fut>::result_type&;
};

template<Future Fut>
using future_stored_result_type = typename future_result_type__impl<Fut>::stored_type;

template<Future Fut>
using future_result_type        = typename future_result_type__impl<Fut>::type;
// future_result_type }


} // std_e
