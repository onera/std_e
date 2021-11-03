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


// TODO del (inline where used)
struct task_in_graph {
  task_graph* tg;
  int i_node;
};

template<class R>
class future<const R&> { // TODO make it a class (invariant: result points to tg result)
  public:
    static constexpr bool enable_future = true;
    using result_type = const R&;
    using result_stored_type = task_result_stored_type<const R&>;

  public:
    future() = default;
    future(task_graph* tg, int i_node, result_stored_type* res)
      : t(tg,i_node)
      , res(res)
    {}

    auto
    graph() const -> task_graph* {
      return t.tg;
    }
    auto
    task_id() const -> int {
      return t.i_node;
    }

    auto
    result() const -> result_stored_type* {
      //any_task& t0 = t.tg->node(t.i_node);
      //return static_cast<result_stored_type*>(t0.result_ptr());
      return res;
    }
  private:
    task_in_graph t;
    result_stored_type* res; // class invariant: points to tg.result // TODO DEL
};

// Specialization for non-const ref:
//   Publicly inherit from the const ref version for implicit conversions
//   Then only "result_type" has to be overridden (contrary to the const ref version, it is not const!)
template<class R>
requires (!std::is_const_v<R>) // TODO DEL once not needed anymore
class future<R&> : public future<const R&> {
  public:
    using base = future<const R&>;
    using base::base;
    using result_type = R&;
};

template<class R>
class future : private task_ref_result_wrapper<R&>, private future<const R&> { // TODO make it a class (invariant: result points to tg result)
//class future : public task_in_graph { // TODO make it a class (invariant: result points to tg result)
    static_assert(!std::is_reference_v<R>); // if is_reference, should have matched another partial specialization
  public:
    using base = future<const R&>;
    static constexpr bool enable_future = true;
    using result_type = R;
    using result_stored_type = R;
  //private:
    //R* res; // points to tg.result (needed to keep this as a typed information)

  public:
    future() = default;
    future(task_graph* tg, int i_node, R* res)
      : task_ref_result_wrapper<R&>(*res)
      , base(tg,i_node,as_task_ref_wrap())
    {}

    auto
    graph() const -> task_graph* {
      return base::graph();
    }
    auto
    task_id() const -> int {
      return base::task_id();
    }

    auto
    as_task_ref_wrap() -> task_ref_result_wrapper<R&>* {
      return this;
    }
    auto
    result() const -> result_stored_type* {
      return task_ref_result_wrapper<R&>::ptr;
    }
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
