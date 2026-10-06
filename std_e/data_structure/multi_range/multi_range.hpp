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
#include "std_e/meta/find.hpp"


namespace std_e {

// multi_range2: multiple ranges of the same size zipped together
// Invariant: all ranges have the same length
template<std::ranges::random_access_range... Ranges>
class multi_range2 {
  public:
  // type traits
    static constexpr bool is_multi_range = true;
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
    template<std::ranges::random_access_range... Rngs> constexpr
    multi_range2(Rngs&&... xs)
      : _impl(FWD(xs)...)
    {}

  // size
    auto
    size() const -> std::ptrdiff_t {
      return get<0>(_impl).size();
    }

    template<size_t I> auto
    range(this auto&& self) -> auto&& {
      return get<I>(FWD(self)._impl); // TODO return span to enforce invariant (1)
    }
    template<class T> auto
    range(this auto&& self) -> auto&& {
      constexpr size_t I = meta::find_rng_idx<T, Ranges...>;
      return get<I>(FWD(self)._impl); // TODO return span to enforce invariant (1)
    }

    auto begin(this auto& self) -> iterator { return multi_range_iterator{&self, 0          }; }
    auto end  (this auto& self) -> iterator { return multi_range_iterator{&self, self.size()}; }
    auto
    operator[](this auto& self, index_type i) {
      return self.subscript_op_impl(i,std::make_index_sequence<n_range()>());
    }
    auto
    back(this auto& self) {
      return self[self.size()-1];
    }

  // low-level access
    constexpr auto
    underlying_tuple(this auto&& self) -> auto& {
      return self._impl;
    }

  // vector-like interface
    template<class... Ts0> auto
    // requires Ts0 is Ts
    push_back(const Ts0&... elts) -> reference { // TODO how to enforce invariant (2) ?
      return push_back_impl(std::forward_as_tuple(elts...),std::make_index_sequence<n_range()>());
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
    push_back_impl(const args_tuple_type& elts, std::index_sequence<Is...>) -> reference {
      ( get<Is>(_impl).push_back(get<Is>(elts)) , ... );
      return back();
    }
  // data members
    impl_type _impl;
};

// deduction guides
template<class... R0> multi_range2(R0&&...) -> multi_range2<std::decay_t<R0>...>;

// aliases
template<class... Ts> using multi_vector2 = multi_range2<std::vector<Ts>...>;
template<class... Ts> using multi_span2 = multi_range2<span<Ts>...>;


// range, get, proj
template<size_t I, class MR>
  requires (std::remove_cvref_t<MR>::is_multi_range)
auto
range(MR&& mr) -> auto&& {
  return FWD(mr).template range<I>();
}
template<class T, class MR>
  requires (std::remove_cvref_t<MR>::is_multi_range)
auto
range(MR&& mr) -> auto&& {
  return FWD(mr).template range<T>();
}

// get==range (get is the required name for structured bindings) 
template<size_t I, class MR>
  requires (std::remove_cvref_t<MR>::is_multi_range)
auto
get(MR&& mr) -> auto&& {
  return FWD(mr).template range<I>();
}

// begin/end
template<class MR>
  requires (MR::is_multi_range)
auto
begin(MR& mr) {
  return mr.begin();
}
template<class MR>
  requires (MR::is_multi_range)
auto
end(MR& mr) {
  return mr.end();
}

// Same as range/get function, but this one is a lambda
// Useful for expressions such as `std_e::ranges::sort(my_multi_range, proj<0>)`
template<int i> constexpr auto proj = [](const auto& x) -> decltype(auto) { return get<i>(x); };


// element
template<int j, class MR, class I = MR::index_type>
  requires (MR::is_multi_range)
auto
element(MR& x, I i) -> auto& {
  return range<j>(x)[i];
}
template<class T, class MR, class I = MR::index_type>
  requires (MR::is_multi_range)
auto
element(MR& x, I i) -> auto& {
  return range<T>(x)[i];
}


// zip
template<std::ranges::random_access_range... Rngs> auto
zip(Rngs&&... rngs) {
  return multi_range2<std::decay_t<Rngs>...>(rngs...);
}

template<std::ranges::random_access_range... Rngs> auto
zip_view(Rngs&... rngs) {
  return multi_range2(make_span(rngs)...);
}


// make_span
template<class MR>
  requires (MR::is_multi_range)
auto
make_span(MR& x) {
  auto make_multi_span_from_args = [](auto&&... args){ return multi_range2(make_span(args)...); };
  return std::apply(make_multi_span_from_args, x.underlying_tuple());
}

template<class MR, class I = MR::index_type>
  requires (MR::is_multi_range)
auto
make_span_n(MR& x, I i, I n) {
  auto make_multi_span_from_args = [i,n](auto&&... args){ return multi_range2(make_span_n(args,i,n)...); };
  return std::apply(make_multi_span_from_args, x.underlying_tuple());
}

template<class MR, class I = MR::index_type>
  requires (MR::is_multi_range)
auto
make_span(MR& x, I i, I j) {
  auto make_multi_span_from_args = [i,j](auto&&... args){ return multi_range2(make_span_n(args,i,j)...); };
  return std::apply(make_multi_span_from_args, x.underlying_tuple());
}

} // std_e


// tuple protocol
//   in particular this is useful for structured bindings
namespace std {
  template<class... Ranges>
  struct tuple_size<std_e::multi_range2<Ranges...>>
    : integral_constant<size_t, sizeof...(Ranges)>
  {};

  template<size_t I, class... Ranges>
  struct tuple_element<I, std_e::multi_range2<Ranges...>>
    : tuple_element<I, tuple<Ranges...>>
  {};
}
