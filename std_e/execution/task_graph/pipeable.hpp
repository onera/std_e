#pragma once


#include <utility>
#include "std_e/execution/task_graph/task_graph_handle.hpp"


namespace std_e {


template<class F>
struct pipeable_wrapper : F {
  pipeable_wrapper(F&& f)
    : F(std::move(f))
  {}
};

template<class F> auto
make_pipeable(F&& f) {
  return pipeable_wrapper(std::move(f));
}

template<Task_graph_handle TGH, class F> auto
operator|(TGH&& tgh, pipeable_wrapper<F>&& f) {
  return f(FWD(tgh));
}


} // std_e
