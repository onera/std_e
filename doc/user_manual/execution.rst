.. _execution:

************************
Execution [Experimental]
************************


.. _task_graph:

Tasks and task graphs
=====================

A `Then_task` represents a unit of execution that can be executed at some point with no need for extra information. More precisely, it is a data structure holding a function, its arguments, and the result for those arguments. More precisely, it holds:
- A function `F`
- References to arguments `Args&...`
- A location where a `result` will be stored
All these attributes are typed (no type erasure at this point).

The goal is to create dependency graphs of tasks to let a scheduler execute task asynchronously (e.g. by executing concurrently a computation heavy task and a ressource/memory io-bound task). Hence we want to populate our task graph with `Then_task` and to wire them so that one task can be executed once its dependencies have.

We begin our graph by tasks that are ready to be started. These are special kinds of tasks, called `Input_tasks`, that only have a `result`, already ready, (and no function nor arguments). Then we want to attach `Then_tasks` as depending tasks. Once a task `t0` have been created, we can take a reference to its `result` and take it has an argument for a `Then_task` `t1`. This way, even if the `result` of `t0` is not computed yet, `t1` has a reference to **were it will** be once `t0` has completed. So `t1` will be able to be launched with no further data once the scheduler knows that `t0` has completed.

[Technical note: it means that the graph where the tasks are stored needs to guarantee reference stability (i.e. references to its task nodes are not invalidated by addition or deletions of other nodes). This is currently done be using a `deque` container]

Since `Tasks` (either `Then_tasks` or `Input_tasks`) are typed, it is impratical to store them as-is in a graph. So they are type-erased behind `any_task`. `any_task` provides three methods:
 * `execute() -> void` to execute the task
    * for a `Then_task` it will call the function with the arguments and store the result in `result`.
    * for an `Input_task` it does nothing
 * `kind() const -> task_kind` gives a hint to the scheduler about the kind of task (computation-heavy, communication, does nothing...)
 * `result_ptr() -> void*` returns a pointer to the result

These type-erased tasks are stored in a `task_graph`.

Tasks are impratical to be used directly to build a `task_graph`. They are seen through `Futures` by the user.


Future
======

[Note: while we use the term `Future` here because it is short and expressive, beware that it has not the same samantics and use as `std::future`]

A `Future` is just an API temporary object that is used to create dependency graphs in a type-safe, ressource safe manner. It represents a lightweight view of a task in a dependency graph. Internally, a future holds:
 * a pointer to its `task_graph`
 * the id of the task node it represents in the `task_graph`
 * a **typed** pointer to the result of the task it reprensents


A `Future` can be created by:
 * Futurising allready available data, through `input_data(task_graph& tg, T&& data) -> Future<T>`
    * behind the scenes, it attaches an `Input_task` to the `task_graph`
 * Creating a continuation from one or multiple other futures, with e.g. `then(Future<T> fut, F f) -> Future<R>`
    * behind the scenes, it
        * attaches a `Then_task` `t1` to the `task_graph`
        * adds a dependency relation of `t1` to `t0`, the task associated to `fut`
        * Puts the `result` of `t0` as the reference argument of `t1`

Syntax `then(fut,f)` is equivalent to `fut | then(f)`. The latter syntax is similar to Unix pipes, and enables clearer, left-to-right chaining, e.g. `fut | then(f0) | then(f1)`

`then_comm` is the same as `then`, except that the resulting task is supposed to be a communication task (light computation, mainly waiting for ressources).

`join` packs together multiple `Futures`, so that a multi-argument function can be attached to them, e.g. `join(Future<T0> fut0, Future<T1> fut1) | then(f)` where `f` is suppose to have a signature like `f(T0,T1)`.

A future can be single-shot, i.e. attached to only one continuation; or multi-shot, i.e. attached multiple times, so that execution can be *forked*.


Values, references, move
------------------------

The property of being single or multi-shot is not a property of a `Future` type, but rather if it is passed has an lvalue (multi-shot) or an rvalue (single-shot). Everything can be implemented considering only multi-shot `Futures`. However, knowing that a `Future` is single-shot means that its result can be **moved** safely to its only continuation, since there can't be another continuation to use it.

A `Future` can have two kinds of `result_type`:
 * a value `R`
 * an lvalue reference `R&`

If the future `fut0` is multi-shot, i.e. is passed by lvalue reference to `then` or `join`, then its continuations `futs...` will be passed `fut0.result` by lvalue reference.

If the future `fut0` is single-shot, i.e. is passed by rvalue reference to `then` or `join`, then its continuation `fut1` will either:
  * be passed `fut0.result` by lvalue reference if it is an lvalue reference (note: similar to T& && = T&)
  * be passed `fut0.result` by rvalue reference (i.e. by **move**) if it is a value (since it must be its only continuation, might as well treat the value as non-reusable later)

Conversions
-----------

`future<R >[&|&&] -> future<const R&>` // Good, but in the && case, note that the lifetime is not extended as in the language

`future<R >[&] -> future<R&>` // Good but will imply...
`future<R >&& -> future<R&>` // ...this, that should not be possible (analoguous to the fact that R&& -> R& is forbidden). Note that important (?? may work in this context)
`future<R&>[&|&&] -> future<const R&>`

Lifetime
--------

The result of future is guaranteed to be kept alive until its dependencies are computed (since they need it). If it is needed further (because its data is still referenced by further continuations), then it should be also attached to further futures
