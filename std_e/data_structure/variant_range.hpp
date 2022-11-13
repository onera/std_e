#pragma once


#include <variant>


namespace std_e {


template<typename T, typename Tuple>
  struct tuple_count;

template<typename T, typename Tuple>
  inline constexpr size_t tuple_count_v =
    tuple_count<T, Tuple>::value;

template<typename T, typename... Ts>
  struct tuple_count<T, std::tuple<Ts...>>
  : std::integral_constant<size_t, 0> {};

template<typename T, typename _First, typename... _Rest>
  struct tuple_count<T, std::tuple<_First, _Rest...>>
  : std::integral_constant< size_t, tuple_count_v<T, std::tuple<_Rest...>> + std::is_same_v<T, _First>> {};

template<typename T, class... Ts>
  inline constexpr bool exactly_once =
    tuple_count_v<T, std::tuple<Ts...>> == 1;


// reference_variant {
template<class... Ts>
class reference_variant {
  private:
    std::variant<Ts*...> impl;
  public:
    template<class T>
    reference_variant(T& x)
      : impl(&x)
    {
      static_assert(exactly_once<T,Ts...>);
    }

    template<class T> auto
    // requires Ts is not const
    operator=(const T& x) -> reference_variant& {
      static_assert(exactly_once<T,Ts...>); // ensures non-const versions
      *std::get<T*>(impl) = x;
      return *this;
    }

    template<class T> auto
    get_impl() & -> auto& {
      if constexpr (exactly_once<T,Ts...>) {
        return *std::get<T*>(impl);
      } else {
        static_assert(exactly_once<const T,Ts...>);
        return *std::get<const T*>(impl);
      }
    }
    template<class T> auto
    get_impl() && -> auto& {
      if constexpr (exactly_once<T,Ts...>) {
        return *std::get<T*>(impl);
      } else {
        static_assert(exactly_once<const T,Ts...>);
        return *std::get<const T*>(impl);
      }
    }
    template<class T> auto
    get_impl() const -> const T& {
      if constexpr (exactly_once<T,Ts...>) {
        return *std::get<T*>(impl);
      } else {
        static_assert(exactly_once<const T,Ts...>);
        return *std::get<const T*>(impl);
      }
    }
    template<class T> auto
    get_impl() const&& -> const T&& {
      if constexpr (exactly_once<T,Ts...>) {
        return *std::get<T*>(impl);
      } else {
        static_assert(exactly_once<const T,Ts...>);
        return *std::get<const T*>(impl);
      }
    }

    template<class T, class... Ts0> friend auto
    holds_alternative(const reference_variant<Ts0...>& x) -> bool;

    template<class F, class... Ts0> friend auto
    visit(F&& f, reference_variant<Ts0...>& x) -> decltype(auto);
    template<class F, class... Ts0> friend auto
    visit(F&& f, const reference_variant<Ts0...>& x) -> decltype(auto);
};

template<class T, class... Ts> auto
get(reference_variant<Ts...>& x) -> auto& {
  return x.template get_impl<T>();
}
template<class T, class... Ts> auto
get(reference_variant<Ts...>&& x) -> auto& {
  return std::move(x).template get_impl<T>();
}
template<class T, class... Ts> auto
get(const reference_variant<Ts...>& x) -> const T& {
  return x.template get_impl<T>();
}
template<class T, class... Ts> auto
get(const reference_variant<Ts...>&& x) -> const T& {
  return std::move(x).template get_impl<T>();
}

template<class T, class... Ts0> auto
holds_alternative(const reference_variant<Ts0...>& x) -> bool {
  if constexpr (exactly_once<T,Ts0...>) {
    return std::holds_alternative<T*>(x.impl);
  } else {
    static_assert(exactly_once<const T,Ts0...>);
    return std::holds_alternative<const T*>(x.impl);
  }
}

template<class F, class... Ts> auto
visit(F&& f, reference_variant<Ts...>& x) -> decltype(auto) {
  return visit([f](auto&& y){ return f(*y); },x.impl);
}
template<class F, class... Ts> auto
visit(F&& f, const reference_variant<Ts...>& x) -> decltype(auto) {
  return visit([f](auto&& y){ return f(*y); },x.impl);
}
// reference_variant }


// variant_range {
template<template<class...> class Range, template<class...> class Reference_variant, class... Ts>
// requires Ts types are unique
// requires Ts types are regular (in partic., not refs)
class variant_range {
  private:
  // types
    using range_variant = std::variant<Range<Ts>...>;
    using ref_variant = Reference_variant<Ts...>;
    using const_ref_variant = Reference_variant<const Ts...>;
  // data member
    range_variant impl;
  public:
  // type traits
    using value_type = std::variant<Ts...>;
    using reference = ref_variant;
    using const_reference = const_ref_variant;
    using pointer = void*;
    using const_pointer = const void*;
  // ctors
    constexpr
    variant_range() = default;

