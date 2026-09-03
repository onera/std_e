#pragma once


#include "std_e/field/concepts.hpp"
#include "std_e/future/span_ref.hpp"
#include "std_e/future/dynarray.hpp"
#include "std_e/contract/contract.hpp"


namespace std_e {


template<class T> class field_view;


// --- impl
template<std::ranges::sized_range Rng>
class field_impl {
  public:
    using value_type = Rng::value_type;

    static constexpr int rank = 0;
    static constexpr int dim_tot = 1;
    static constexpr std::array<int,0> dims = {};

    constexpr explicit
    field_impl(Rng x)
      : rng(std::move(x))
    {}

    constexpr auto
    n_element() const -> int64_t {
      return int64_t(rng.size());
    }

    constexpr auto
    operator()(this auto&& self, std::integral auto i) -> decltype(auto) {
      return self.rng[i];
    }

    constexpr auto
    sub(this auto&& self, std::integral auto n) {
      auto* ptr = self.rng.data();
      STD_E_ASSERT_LVL1(n < self.n_element());
      return field_view(std_e::make_span(ptr, ptr+n));
    }

    // --- array API
    constexpr auto begin(this auto&& self) { return self.rng.begin(); }
    constexpr auto end(this auto&& self)   { return self.rng.end(); }
    constexpr auto data(this auto&& self)  { return self.rng.data(); }
    constexpr auto size(this auto&& self)  { return self.rng.size(); }
  protected:
    Rng rng;
};


// --- impl equality
template<class R0, class R1>
  requires std::equality_comparable_with<typename R0::value_type, typename R1::value_type>
constexpr bool operator==(const field_impl<R0>& x, const field_impl<R1>& y) {
  return std::ranges::equal(x, y);
}


// --- API types
template<class T, class A = mallocator>
class field : public field_impl<std_e::dynarray<T,A>> {
  public:
    using base = field_impl<std_e::dynarray<T,A>>;
    using base::base;

    constexpr
    field(std::initializer_list<T> l)
      : base(std_e::dynarray<T,A>(l))
    {}
};

template<class T>
class field_view : public field_impl<std_e::span<T>> {
  public:
    using base = field_impl<std_e::span<T>>;

    explicit
    field_view(std_e::span<T> x)
      : base(x)
    {}

    template<class R>
      requires (!std::same_as<field_view, field<R>>) // don't hijack the copy ctor
    explicit
    field_view(field<R>& x)
      : base(std_e::make_span(x))
    {}

    template<class R>
      requires (!std::same_as<field_view, field<R>>) // don't hijack the assignment op
    auto
    operator=(field<R>& x) -> field_view& {
      this->rng = std_e::make_span(x); // rebind view to `x`
      return *this;
    }
};

template<class T>
class field_ref : public field_impl<std_e::span_ref<T>> {
  public:
    using base = field_impl<std_e::span_ref<T>>;

    explicit
    field_ref(std_e::span_ref<T> x)
      : base(x)
    {}

    template<class R>
      requires (!std::same_as<field_ref, field<R>>) // don't hijack the copy ctor
    explicit
    field_ref(field<R>& x)
      : base(std_e::make_span_ref(x))
    {}

    template<class R>
      requires (!std::same_as<field_ref, field<R>>) // don't hijack the assignment op
    auto
    operator=(const field<R>& x) -> field_ref& {
      this->rng = x; // assign through (copy `x` values into `this->rng`)
      return *this;
    }
};


// --- shorthand types (e.g. for tests)
using scalar_field = field<double, mallocator>;
using scalar_field_view = field_view<double>;
using scalar_field_const_view = field_view<const double>;
using scalar_field_ref = field_ref<double>;


} // std_e
