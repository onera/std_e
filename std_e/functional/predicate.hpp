#pragma once


#include "std_e/base/macros.hpp"


namespace std_e {


template<class F>
class Predicate {
  private:
    F f;
  public:
    static constexpr bool enable_predicate = true;
    Predicate(F f)
      : f(std::move(f))
    {}

    auto
    operator()(const auto& x) {
      return f(x);
    }
};

template<class F>
class Not_predicate {
  private:
    F f;
  public:
    static constexpr bool enable_predicate = true;
    Not_predicate(F f)
      : f(std::move(f))
    {}

    auto
    operator()(const auto& x) {
      return !f(x);
    }
};

template<class F0, class F1>
class And_predicate {
  private:
    F0 f0;
    F1 f1;
  public:
    static constexpr bool enable_predicate = true;
    And_predicate(F0 f0, F1 f1)
      : f0(std::move(f0))
      , f1(std::move(f1))
    {}

    auto
    operator()(const auto& x) {
      return f0(x) && f1(x);
    }
};


template<class F0, class F1>
class Or_predicate {
  private:
    F0 f0;
    F1 f1;
  public:
    static constexpr bool enable_predicate = true;
    Or_predicate(F0 f0, F1 f1)
      : f0(std::move(f0))
      , f1(std::move(f1))
    {}

    auto
    operator()(const auto& x) {
      return f0(x) || f1(x);
    }
};


template<class P>
  requires std::decay_t<P>::enable_predicate
auto
operator!(P&& p) {
  return Not_predicate<std::decay_t<P>>(FWD(p));
}

template<class P0, class P1>
  requires std::decay_t<P0>::enable_predicate && std::decay_t<P1>::enable_predicate
auto
operator&&(P0&& p0, P1&& p1) {
  return And_predicate<std::decay_t<P0>,std::decay_t<P1>>(FWD(p0), FWD(p1));
}

template<class P0, class P1>
  requires std::decay_t<P0>::enable_predicate && std::decay_t<P1>::enable_predicate
auto
operator||(P0&& p0, P1&& p1) {
  return Or_predicate<std::decay_t<P0>,std::decay_t<P1>>(FWD(p0), FWD(p1));
}


} // std_e
