#pragma once

#include "std_e/algorithm/permutation.hpp"
#include "std_e/base/template_alias.hpp"
#include "std_e/future/contract.hpp"
#include "std_e/data_structure/heterogenous_vector.hpp"
#include "std_e/algorithm/permutation.hpp"
#include "std_e/future/span.hpp"


namespace std_e {

template<class T>
struct _mr_reference {
  using type = typename T::reference;
};
template<class T>
struct _mr_reference<const T> {
  using type = typename T::const_reference;
};

template<class T>
class multi_range2_iterator {
  public:
    using index_type = typename T::index_type;
    /// std::iterator type traits
    using value_type = typename T::value_type;
    using reference = typename _mr_reference<T>::type;
    using difference_type = index_type;
    using iterator_category = std::random_access_iterator_tag;

    multi_range2_iterator() = default;
    multi_range2_iterator(T* ptr, index_type pos)
      : ptr(ptr)
      , pos(pos)
    {}

    auto
    operator++() -> multi_range2_iterator& {
      ++pos;
      return *this;
    }
    auto
    operator--() -> multi_range2_iterator& {
      --pos;
      return *this;
    }

    auto
    operator++(int) -> multi_range2_iterator {
      multi_range2_iterator tmp(*this);
      ++*this;
      return tmp;
    }
    auto
    operator--(int) -> multi_range2_iterator {
      multi_range2_iterator tmp(*this);
      --*this;
      return tmp;

    }

    auto
    operator+=(index_type i) -> multi_range2_iterator& {
      pos += i;
      return *this;
    }
    auto
    operator-=(index_type i) -> multi_range2_iterator& {
      pos -= i;
      return *this;
    }
    friend auto
    operator+(multi_range2_iterator x, index_type i) -> multi_range2_iterator {
      multi_range2_iterator res = x;
      res += i;
      return res;
    }
    friend auto
    operator+(index_type i, multi_range2_iterator x) -> multi_range2_iterator {
      return x+i;
    }
    friend auto
    operator-(multi_range2_iterator x, index_type i) -> multi_range2_iterator {
      multi_range2_iterator res = x;
      res -= i;
      return res;
    }
    friend auto
    operator-(multi_range2_iterator x, multi_range2_iterator y) -> difference_type {
      STD_E_ASSERT(x.ptr == y.ptr);
      return x.pos - y.pos;
    }

    auto
    operator*() const -> reference {
      return (*ptr)[pos];
    }
    auto
    operator[](index_type i) const -> reference {
      return (*ptr)[pos+i];
    }

    friend constexpr auto
    operator==(const multi_range2_iterator& x, const multi_range2_iterator& y) {
      STD_E_ASSERT(x.ptr == y.ptr);
      return x.pos == y.pos;
    }
    friend constexpr auto
    operator!=(const multi_range2_iterator& x, const multi_range2_iterator& y) {
      return !(x == y);
    }
    friend constexpr auto
    operator<(const multi_range2_iterator& x, const multi_range2_iterator& y) {
      STD_E_ASSERT(x.ptr == y.ptr);
      return x.pos < y.pos;
    }
    friend constexpr auto
    operator>(const multi_range2_iterator& x, const multi_range2_iterator& y) {
      return y<x;
    }
    friend constexpr auto
    operator<=(const multi_range2_iterator& x, const multi_range2_iterator& y) {
      return !(y<x);
    }
    friend constexpr auto
    operator>=(const multi_range2_iterator& x, const multi_range2_iterator& y) {
      return !(x<y);
    }
  private:
    T* ptr;
    index_type pos;
};

} // std_e

template<class T>
struct std::iterator_traits<std_e::multi_range2_iterator<T>> {
  using type = std_e::multi_range2_iterator<T>;
  using value_type = typename type::value_type;
  using reference = typename type::reference;
  using difference_type = typename type::difference_type;
  using iterator_category = typename type::iterator_category;
};

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

    using value_type      =       std::tuple<typename Ranges::value_type...>;
    using reference       =       std::tuple<typename Ranges::value_type&...>;
    using const_reference = const std::tuple<typename Ranges::value_type&...>;
    using iterator = multi_range2_iterator<this_type>;
    using const_iterator = multi_range2_iterator<const this_type>;

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
      return std::get<0>(_impl).size();
    }

    template<int index> auto
    range() -> auto& {
      return std::get<index>(_impl); // TODO return span to enforce invariant (1)
    }
    template<int index> auto
    range() const -> const auto& {
      return std::get<index>(_impl);
    }
    //template<class T> auto
    //range() -> auto& {
    //  return std::get<T>(_impl);
    //}
    //template<class T> auto
    //range() const -> const auto& {
    //  return std::get<T>(_impl);
    //}

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
      return this->subscript_op__impl(i,std::make_index_sequence<n_range()>());
    }
    auto
    operator[](index_type i) const -> const_reference {
      return this->subscript_op__impl(i,std::make_index_sequence<n_range()>());
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
    subscript_op__impl(index_type i, std::index_sequence<Is...>) const -> const_reference {
      return {get<Is>(_impl)[i]...};
    }
    template<size_t... Is> constexpr auto
    // requires T==Ts[index]
    subscript_op__impl(index_type i, std::index_sequence<Is...>) -> reference {
      return {get<Is>(_impl)[i]...};
    }
    template<class args_tuple_type, size_t... Is> auto
    // requires args_tuple_type is tuple<Ts...>
    push_back__impl(const args_tuple_type& elts, std::index_sequence<Is...>) -> reference {
      ( get<Is>(_impl).push_back(std::get<Is>(elts)) , ... );
      return back();
    }
  // data members
    impl_type _impl;
};


template<int j, class... Rngs> auto
range(const multi_range2<Rngs...>& x) -> const auto& {
  return x.template range<j>();
}
template<int j, class... Rngs> auto
range(multi_range2<Rngs...>& x) -> auto& {
  return x.template range<j>();
}
//template<class T, class... Rngs> auto
//range(const multi_range2<Rngs...>& x) -> const auto& {
//  return x.template range<T>();
//}
//template<class T, class... Rngs> auto
//range(multi_range2<Rngs...>& x) -> auto& {
//  return x.template range<T>();
//}

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
