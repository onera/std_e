#pragma once


#include "std_e/base/macros.hpp"


namespace std_e {


template<class... Ts>
class tuple_impl;

template<>
class tuple_impl<> {
  public:
    constexpr
    tuple_impl() = default;

    constexpr auto
    operator<=>(const tuple_impl<>& x) const = default;
};

template<class T, class... Ts>
class tuple_impl<T,Ts...> {
  private:
    [[no_unique_address]] T head;
    [[no_unique_address]] tuple_impl<Ts...> tail;
  public:
    constexpr
    tuple_impl() = default;

    template<class T0, class... Ts0>
      requires (!std::is_same_v<T0,tuple_impl<T,Ts...>>)
        constexpr
    tuple_impl(T0&& x, Ts0&&... xs)
      : head(FWD(x))
      , tail(FWD(xs)...)
    {}

    constexpr auto
    operator<=>(const tuple_impl<T,Ts...>& x) const = default;


    template<size_t I>
        constexpr auto
    get() -> auto& {
      if constexpr (I == 0) {
        return head;
      } else {
        return tail.template get<I-1>();
      }
    }
    template<size_t I>
        constexpr auto
    get() const -> const auto& {
      if constexpr (I == 0) {
        return head;
      } else {
        return tail.template get<I-1>();
      }
    }
};

template<class... Ts>
class tuple {
  private:
    tuple_impl<Ts...> impl;
  public:
    constexpr
    tuple() = default;

    template<class T0, class... Ts0>
      requires (!std::is_same_v<T0,tuple<Ts...>>)
        constexpr
    tuple(T0&& x, Ts0&&... xs)
      : impl(FWD(x),FWD(xs)...)
    {}

    constexpr auto
    operator<=>(const tuple<Ts...>& x) const = default;

    template<size_t I> constexpr auto get()       ->       auto& { return impl.template get<I>(); }
    template<size_t I> constexpr auto get() const -> const auto& { return impl.template get<I>(); }
};


template<size_t I, class... Ts> constexpr auto
get(tuple<Ts...>& t) -> auto& {
  return t.template get<I>();
}
template<size_t I, class... Ts> constexpr auto
get(const tuple<Ts...>& t) -> const auto& {
  return t.template get<I>();
}


} // std_e


template<class... Ts>
struct std::tuple_size<std_e::tuple<Ts...>> {
  static constexpr size_t value = sizeof...(Ts);
};
