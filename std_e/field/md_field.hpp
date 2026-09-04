#pragma once


#include <ranges>
#include <array>
#include "std_e/future/dynarray.hpp"
#include "std_e/multi_index/fortran_order.hpp"
#include "std_e/future/span_ref.hpp"
#include "std_e/field/concepts.hpp"
#include "std_e/debug.hpp"
#include "std_e/contract/contract.hpp"
#include "std_e/field/md_field_uniform.hpp"
#include "std_e/field/field.hpp"
#include "std_e/contract/contract.hpp"

namespace std_e {


// --- impl
template<int... Ns>
class md_field_impl {
  public:
    static constexpr int rank = sizeof...(Ns);
    static constexpr std::array<int,rank> dims = {Ns...};
    static constexpr int dim_tot = (Ns * ... * 1);
    static_assert(dim_tot > 0);

    constexpr auto
    n_element(this auto&& self) -> int64_t {
      return int64_t(self.rngs[0].size());
    }

    constexpr auto
    data(this auto&& self, std::integral auto... is) {
      static_assert(sizeof...(is) == rank);
      auto i = self.index_of_field(is...);
      return self.rngs[i].data();
    }

    constexpr auto
    field(this auto&& self, std::integral auto... is) -> decltype(auto) {
      static_assert(sizeof...(is) == rank);
      auto i = self.index_of_field(is...);
      return self.rngs[i];
    }

    template<class Self> constexpr auto
    ref(this Self&& self, std::integral auto... is) {
      static_assert(sizeof...(is) == rank);
      auto i = self.index_of_field(is...);
      using T = std::decay_t<Self>::value_type;
      if constexpr (std::is_const_v<Self>) {
        return field_ref<const T>(self.rngs[i]);
      } else {
        return field_ref<T>(self.rngs[i]);
      }
    }

    constexpr auto
    operator()(this auto&& self, std::integral auto fld_idx, std::integral auto... is) -> decltype(auto) { // TODO invert fld_idx <-> is
      static_assert(sizeof...(is) == rank);
      auto i = self.index_of_field(is...);
      return self.rngs[i](fld_idx);
    }

    // --- array API
    constexpr auto begin(this auto&& self) { return self.rngs.begin(); }
    constexpr auto end(this auto&& self)   { return self.rngs.end(); }
    constexpr auto data(this auto&& self)  { return self.rngs.data(); }
    constexpr auto size(this auto&& self)  { return self.rngs.size(); }

  private:
  // static functions
    static constexpr auto
    index_of_field(std::integral auto... is) -> int {
      STD_E_ASSERT_LVL1(((std::in_range<int>(is)) && ...));
      return std_e::fortran_order_from_dimensions(
        dims, std_e::multi_index<int>{int(is)...}
      );
    }
};

template<class AT, int... Ns> auto
to_string(const md_field_impl<Ns...>& x) -> std::string {
  return to_string(x());
}


template<class T, class A, int... Ns>
class md_field : public md_field_impl<Ns...> {
  public:
    using value_type = T;
    constexpr static bool is_owner = true;

    md_field() = default;

    md_field(size_t n) {
      std::ranges::fill(rngs, field<T,A>(n));
    }

    md_field(size_t n, T value) {
      std::ranges::fill(rngs, field<T,A>(n,value));
    }

    md_field(size_t n, const md_field_uniform<T,Ns...>& uni_fld)
    {
      for (int i=0; i<this->dim_tot; ++i) {
        rngs[i] = field<T,A>(n,uni_fld(i));
      }
    }

    template<class F>
      requires (can_convert_field<F,T,Ns...>())
    md_field(const F& x)
      : md_field(x.n_element())
    {
      for (int i=0;i<this->dim_tot;++i) {
        std::ranges::copy(x.rngs[i], rngs[i]);
      }
    }

