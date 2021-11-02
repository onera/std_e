#pragma once


#include <utility>
#include "std_e/execution/future/future.hpp"


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

template<Future Fut, class F> auto
operator|(Fut&& fut, pipeable_wrapper<F>&& f) {
  return f(FWD(fut));
}
template<Future... Futs, class F> auto
operator|(std::tuple<Futs...>&& futs, pipeable_wrapper<F>&& f) {
  return f(FWD(futs));
}
template<Future... Futs, class F> auto
operator|(std::tuple<Futs...>& futs, pipeable_wrapper<F>&& f) {
  return f(FWD(futs));
}
template<Future... Futs, class F> auto
operator|(const std::tuple<Futs...>& futs, pipeable_wrapper<F>&& f) {
  return f(FWD(futs));
}


} // std_e
