#pragma once

#include <tuple>
#include "std_e/base/macros.hpp"


namespace std_e {


template<class... Ts>
class hierarchical_zip {
  public:
  // traits
    using impl_type = std::tuple<Ts...>;
  // ctor
    template<class... Ts0>
    hierarchical_zip(Ts0&&... xs)
      : _impl{FWD(xs)...}
    {}

  // low-level access
    constexpr auto
    impl() -> impl_type& {
      return _impl;
    }
    constexpr auto
    impl() const -> const impl_type& {
      return _impl;
    }

  private:
    impl_type _impl;
};
template<class T, class... Ts> constexpr auto
get(hierarchical_zip<Ts...>& x) -> T& {
  return std::get<T>(x.impl());
}
template<class T, class... Ts> constexpr auto
get(const hierarchical_zip<Ts...>& x) -> const T& {
  return std::get<T>(x.impl());
}
template<size_t I, class... Ts> constexpr auto
get(hierarchical_zip<Ts...>& x) -> auto& {
  return std::get<I>(x.impl());
}
template<size_t I, class... Ts> constexpr auto
get(const hierarchical_zip<Ts...>& x) -> const auto& {
  return std::get<I>(x.impl());
}


template<class... Ts> constexpr auto
hierarchical_zip_view(Ts&... xs) {
  return hierarchical_zip<Ts&...>(xs...);
}
template<class... Ts> constexpr auto
hierarchical_zip_const_view(const Ts&... xs) {
  return hierarchical_zip<const Ts&...>(xs...);
}


template<class... Ts, class Projection> constexpr auto
zip_projection(hierarchical_zip<Ts...>& hzip, Projection proj) {
  return hierarchical_zip_view( proj(get<Ts>(hzip.impl())) ... ); // TODO get<Ts> -> get<I> (here, if several times same type: WRONG behavior)
}


} // std_e
