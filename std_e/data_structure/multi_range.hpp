#pragma once


#include "std_e/algorithm/permutation.hpp"
#include "std_e/future/contract.hpp"
#include "std_e/data_structure/heterogenous_vector.hpp"
#include "std_e/utils/macro.hpp"
#include "std_e/algorithm/permutation.hpp"
#include "std_e/future/span.hpp"


namespace std_e {


// multi_range: multiple ranges of the same size zipped together
// Invariants:
//   (1) all ranges have the same length
//   (2) if a range has been sorted, calls to find on it will use a binary search
template<template<class> class range_template, class... Ts>
class multi_range {
  public:
  // type traits
    using multi_elt_ref = std::tuple<Ts&...>;
    using multi_elt_const_ref = std::tuple<const Ts&...>;
    using index_type = std::ptrdiff_t;
    using impl_type = hrange<range_template,Ts...>;
    using value_type = std::tuple_element_t<0,std::tuple<Ts...>>; // when used in a (single-)range context

  // ctors
    multi_range() = default;

    multi_range(index_type sz)
      : multi_range(sz,std::make_index_sequence<nb_ranges()>())
    {}
    template<class Range, class... Ranges, std::enable_if_t<!std::is_integral_v<Range>,int> =0> constexpr
    // requires Ts0 == Ts
    multi_range(Range&& x, Ranges&&... xs)
      : _impl(FWD(x),FWD(xs)...)
    {}

  // size
    static constexpr auto
    nb_ranges() -> int {
      return sizeof...(Ts);
    }
    auto
    size() const -> std::ptrdiff_t {
      return get<0>(_impl).size();
    }

    auto
    sorted_range_index() const -> int {
      return sorted_rng_idx;
    }

    template<int index> auto
    range() const -> const auto& {
      return get<index>(_impl);
    }
    template<int index> auto
    range() -> auto& {
      return get<index>(_impl); // TODO return span to enforce invariant (1)
    }
    template<class T> auto
    range() const -> const auto& {
      return get<T>(_impl);
    }
    template<class T> auto
    range() -> auto& {
      return get<T>(_impl);
    }

