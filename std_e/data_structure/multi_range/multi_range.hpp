#pragma once

#include "std_e/algorithm/permutation.hpp"
#include "std_e/base/template_alias.hpp"
#include "std_e/future/contract.hpp"
#include "std_e/data_structure/heterogenous_vector.hpp"
#include "std_e/algorithm/permutation.hpp"
#include "std_e/future/span.hpp"
#include "std_e/future/tuple.hpp"
#include "std_e/future/type_traits.hpp"
#include "std_e/data_structure/multi_range/multi_range_iterator.hpp"


namespace std_e {

// multi_range2: multiple ranges of the same size zipped together
// Invariant: all ranges have the same length
template<class... Ranges>
class multi_range2 {
  public:
  // type traits
    using this_type = multi_range2<Ranges...>;
    using index_type = std::ptrdiff_t;
    using impl_type = std::tuple<Ranges...>;

    using value_type      = std_e::tuple<typename std_e::remove_cvref_t<Ranges>::value_type...>;
    using reference       = std_e::tuple<typename std_e::remove_cvref_t<Ranges>::reference ...>;
    using const_reference = std_e::tuple<typename std_e::remove_cvref_t<Ranges>::const_reference ...>;
    using iterator = multi_range_iterator<this_type>;
    using const_iterator = multi_range_iterator<const this_type>;

    static constexpr auto
    n_range() -> int {
      return sizeof...(Ranges);
    }

  // ctors
    multi_range2() = default;

    multi_range2(index_type sz)
      : multi_range2(Ranges(sz)...)
    {}
    template<class Range_0, class... Ranges_0, std::enable_if_t<!std::is_integral_v<Range_0>,int> =0> constexpr
    // requires Ts0 == Ts
    multi_range2(Range_0&& x, Ranges_0&&... xs)
      : _impl(FWD(x),FWD(xs)...)
    {}

  // size
    auto
    size() const -> std::ptrdiff_t {
      return get<0>(_impl).size();
    }

    template<int index> auto
    range() & -> auto& {
      return get<index>(_impl); // TODO return span to enforce invariant (1)
    }
    template<int index> auto
    range() const& -> const auto& {
      return get<index>(_impl);
    }
    template<int index> auto
    range() && -> auto&& {
      return get<index>(std::move(_impl));
    }
    template<int index> auto
    range() const&& -> const auto&& {
      return get<index>(std::move(_impl));
    }

    auto
    begin() -> iterator {
      return {this,0};
    }
    auto
    begin() const -> const_iterator {
      return {this,0};
    }
    auto
    end() -> iterator {
      return {this,size()};
    }
    auto
    end() const -> const_iterator {
      return {this,size()};
    }
    auto
    operator[](index_type i) -> reference { // TODO how to enforce invariant (2) ?
      return this->subscript_op_impl(i,std::make_index_sequence<n_range()>());
    }
    auto
    operator[](index_type i) const -> const_reference {
      return this->subscript_op_impl(i,std::make_index_sequence<n_range()>());
    }
    auto
    back() -> reference {
      return (*this)[size()-1];
    }
    auto
    back() const -> const_reference {
      return (*this)[size()-1];
    }

  // low-level access
    constexpr auto
    impl() -> impl_type& {
      return _impl;
    }
    constexpr auto
    impl() const -> const impl_type& {
      return _impl;
    }

  // vector-like interface
    template<class... Ts0> auto
    // requires Ts0 is Ts
    push_back(const Ts0&... elts) -> reference { // TODO how to enforce invariant (2) ?
      return push_back__impl(std::forward_as_tuple(elts...),std::make_index_sequence<n_range()>());
    }

