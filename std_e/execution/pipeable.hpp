#pragma once


#include <utility>


namespace std_e {

// TODO move
template<class T> concept task_graph_handle = std::remove_cvref_t<T>::enable_task_graph_handle; // TODO not really a concept

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

template<task_graph_handle TGH, class F> auto
operator|(TGH&& tgh, pipeable_wrapper<F>&& f) {
  return f(FWD(tgh));
}


} // std_e
