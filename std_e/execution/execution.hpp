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

template<class S, class F> auto
execute(then_comm_sender<S,F>& s) -> decltype(auto) {
  return
    s.continuation()(
      execute(s.dependent_sender())
    );
}

//template<class... Ss> auto
//execute(wait_all_sender<Ss...>& s) -> decltype(auto) {
//  return std::make_tuple(
//}
template<class S0, class S1> auto
execute(wait_all_sender<S0,S1>& s) -> decltype(auto) {
  return std::make_tuple(execute(s.template dependent_sender<0>()),execute(s.template dependent_sender<1>()));
}

template<class S> auto
execute(split_sender<S>& s) -> decltype(auto) {
  if (!s.has_completed()) {
    s.execute_dep();
  }
  return s.result();
}


} // std_e