    auto
    operator[](index_type i) const -> multi_elt_const_ref {
      return this->subscript_op__impl(i,std::make_index_sequence<nb_ranges()>());
    }
    auto
    operator[](index_type i) -> multi_elt_ref { // TODO how to enforce invariant (2) ?
      return this->subscript_op__impl(i,std::make_index_sequence<nb_ranges()>());
    }
    auto
    back() const -> multi_elt_const_ref {
      return (*this)[size()-1];
    }
    auto
    back() -> multi_elt_ref {
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

    template<int index, class T> constexpr auto
    // requires T==Ts[index]
    find_index(const T& x) const -> index_type {
      static_assert(index < nb_ranges());
      const auto& range = get<index>(_impl);
      if (index==sorted_rng_idx) {
        auto it = std::lower_bound(begin(range),end(range),x);
        return it-begin(range);
      } else {
        auto it = std::find(begin(range),end(range),x);
        return it-begin(range);
      }
    }
    template<int index, class T> constexpr auto
    // requires T==Ts[index]
    find(const T& x) const -> multi_elt_const_ref {
      static_assert(index < nb_ranges());
      index_type i = find_index<index>(x);
      STD_E_ASSERT(i<size());
      return (*this)[i];
    }
    template<int index, class T> constexpr auto
    // requires T==Ts[index]
    find(T& x) -> multi_elt_ref { // TODO how to enforce invariant (2) ?
      static_assert(index < nb_ranges());
      index_type i = find_index<index>(x);
      STD_E_ASSERT(i<size());
      return (*this)[i];
    }
    template<int search_index, int found_index, class T> constexpr auto
    // requires T==Ts[search_index]
    find_element(const T& x) const -> const auto& {
      static_assert(search_index < nb_ranges());
      static_assert(found_index < nb_ranges());
      index_type i = find_index<search_index>(x);
      STD_E_ASSERT(i<size());
      return get<found_index>(_impl)[i];
    }
    template<int search_index, int found_index, class T> constexpr auto
    // requires T==Ts[search_index]
    find_element(const T& x) -> auto& { // TODO how to enforce invariant (2) ?
      static_assert(search_index < nb_ranges());
      static_assert(found_index < nb_ranges());
      index_type i = find_index<search_index>(x);
      STD_E_ASSERT(i<size());
      return get<found_index>(_impl)[i];
    }

  // vector-like interface
    template<class... Ts0> auto
    // requires Ts0 is Ts
    push_back(const Ts0&... elts) -> multi_elt_ref { // TODO how to enforce invariant (2) ?
      return push_back__impl(std::forward_as_tuple(elts...),std::make_index_sequence<nb_ranges()>());
    }

    template<int index, class F> auto
    reorder_by(F reordering) -> void {
      auto perm = std_e::sort_permutation(get<index>(_impl));
      apply_permutation__impl(perm,std::make_index_sequence<nb_ranges()>());
      sorted_rng_idx = index;
    }

    template<int index> auto
    sort_by() -> void {
      auto perm = std_e::sort_permutation(get<index>(_impl));
      apply_permutation__impl(perm,std::make_index_sequence<nb_ranges()>());
      sorted_rng_idx = index;
    }
    auto
    sort() -> void {
      sort_by<0>();
    }
  private:
  // methods
    template<size_t... Is>
    multi_range(index_type sz, std::index_sequence<Is...>)
      : _impl(range_template<std::tuple_element_t<Is,std::tuple<Ts...>>>(sz)...)
    {}
    template<size_t... Is> constexpr auto
    // requires T==Ts[index]
    subscript_op__impl(index_type i, std::index_sequence<Is...>) const -> multi_elt_const_ref {
      return {get<Is>(_impl)[i]...};
    }
    template<size_t... Is> constexpr auto
    // requires T==Ts[index]
    subscript_op__impl(index_type i, std::index_sequence<Is...>) -> multi_elt_ref {
      return {get<Is>(_impl)[i]...};
    }
    template<class args_tuple_type, size_t... Is> auto
    // requires args_tuple_type is tuple<Ts...>
    push_back__impl(const args_tuple_type& elts, std::index_sequence<Is...>) -> multi_elt_ref {
      ( get<Is>(_impl).push_back(std::get<Is>(elts)) , ... );
      return back();
    }
    template<class Int_range, size_t... Is> auto
    apply_permutation__impl(const Int_range& perm, std::index_sequence<Is...>) -> void {
      ( std_e::permute(get<Is>(_impl),perm) , ... );
    }
  // data members
    impl_type _impl;
    int sorted_rng_idx = -1; // no range sorted by default
};


template<int j, template<class> class RT, class... Ts> auto
range(const multi_range<RT,Ts...>& x) -> const auto& {
  return x.template range<j>();
}
template<int j, template<class> class RT, class... Ts> auto
range(multi_range<RT,Ts...>& x) -> auto& {
  return x.template range<j>();
}
template<class T, template<class> class RT, class... Ts> auto
range(const multi_range<RT,Ts...>& x) -> const auto& {
  return x.template range<T>();
}
template<class T, template<class> class RT, class... Ts> auto
range(multi_range<RT,Ts...>& x) -> auto& {
  return x.template range<T>();
}

template<int j, template<class> class RT, class... Ts, class I> auto
element(const multi_range<RT,Ts...>& x, I i) -> const auto& {
  return range<j>(x)[i];
}
template<int j, template<class> class RT, class... Ts, class I> auto
element(multi_range<RT,Ts...>& x, I i) -> auto& {
  return range<j>(x)[i];
}
template<class T, template<class> class RT, class... Ts, class I> auto
element(const multi_range<RT,Ts...>& x, I i) -> const auto& {
  return range<T>(x)[i];
}
template<class T, template<class> class RT, class... Ts, class I> auto
element(multi_range<RT,Ts...>& x, I i) -> auto& {
  return range<T>(x)[i];
}

template<int index, template<class> class RT, class... Ts, class T> constexpr auto
// requires T==Ts[index]
find(const multi_range<RT,Ts...>& x, const T& value) {
  return x.template find<index>(value);
}
template<int index, template<class> class RT, class... Ts, class T> constexpr auto
// requires T==Ts[index]
find(multi_range<RT,Ts...>& x, const T& value) {
  return x.template find<index>(value);
}

template<int search_index, int found_index, template<class> class RT, class... Ts, class T> constexpr auto
// requires T==Ts[index]
find_element(const multi_range<RT,Ts...>& x, const T& value) -> const auto& {
  return x.template find_element<search_index,found_index>(value);
}
template<int search_index, int found_index, template<class> class RT, class... Ts, class T> constexpr auto
// requires T==Ts[index]
find_element(multi_range<RT,Ts...>& x, const T& value) -> auto& {
  return x.template find_element<search_index,found_index>(value);
}

template<int index, template<class> class RT, class... Ts> auto
sort_by(multi_range<RT,Ts...>& x) -> void {
  return x.template sort_by<index>();
}
template<template<class> class RT, class... Ts> auto
sort(multi_range<RT,Ts...>& x) -> void {
  return x.sort();
}

/// find when only two ranges {
template<template<class> class RT, class T0, class T1> constexpr auto
// requires T==Ts[index]
find(const multi_range<RT,T0,T1>& x, const T0& value) {
  static_assert(!std::is_same_v<T0,T1>);
  return find<0>(x,value);
}
template<template<class> class RT, class T0, class T1> constexpr auto
// requires T==Ts[index]
find(multi_range<RT,T0,T1>& x, const T0& value) {
  static_assert(!std::is_same_v<T0,T1>);
  return find<0>(x,value);
}
template<template<class> class RT, class T0, class T1> constexpr auto
// requires T==Ts[index]
find(const multi_range<RT,T0,T1>& x, const T1& value) {
  static_assert(!std::is_same_v<T0,T1>);
  return find<1>(x,value);
}
template<template<class> class RT, class T0, class T1> constexpr auto
// requires T==Ts[index]
find(multi_range<RT,T0,T1>& x, const T1& value) {
  static_assert(!std::is_same_v<T0,T1>);
  return find<1>(x,value);
}

template<template<class> class RT, class T0, class T1> constexpr auto
// requires T==Ts[index]
find_associate(const multi_range<RT,T0,T1>& x, const T0& value) -> const T1& {
  static_assert(!std::is_same_v<T0,T1>);
  return find_element<0,1>(x,value);
}
template<template<class> class RT, class T0, class T1> constexpr auto
// requires T==Ts[index]
find_associate(multi_range<RT,T0,T1>& x, const T0& value) -> T1& {
  static_assert(!std::is_same_v<T0,T1>);
  return find_element<0,1>(x,value);
}
template<template<class> class RT, class T0, class T1> constexpr auto
// requires T==Ts[index]
find_associate(const multi_range<RT,T0,T1>& x, const T1& value) -> const T0& {
  static_assert(!std::is_same_v<T0,T1>);
  return find_element<1,0>(x,value);
}
template<template<class> class RT, class T0, class T1> constexpr auto
// requires T==Ts[index]
find_associate(multi_range<RT,T0,T1>& x, const T1& value) -> T0& {
  static_assert(!std::is_same_v<T0,T1>);
  return find_element<1,0>(x,value);
}
/// find when only two ranges }

template<class... Ts> using multi_vector = multi_range<std::vector,Ts...>;

// multi_span {
template<class... Ts> using multi_span = multi_range<std_e::span,Ts...>;

template<class return_type, class multi_range_type, size_t... Is> auto
make_multi_span__impl(multi_range_type& x, std::index_sequence<Is...>) {
  return return_type(std_e::make_span(range<Is>(x))...);
}
template<template<class> class RT, class... Ts> auto
make_span(multi_range<RT,Ts...>& x) -> multi_span<Ts...> {
  using return_type = multi_span<Ts...>;
  return make_multi_span__impl<return_type>(x,std::make_index_sequence<x.nb_ranges()>());
}
template<template<class> class RT, class... Ts> auto
make_span(const multi_range<RT,Ts...>& x) -> multi_span<const Ts...> {
  using return_type = multi_span<const Ts...>;
  return make_multi_span__impl<return_type>(x,std::make_index_sequence<x.nb_ranges()>());
}

template<class return_type, class multi_range_type, class I, size_t... Is> auto
make_span_n__impl(multi_range_type& x, I start_idx, I n, std::index_sequence<Is...>) {
  return return_type(std_e::make_span_n(range<Is>(x),start_idx,n)...);
}
template<template<class> class RT, class... Ts, class I> auto
make_span_n(multi_range<RT,Ts...>& x, I start_idx, I n) -> multi_span<Ts...> {
  using return_type = multi_span<Ts...>;
  return make_span_n__impl<return_type>(x,start_idx,n,std::make_index_sequence<x.nb_ranges()>());
}
template<template<class> class RT, class... Ts, class I> auto
make_span_n(const multi_range<RT,Ts...>& x, I start_idx, I n) -> multi_span<const Ts...> {
  using return_type = multi_span<const Ts...>;
  return make_span_n__impl<return_type>(x,start_idx,n,std::make_index_sequence<x.nb_ranges()>());
}

template<class return_type, class multi_range_type, class I, size_t... Is> auto
make_span__impl(multi_range_type& x, I start_idx, I finish_idx, std::index_sequence<Is...>) {
  return return_type(std_e::make_span(range<Is>(x),start_idx,finish_idx)...);
}
template<template<class> class RT, class... Ts, class I> auto
make_span(multi_range<RT,Ts...>& x, I start_idx, I finish_idx) -> multi_span<Ts...> {
  using return_type = multi_span<Ts...>;
  return make_span_n__impl<return_type>(x,start_idx,finish_idx,std::make_index_sequence<x.nb_ranges()>());
}
template<template<class> class RT, class... Ts, class I> auto
make_span(const multi_range<RT,Ts...>& x, I start_idx, I finish_idx) -> multi_span<const Ts...> {
  using return_type = multi_span<const Ts...>;
  return make_span_n__impl<return_type>(x,start_idx,finish_idx,std::make_index_sequence<x.nb_ranges()>());
}
// multi_span }

} // std_e
