#pragma once


#include "std_e/field/concepts.hpp"
#include "std_e/future/span_ref.hpp"
#include "std_e/future/dynarray.hpp"
#include "std_e/contract/contract.hpp"


namespace std_e {


template<class T> class field_view;


// --- impl
class field_impl {
  public:
    static constexpr int rank = 0;
    static constexpr int dim_tot = 1;
    static constexpr std::array<int,0> dims = {};

    constexpr auto
    n_element(this auto&& self) -> int64_t {
      return int64_t(self.rng.size());
    }

    constexpr auto
    operator()(this auto&& self, std::integral auto i) -> decltype(auto) {
      return self.rng[i];
    }

    constexpr auto
    sub(this auto&& self, std::integral auto n) {
      auto* ptr = self.rng.data();
      STD_E_ASSERT_LVL1(n < self.n_element());
      return field_view(ptr, ptr+n);
    }

    // --- array API
    constexpr auto begin(this auto&& self) { return self.rng.begin(); }
    constexpr auto end(this auto&& self)   { return self.rng.end(); }
    constexpr auto data(this auto&& self)  { return self.rng.data(); }
    constexpr auto size(this auto&& self)  { return self.rng.size(); }
};


// --- impl equality
template<Scalar_field F0, Scalar_field F1>
  requires std::equality_comparable_with<typename F0::value_type, typename F1::value_type>
constexpr bool operator==(const F0& x, const F1& y) {
  return std::ranges::equal(x, y);
}


// --- API types
template<class T, class A = mallocator>
class field : public field_impl {
  public:
    using value_type = T;
    constexpr static bool is_owner = false;

    constexpr
    field() = default;

    constexpr
    field(int64_t n)
      : rng(n)
    {}
    constexpr
    field(int64_t n, T value)
      : rng(n,value)
    {}
    constexpr
    field(std::initializer_list<T> l)
      : rng(l.begin(), l.end())
    {}

    template<class F>
      requires (can_convert_field<F,T>())
    field(const F& x)
      : rng(x)
    {}
  private:
    friend field_impl;
    std_e::dynarray<T,A> rng;
};

template<class T>
class field_view : public field_impl {
  public:
    using value_type = T;
    constexpr static bool is_owner = false;

    // --- ctors
    template<class F>
      requires (can_take_reference_of_field<F,T>())
    explicit
    field_view(F& x)
      : rng(x)
    {}

    constexpr
    field_view() = default;

    field_view(T* first, int64_t n)
      : rng(first, n)
    {}
    field_view(T* first, T* last)
      : rng(first, last)
    {}

    template<class F>
      requires (can_take_reference_of_field<F,T>())
    auto
    operator=(F& x) -> field_view& {
      this->rng = std_e::make_span(x); // rebind view to `x`
      return *this;
    }

  private:
    friend class field_impl;
    std_e::span<T> rng;
};

template<class T>
class field_ref : public field_impl {
  public:
    using value_type = T;
    constexpr static bool is_owner = false;

    // --- ctors
    template<class F>
      requires (can_take_reference_of_field<F,T>())
    explicit
    field_ref(F& x)
      : rng(x)
    {}

    field_ref(T* first, int64_t n)
      : rng(first, n)
    {}
    field_ref(T* first, T* last)
      : rng(first, last)
    {}

    template<class F>
      requires (Scalar_field<F> && std::convertible_to<typename F::value_type,T>)
    auto
    operator=(const F& x) -> field_ref& {
      this->rng = x; // assign through (copy `x` values into `this->rng`)
      return *this;
    }

  private:
    friend class field_impl;
    std_e::span_ref<T> rng;
};


// --- shorthand types (e.g. for tests)
using scalar_field = field<double, mallocator>;
using scalar_field_view = field_view<double>;
using scalar_field_const_view = field_view<const double>;
using scalar_field_ref = field_ref<double>;


} // std_e