    template<class... Is> auto
    reclaim(Is... is) -> field<T,A>&& {
      return std::move(this->field(is...));
    }
  private:
  public: // TODO
  // data members
    using impl = md_field_impl<Ns...>;
    friend impl;
    std::array<field<T,A>, impl::dim_tot> rngs;
};

template<class T, int... Ns>
class md_field_view : public md_field_impl<Ns...> {
  public:
    using value_type = T;
    constexpr static bool is_owner = false;

    md_field_view() = default;

    template<class F>
      requires (can_take_reference_of_field<F,T,Ns...>())
    md_field_view(F& x)
    {
      for (int i=0; i<this->dim_tot; ++i) {
        rngs[i] = x.rngs[i];
      }
    }
  private:
  public: // TODO
  // data members
    using impl = md_field_impl<Ns...>;
    friend impl;
    std::array<field_view<T>, impl::dim_tot> rngs;
};

template<Vector_field F> 
auto
row(F& x, int i) {
  return x.field(i);
}
template<Tensor_field F> 
auto
row(F& x, int i) {
  using FT = std::decay_t<F>;
  using T = typename FT::value_type;
  md_field_view<T, F::dims[1]> res;
  for (int j=0; j<F::dims[1]; ++j) {
    res.rngs[j] = x.field(i,j);
  }
  return res;
}

// TODO clean this ugly one
template<class Md_field_0, class Md_field_1, class Md_field_2> auto
v_stack(Md_field_0& x, Md_field_1& y, Md_field_2& z) {
  using FT0 = std::decay_t<Md_field_0>;
  using FT1 = std::decay_t<Md_field_1>;
  using FT2 = std::decay_t<Md_field_2>;
  using T0 = typename FT0::value_type;
  using T1 = typename FT1::value_type;
  using T2 = typename FT2::value_type;
  static_assert(std::is_same_v<T0,T1>);
  static_assert(std::is_same_v<T0,T2>);
  //static_assert(x.rank <= 2);
  //static_assert(y.rank <= 2);
  if constexpr (Md_field_0::rank == 0 && Md_field_1::rank == 0 && Md_field_2::rank == 0) {
    md_field_view<T0, 3> res;
    res.field(0) = x;
    res.field(1) = y;
    res.field(2) = z;
    return res;
  } else if constexpr (Md_field_0::rank == 0 && Md_field_1::rank == 1 && Md_field_2::rank == 0) {
    constexpr int n_row = 1+Md_field_1::dims[0]+1;
    md_field_view<T0, n_row> res;
    res.field(0) = x;
    for (int i=0; i<n_row-2; ++i) {
      res.field(1+i) = y.field(i);
    }
    res.field(n_row-1) = z;
    return res;
  } else if constexpr (Md_field_0::rank == 1 && Md_field_1::rank == 2 && Md_field_2::rank == 1) {
    static_assert(Md_field_0::dims[0] == Md_field_1::dims[1]);
    static_assert(Md_field_2::dims[0] == Md_field_1::dims[1]);
    constexpr int n_row = 1+Md_field_1::dims[0]+1;
    constexpr int n_col = Md_field_0::dims[0];
    md_field_view<T0, n_row, n_col> res;
    for (int j=0; j<n_col; ++j) {
      res.field(0,j) = x.field(j);
    }
    for (int j=0; j<n_col; ++j) {
      for (int i=0; i<n_row-2; ++i) {
        res.field(1+i,j) = y.field(i,j);
      }
    }
    for (int j=0; j<n_col; ++j) {
      res.field(n_row-1,j) = z.field(j);
    }
    return res;
  } else {
    throw std_e::msg_exception("Not implemented");
  }
}

// --- shorthand types (e.g. for tests)
template<int N>          using vector_field = md_field<double, mallocator, N>;
template<int N0, int N1> using tensor_field = md_field<double, mallocator, N0, N1>;

template<int N>          using vector_field_view = md_field_view<double, N>;
template<int N0, int N1> using tensor_field_view = md_field_view<double, N0, N1>;


} // std_e
