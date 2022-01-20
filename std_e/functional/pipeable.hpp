#pragma once


#include <utility>


namespace std_e {


template<class F>
struct pipeable_wrapper : F {
  constexpr
  pipeable_wrapper(F&& f)
    : F(std::move(f))
  {}
};

template<class F> constexpr auto
make_pipeable(F&& f) {
  return pipeable_wrapper(std::move(f));
}

template<class T, class F> auto
operator|(T&& x, pipeable_wrapper<F>&& f) {
  return f(std::forward<T>(x));
}


} // std_e
