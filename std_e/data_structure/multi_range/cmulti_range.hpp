#pragma once

#include "std_e/algorithm/permutation.hpp"
#include "std_e/base/template_alias.hpp"
#include "std_e/future/contract.hpp"
#include "std_e/data_structure/heterogenous_vector.hpp"
#include "std_e/algorithm/permutation.hpp"
#include "std_e/future/span.hpp"
#include "std_e/future/array_ref.hpp"
#include "std_e/future/tuple.hpp"
#include "std_e/future/type_traits.hpp"
#include "std_e/data_structure/multi_range/multi_range_iterator.hpp"
#include "std_e/meta/find.hpp"
#include "std_e/data_structure/multi_range/multi_range.hpp"


namespace std_e {

// cmulti_range: contiguous multi_range
// This is a variant of multi_range where all ranges are actually contiguous in memory
// This constraint forces that all the ranges are of the same type (contrary to multi_range)
// Note:
//    in principle, this is a 2D array where the first dimension is fixed,
//    we could use e.g. multi_array to provide the functionality
//    however we feel the interface is slighly different
//      -> here, there is a sort of asymmetry between directions (first fixed, second dyn), not present in a multi-array
//    plus, debugging/error messages are simpler
template<std::ranges::random_access_range Range, size_t N>
class cmulti_range {
  public:
  // type traits
    static constexpr bool is_cmulti_range = true;
    using this_type = cmulti_range<Range,N>;
    using index_type = std::ptrdiff_t;

    using scalar_value_type = Range::value_type;
    using T = scalar_value_type;
    using value_type      = std::array<T,N>;
    using reference       = std_e::array_ref<T,N>;
    using const_reference = std_e::array_ref<const T,N>;
    using iterator = multi_range_iterator<this_type>;
    using const_iterator = multi_range_iterator<const this_type>;

    static constexpr auto n_range = N;

  // ctors
    cmulti_range() = default;

    cmulti_range(index_type sz)
      : cmulti_range(Range(sz*N))
    {}
    template<std::ranges::random_access_range Rng> constexpr
    cmulti_range(Rng&& x)
      : _impl(FWD(x))
    {
      STD_E_ASSERT_LVL0(_impl.size() % N == 0);
    }

    template<size_t NN>
    cmulti_range(const T(&a)[NN])
      : _impl(a,a+NN)
    {}

  // size
    auto
    size() const -> std::ptrdiff_t {
      return _impl.size()/N;
    }

    // get individual ranges
    /// get a view
    template<size_t I> auto
    range_view(this auto&& self) {
      return make_span(self._impl.data() + I*self.size(), self.size());
    }
    /// get a ref
    template<size_t I> auto
    range_ref(this auto&& self) {
      return make_span_ref(self._impl.data() + I*self.size(), self.size());
    }

    auto
    multi_range_view(this auto&& self) {
      return self._multi_range_view(std::make_index_sequence<n_range>());
    }
 
    // only provided for structured binding (see `get(cmulti_range)`)
    template<size_t I> auto
    _get_lvalue_ref(this auto& self) {
      return self.template range_ref<I>();
    }

    auto begin(this auto&& self) { return multi_range_iterator{&self, 0          }; }
    auto end  (this auto&& self) { return multi_range_iterator{&self, self.size()}; }
    auto
    operator[](this auto&& self, index_type i) {
      return self._subscript_op(i, std::make_index_sequence<n_range>());
    }

    auto
    operator[](this auto& self, index_type i, index_type j) -> auto& {
      return *(self._impl.data() + i*self.size() + j);
    }
    auto
    operator()(this auto& self, index_type i, index_type j) -> auto& { // TODO deprecate
      return *(self._impl.data() + i*self.size() + j);
    }

    auto
    back(this auto& self) {
      return self[self.size()-1];
    }

  // low-level access
    constexpr auto
    underlying(this auto&& self) -> auto& {
      return self._impl;
    }

    constexpr auto
    data(this auto& self) -> auto* {
      return self._impl.data();
    }

    auto
    flat_view(this auto&& self) {
      return make_span(self.data(), self.size()*N);
    }

  // vector-like interface TODO del (would require to shift)
    //template<class... Ts0> auto
    //  requires (sizeof...(Ts0) == N)
    //push_back(const Ts0&... elts) -> reference {
    //  return _push_back(std::forward_as_tuple(elts...), std::make_index_sequence<n_range>());
    //}

