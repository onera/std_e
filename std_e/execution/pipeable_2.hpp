#pragma once


#include "std_e/execution/sender_concept.hpp"


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

template<Sender S, class F> auto
operator|(S&& s, pipeable_wrapper<F>&& f) {
  return f(FWD(s));
}


} // std_e
