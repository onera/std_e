#pragma once


#include <type_traits>


namespace std_e {


// If the result of a task is a reference, we can't store it directly
//     because we only get the result when the task is executed,
//     whereas the result location is created before execution
// The solution is rather to store a pointer,
//     and assign it the result once the task is executed
// To desambiguate from the task really returning a pointer,
//     we wrap it in a task_ref_result_wrapper
template<class R>
//requires (std::is_reference_v<R>)
struct task_ref_result_wrapper {
  static_assert(std::is_reference_v<R>);

  std::remove_reference_t<R>* ptr;

  task_ref_result_wrapper()
    : ptr(nullptr)
  {}
  task_ref_result_wrapper(R r)
    : ptr(&r)
  {}
};


template<class R>
using task_result_stored_type = std::conditional_t<
  std::is_reference_v<R>,
  task_ref_result_wrapper<std::remove_cvref_t<R>&>, // remove_cvref_t: Treat everything non-const for storing
                                                    //   non-const to const conversions are needed (driven by the user function signatures)
                                                    //   but then it implies to convert task_ref_result_wrapper<R&>*
                                                    //   to task_ref_result_wrapper<const R&>*
                                                    //   and although it is semantically safe, it is UB in the language
                                                    // note that const-correctness is preserved in the API (or should be!)
  R
>;

template<class task_result> auto
unwrap_task_result(task_result&& r) -> decltype(auto) {
  return FWD(r);
}
template<class R> auto
unwrap_task_result(task_ref_result_wrapper<R&> r) -> R& {
  return *r.ptr;
}


} // std_e