  private:
  // methods
    template<size_t... Is> constexpr auto
    // requires T==Ts[index]
    _subscript_op(index_type i, std::index_sequence<Is...>) const -> const_reference {
      return const_reference(*(_impl.data() + Is*size() + i)...);
    }
    template<size_t... Is> constexpr auto
    // requires T==Ts[index]
    _subscript_op(index_type i, std::index_sequence<Is...>) -> reference {
      return reference(*(_impl.data() + Is*size() + i)...);
    }
    //template<class args_tuple_type, size_t... Is> auto
    //// requires args_tuple_type is tuple<Ts...>
    //_push_back(const args_tuple_type& elts, std::index_sequence<Is...>) -> reference {
    //  ( get<Is>(_impl).push_back(get<Is>(elts)) , ... );
    //  return back();
    //}
    template<size_t... Is> constexpr auto
    _multi_range_view(this auto& self, std::index_sequence<Is...>) {
      return multi_span(self.template range_view<Is>()...);
    }
  // data members
    Range _impl;
};

template<size_t N, std::ranges::random_access_range Range> auto
make_cmulti_range(Range&& x) {
  return cmulti_range<std::remove_cvref_t<Range>,N>(FWD(x));
}

// aliases
template<class T, size_t N> using cmulti_vector = cmulti_range<std::vector<T>,N>;
template<class T, size_t N> using cmulti_span   = cmulti_range<span<T>,N>;


// range_view, get, proj
template<size_t I, class MR>
  requires (std::remove_cvref_t<MR>::is_cmulti_range)
auto
range_view(MR&& mr) {
  return FWD(mr).template range_view<I>();
}

template<size_t I, class MR>
  requires (std::remove_cvref_t<MR>::is_cmulti_range)
auto
range_ref(MR&& mr) {
  return FWD(mr).template range_ref<I>();
}

// get==range_view (get is the required name for structured bindings) 
template<size_t I, class MR>
  requires (std::remove_cvref_t<MR>::is_cmulti_range)
auto
get(MR& mr) {
  return mr.template _get_lvalue_ref<I>();
}

// begin/end
template<class MR>
  requires (MR::is_cmulti_range)
auto
begin(MR& mr) {
  return mr.begin();
}
template<class MR>
  requires (MR::is_cmulti_range)
auto
end(MR& mr) {
  return mr.end();
}

// Same as range_view/get function, but this one is a lambda
// Useful for expressions such as `std_e::ranges::sort(my_multi_range, proj<0>)`
template<int i> constexpr auto crange_proj = [](const auto& x) -> decltype(auto) { return get<i>(x); };


// element
template<int j, class MR, class I = MR::index_type>
  requires (MR::is_cmulti_range)
auto
element(MR& x, I i) -> auto& {
  return range_view<j>(x)[i];
}
template<class T, class MR, class I = MR::index_type>
  requires (MR::is_cmulti_range)
auto
element(MR& x, I i) -> auto& {
  return range_view<T>(x)[i];
}


// views
template<class MR>
  requires (std::remove_cvref_t<MR>::is_cmulti_range)
  // TODO allow rvalue ref only if span
auto
view(MR&& x) {
  return make_cmulti_range<std::remove_cvref_t<MR>::n_range>(make_span(x.underlying()));
}

template<class MR, class I = MR::index_type>
  requires (std::remove_cvref_t<MR>::is_cmulti_range)
  // TODO allow rvalue ref only if span
auto
subview_n(MR&& x, I i, I n) {
  return subview_n(x.multi_range_view(), i, n);
}

template<class MR, class I = MR::index_type>
  requires (std::remove_cvref_t<MR>::is_cmulti_range)
  // TODO allow rvalue ref only if span
auto
subview(MR&& x, I i, I j) {
  return make_span(x.multi_range_view(), i, j);
}

} // std_e


// tuple protocol
//   in particular this is useful for structured bindings
namespace std {
  template<class Range, size_t N>
  struct tuple_size<std_e::cmulti_range<Range,N>>
    : integral_constant<size_t, N>
  {};

  template<size_t I, class Range, size_t N>
  struct tuple_element<I, std_e::cmulti_range<Range,N>> {
    using T = Range::value_type;
    using type = std_e::span_ref<T>;
  };
}
