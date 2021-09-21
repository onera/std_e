#pragma once


#include "std_e/execution/sender.hpp"


namespace std_e {


template<class T> auto
execute(input_sender<T>& s) -> T&& {
  return s.retrieve_data();
}

template<class S, class F> auto
execute(then_sender<S,F>& s) -> decltype(auto) {
  return
    s.continuation()(
      execute(s.dependent_sender())
    );
}


} // std_e
