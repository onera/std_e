#pragma once


#include "std_e/execution/sender_concept.hpp"
#include "std_e/execution/execution.hpp"


namespace std_e {


namespace detail {

template<class R>
struct psender_internal_base {
  virtual auto execute() const -> R = 0;

  virtual ~psender_internal_base() {}
};

template<sender S, class R = decltype(execute(std::declval<S>()))>
struct psender_internal_impl: psender_internal_base<R> {
  public:
    psender_internal_impl(S s)
      : s(std::move(s))
    {}

    auto execute() const -> R {
      return execute(s);
    }
  private:
    S s;
};

} // detail


template<class R>
struct null_sender {
  auto execute() const -> { return R{}; }
};

template<class R>
class psender {
  public:
    psender()
      : psender(null_sender<R>{})
    {}

    template<sender S>
    psender(S&& x):
      impl(std::make_unique<detail::psender_internal_impl<std::decay_t<S>>>(std::forward<S>(x)))
    {}

    auto execute() const -> decltype(auto) {
      return impl->execute();
    }
  private:
    std::unique_ptr<detail::psender_internal_base<R>> impl;
};


template<class R> auto
execute(psender<R>& s) -> decltype(auto) {
  return s.execute();
}


} // std_e
