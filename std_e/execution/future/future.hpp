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
struct future;


template<class R>
class future<const R&> { // TODO make it a class (invariant: result points to tg result)
  public:
    static constexpr bool enable_future = true;
    using result_type = const R&;
    using result_stored_type = task_result_stored_type<const R&>;

  public:
    future() = default;
    // TODO Class invariant:
    //     node at position i_node in tg
    //     is a task for which result_ptr() is indeed of type result_stored_type*
    future(task_graph* tg, int i_node, result_stored_type* res)
      : tg(tg)
      , i_node(i_node)
      , res(res)
    {}

    auto
    graph() const -> task_graph* {
      return tg;
    }
    auto
    task_id() const -> int {
      return i_node;
    }

    auto
    result() const -> result_stored_type* {
      // TODO
      //return static_cast<result_stored_type*>(task().result_ptr());
      return res;
    }
  protected:
     auto task() -> any_task& { // TODO const
       return tg->node(i_node);
     }
  private:
    task_graph* tg;
    int i_node;
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
class future : public future<const R&> { // TODO make it a class (invariant: result points to tg result)
    static_assert(!std::is_reference_v<R>); // if is_reference, should have matched another partial specialization
  public:
    using base = future<const R&>;
    static constexpr bool enable_future = true;
    using result_type = R;
    using result_stored_type = R;
    using result_stored_ref_type = base::result_stored_type;

    future() = default;
    future(task_graph* tg, int i_node, R* res)
      : base(tg,i_node,static_cast<result_stored_ref_type*>(tg->node(i_node).result_ptr_ptr()))
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
    result() -> result_stored_type* { // TODO const
      //return task().result_ptr();
      return static_cast<result_stored_type*>(this->task().result_ptr());
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