  private:
  // methods
    template<size_t... Is> constexpr auto
    // requires T==Ts[index]
    subscript_op_impl(index_type i, std::index_sequence<Is...>) const -> const_reference {
      return const_reference(get<Is>(_impl)[i]...);
    }
    template<size_t... Is> constexpr auto
    // requires T==Ts[index]
    subscript_op_impl(index_type i, std::index_sequence<Is...>) -> reference {
      return reference(get<Is>(_impl)[i]...);
    }
    template<class args_tuple_type, size_t... Is> auto
    // requires args_tuple_type is tuple<Ts...>
    push_back__impl(const args_tuple_type& elts, std::index_sequence<Is...>) -> reference {
      ( get<Is>(_impl).push_back(get<Is>(elts)) , ... );
      return back();
    }
  // data members
    impl_type _impl;
};

// deduction guides // TODO use variadic
template<class R0>                               multi_range2(R0&&               ) -> multi_range2<R0>;
template<class R0, class R1>                     multi_range2(R0&&,R1&&          ) -> multi_range2<R0,R1>;
template<class R0, class R1, class R2>           multi_range2(R0&&,R1&&,R2&&     ) -> multi_range2<R0,R1,R2>;
template<class R0, class R1, class R2, class R3> multi_range2(R0&&,R1&&,R2&&,R3&&) -> multi_range2<R0,R1,R2,R3>;


template<int j, class... Rngs> auto
range(multi_range2<Rngs...>& x) -> auto& {
  return x.template range<j>();
}
template<int j, class... Rngs> auto
range(const multi_range2<Rngs...>& x) -> const auto& {
  return x.template range<j>();
}
template<int j, class... Rngs> auto
range(multi_range2<Rngs...>&& x) -> auto&& {
  return std::move(x).template range<j>();
}
template<int j, class... Rngs> auto
range(const multi_range2<Rngs...>&& x) -> const auto&& {
  return std::move(x).template range<j>();
}

// get (for structured bindings) {
template<int j, class... Rngs> auto
get(multi_range2<Rngs...>& x) -> auto& {
  return x.template range<j>();
}
template<int j, class... Rngs> auto
get(const multi_range2<Rngs...>& x) -> const auto& {
  return x.template range<j>();
}
template<int j, class... Rngs> auto
get(multi_range2<Rngs...>&& x) -> auto&& {
  return std::move(x).template range<j>();
}
template<int j, class... Rngs> auto
get(const multi_range2<Rngs...>&& x) -> const auto&& {
  return std::move(x).template range<j>();
}
// get (for structured bindings) }

template<int j, class... Rngs, class I> auto
element(const multi_range2<Rngs...>& x, I i) -> const auto& {
  return range<j>(x)[i];
}
template<int j, class... Rngs, class I> auto
element(multi_range2<Rngs...>& x, I i) -> auto& {
  return range<j>(x)[i];
}
template<class T, class... Rngs, class I> auto
element(const multi_range2<Rngs...>& x, I i) -> const auto& {
  return range<T>(x)[i];
}
template<class T, class... Rngs, class I> auto
element(multi_range2<Rngs...>& x, I i) -> auto& {
  return range<T>(x)[i];
}


template<class... Ts> using multi_vector2 = multi_range2<std::vector<Ts>...>;

template<class... Rngs> auto
view_as_multi_range(Rngs&... rngs) {
  return multi_range2<Rngs&...>(rngs...);
}
// TODO mixed view/container
template<class... Ts> auto
view_as_multi_range2(span<Ts>... rngs) { // TODO rename without 2 (here, just tmp to be sure this one is called)
  // specialization keep the span by value because it might be ephemeral
  // while the underlying values are not
  return multi_range2<span<Ts>...>(rngs...);
}

//// multi_span {
//template<class... Ts> using multi_span2 = multi_range2<dyn_span,Ts...>;
//
//template<class return_type, class multi_range_type, size_t... Is> auto
//make_multi_span__impl(multi_range_type& x, std::index_sequence<Is...>) {
//  return return_type(std_e::make_span(range<Is>(x))...);
//}
//template<template<class> class RT, class... Ts> auto
//make_span(multi_range2<RT,Ts...>& x) -> multi_span<Ts...> {
//  using return_type = multi_span<Ts...>;
//  return make_multi_span__impl<return_type>(x,std::make_index_sequence<multi_range2<RT,Ts...>::n_range()>());
//}
//template<template<class> class RT, class... Ts> auto
//make_span(const multi_range2<RT,Ts...>& x) -> multi_span<const Ts...> {
//  using return_type = multi_span<const Ts...>;
//  return make_multi_span__impl<return_type>(x,std::make_index_sequence<multi_range2<RT,Ts...>::n_range()>());
//}
//
//template<class return_type, class multi_range_type, class I, size_t... Is> auto
//make_span_n__impl(multi_range_type& x, I start_idx, I n, std::index_sequence<Is...>) {
//  return return_type(std_e::make_span_n(range<Is>(x),start_idx,n)...);
//}
//template<template<class> class RT, class... Ts, class I> auto
//make_span_n(multi_range2<RT,Ts...>& x, I start_idx, I n) -> multi_span<Ts...> {
//  using return_type = multi_span<Ts...>;
//  return make_span_n__impl<return_type>(x,start_idx,n,std::make_index_sequence<multi_range2<RT,Ts...>::n_range()>());
//}
//template<template<class> class RT, class... Ts, class I> auto
//make_span_n(const multi_range2<RT,Ts...>& x, I start_idx, I n) -> multi_span<const Ts...> {
//  using return_type = multi_span<const Ts...>;
//  return make_span_n__impl<return_type>(x,start_idx,n,std::make_index_sequence<multi_range2<RT,Ts...>::n_range()>());
//}
//
//template<class return_type, class multi_range_type, class I, size_t... Is> auto
//make_span__impl(multi_range_type& x, I start_idx, I finish_idx, std::index_sequence<Is...>) {
//  return return_type(std_e::make_span(range<Is>(x),start_idx,finish_idx)...);
//}
//template<template<class> class RT, class... Ts, class I> auto
//make_span(multi_range2<RT,Ts...>& x, I start_idx, I finish_idx) -> multi_span<Ts...> {
//  using return_type = multi_span<Ts...>;
//  return make_span_n__impl<return_type>(x,start_idx,finish_idx,std::make_index_sequence<multi_range2<RT,Ts...>::n_range()>());
//}
//template<template<class> class RT, class... Ts, class I> auto
//make_span(const multi_range2<RT,Ts...>& x, I start_idx, I finish_idx) -> multi_span<const Ts...> {
//  using return_type = multi_span<const Ts...>;
//  return make_span_n__impl<return_type>(x,start_idx,finish_idx,std::make_index_sequence<multi_range2<RT,Ts...>::n_range()>());
//}
//// multi_span }

} // std_e


// tuple protocol
//   in particular this is useful for structured bindings
namespace std {
  template<class... Ranges>
  struct tuple_size<std_e::multi_range2<Ranges...>>
    : integral_constant<size_t, sizeof...(Ranges)>
  {};

  template<size_t Index, class... Ranges>
  struct tuple_element<Index, std_e::multi_range2<Ranges...>>
    : tuple_element<Index, tuple<Ranges...>>
  {};
}