    template<class T> constexpr
    // requires T is one of the Ts
    variant_range(Range<T> x)
      : impl(std::move(x))
    {}

  // range interface
    constexpr auto
    size() const -> size_t {
      return this->visit([](const auto& x){ return x.size(); });
    }
    constexpr auto
    operator[](ptrdiff_t i) const -> const_ref_variant {
      return this->visit([i](const auto& x){ return const_ref_variant(x[i]); });
    }
    constexpr auto
    operator[](ptrdiff_t i) -> ref_variant {
      return this->visit([i](auto& x){ return ref_variant(x[i]); });
    }

  // low-level
    constexpr auto
    data() -> void* {
      return this->visit([](auto& x)->void*{ return x.data(); });
    }
    constexpr auto
    data() const -> const void* {
      return this->visit([](auto& x)->const void*{ return x.data(); });
    }

    //template<class T> constexpr auto
    //get() -> Range<T>& {
    //  return std::get<Range<T>>(impl);
    //}
    //template<class T> constexpr auto
    //get() const -> const Range<T>& {
    //  return std::get<Range<T>>(impl);
    //}

    template<class T, template<class> class R, template<class...> class RV, class... Ts0> friend auto
    get(variant_range<R,RV,Ts0...>& x) -> R<T>&;
    template<class T, template<class> class R, template<class...> class RV, class... Ts0> friend auto
    get(const variant_range<R,RV,Ts0...>& x) -> const R<T>&;

    // TODO remove
    constexpr auto
    underlying_variant() -> range_variant& {
      return impl;
    }

    template<class F> constexpr auto
    visit(F&& f) -> decltype(auto) {
      return std::visit(FWD(f),impl);
    }
    template<class F> constexpr auto
    visit(F&& f) const -> decltype(auto) {
      return std::visit(FWD(f),impl);
    }

    #if __cplusplus > 201703L
      constexpr auto
      operator<=>(const variant_range& x) const = default;
    #else
      friend constexpr auto
      operator==(const variant_range& x, const variant_range& y) {
        return x.underlying_variant() == y.underlying_variant();
      }
      friend constexpr auto
      operator!=(const variant_range& x, const variant_range& y) {
        return !(x==y);
      }
      friend constexpr auto
      operator<(const variant_range& x, const variant_range& y) {
        return x.underlying_variant() < y.underlying_variant();
      }
    #endif

    template<class T, template<class...> class R, template<class...> class RV, class... Ts0> friend auto
    holds_alternative(const variant_range<R,RV,Ts0...>& x) -> bool;
};
// variant_range }

template<class T, template<class...> class R, template<class...> class RV, class... Ts0> auto
get(variant_range<R,RV,Ts0...>& x) -> R<T>& {
  return std::get<R<T>>(x.impl);
}
template<class T, template<class...> class R, template<class...> class RV, class... Ts0> auto
get(const variant_range<R,RV,Ts0...>& x) -> const R<T>& {
  return std::get<R<T>>(x.impl);
}

template<template<class...> class R, template<class...> class RV, class... Ts, class F> auto
visit(F&& f, variant_range<R,RV,Ts...>& x) -> decltype(auto) {
  return x.visit(FWD(f));
}
template<template<class...> class R, template<class...> class RV, class... Ts, class F> auto
visit(F&& f, const variant_range<R,RV,Ts...>& x) -> decltype(auto) {
  return x.visit(FWD(f));
}

template<class T, template<class...> class R, template<class...> class RV, class... Ts> auto
data_as(variant_range<R,RV,Ts...>& x) -> T* {
  return get<T>(x).data();
}
template<class T, template<class...> class R, template<class...> class RV, class... Ts> auto
data_as(const variant_range<R,RV,Ts...>& x) -> const T* {
  return get<T>(x).data();
}
template<class T, template<class...> class R, template<class...> class RV, class... Ts> auto
holds_alternative(const variant_range<R,RV,Ts...>& x) -> bool {
  return std::holds_alternative<R<T>>(x.impl);
}


} // std_e
