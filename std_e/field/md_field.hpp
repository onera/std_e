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
template<std::ranges::sized_range Rng, int... Ns>
class md_field_impl {
  public:
    using T = Rng::value_type;
    using value_type = T;
    static constexpr int rank = sizeof...(Ns);
    static constexpr std::array<int,rank> dims = {Ns...};
    static constexpr int dim_tot = (Ns * ... * 1);

    md_field_impl() = default;



    constexpr auto
    underlying(this auto&& self, std::integral auto... is) -> auto& {
      static_assert(sizeof...(is) == rank);
      auto i = self.index_of_field(is...);
      return self.rngs[i];
    }

    constexpr auto
    field(this auto&& self, std::integral auto... is) {
      static_assert(sizeof...(is) == rank);
      auto i = self.index_of_field(is...);
      return field_ref<T>(std_e::make_span_ref(self.rngs[i].begin(), self.n_element()));
    }
    constexpr auto
    data(this auto&& self, std::integral auto... is) -> T* {
      static_assert(sizeof...(is) == rank);
      return self.field(is...).data();
    }

    template<class I>constexpr auto
    operator()(this auto&& self, I fld_idx, std::integral auto... is) -> auto& {
      static_assert(sizeof...(is) == rank);
      return self.field(is...)(fld_idx);
    }

    auto
    n_element() const -> int64_t {
      return int64_t(rngs[0].size());
    }

    auto underlying(this auto&& self) -> auto& { return self.rngs; }
    auto underlying_linear(this auto&& self, int i) -> auto& { return self.rngs[i]; }

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

  // data members
    std::array<Rng, dim_tot> rngs;
};

template<class AT, int... Ns> auto
to_string(const md_field_impl<AT,Ns...>& x) -> std::string {
  return to_string(x.underlying());
}

template<class T, class A, int... Ns>
class md_field : public md_field_impl<std_e::dynarray<T,A>, Ns...> {
  public:
    using array_1d_t = std_e::dynarray<T,A>;
    using base = md_field_impl<array_1d_t, Ns...>;
    using base::base;

    md_field() = default;

    md_field(size_t n)
    {
      std::ranges::fill(this->underlying(), array_1d_t(n));
    }

    template<class Array_type0> md_field(const md_field_impl<Array_type0,Ns...>& x)
      : md_field(x.n_element())
    {
      for (int i=0;i<this->dim_tot;++i)
      {
        using T0 = typename Array_type0::value_type;
        for (int j=0;j<this->n_element();++j)
        {
          T& tmp = this->underlying_linear(i)[j];
          const T0& tmp0 = x.underlying_linear(i)[j];
          tmp = tmp0;
        }
      }
    }

    md_field(size_t n, T value)
    {
      for (int i=0; i<this->dim_tot; ++i) {
        this->underlying_linear(i) = array_1d_t(n,value);
      }
    }

    md_field(size_t n, const md_field_uniform<T,Ns...>& uni_fld)
    {
      for (int i=0; i<this->dim_tot; ++i) {
        this->underlying_linear(i) = array_1d_t(n,uni_fld(i));
      }
    }

    template<class... Is> auto
    reclaim(Is... is) -> array_1d_t&& {
      return std::move(this->underlying(is...));
    }
};

template<class T, int... Ns>
class md_field_view : public md_field_impl<std_e::span<T>, Ns...> {
  public:
    using array_1d_t = std_e::span<T>;
    using base = md_field_impl<array_1d_t, Ns...>;
    using base::base;

    md_field_view() = default;

    template<class Md_field_type>
    md_field_view(Md_field_type& x)
    {
      for (int i=0; i<this->dim_tot; ++i) {
        this->underlying_linear(i) = std_e::make_span(x.underlying_linear(i));
      }
    }
};

template<class Md_field> auto
row(Md_field& x, int i) {
  using FT = std::decay_t<Md_field>;
  using T = typename FT::value_type;
  if constexpr (Md_field::rank == 1) {
    return x.field(i);
  } else if constexpr (Md_field::rank == 2) {
    md_field_view<T, Md_field::dims[1]> res;
    for (int j=0; j<Md_field::dims[1]; ++j) {
      res.underlying(j) = x.underlying(i,j);
    }
    return res;
  } else {
    throw std_e::msg_exception("Not implemented");
  }
}

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
    res.underlying(0) = x;
    res.underlying(1) = y;
    res.underlying(2) = z;
    return res;
  } else if constexpr (Md_field_0::rank == 0 && Md_field_1::rank == 1 && Md_field_2::rank == 0) {
    constexpr int n_row = 1+Md_field_1::dims[0]+1;
    md_field_view<T0, n_row> res;
    res.underlying(0) = x;
    for (int i=0; i<n_row-2; ++i) {
      res.underlying(1+i) = y.underlying(i);
    }
    res.underlying(n_row-1) = z;
    return res;
  } else if constexpr (Md_field_0::rank == 1 && Md_field_1::rank == 2 && Md_field_2::rank == 1) {
    static_assert(Md_field_0::dims[0] == Md_field_1::dims[1]);
    static_assert(Md_field_2::dims[0] == Md_field_1::dims[1]);
    constexpr int n_row = 1+Md_field_1::dims[0]+1;
    constexpr int n_col = Md_field_0::dims[0];
    md_field_view<T0, n_row, n_col> res;
    for (int j=0; j<n_col; ++j) {
      res.underlying(0,j) = x.underlying(j);
    }
    for (int j=0; j<n_col; ++j) {
      for (int i=0; i<n_row-2; ++i) {
        res.underlying(1+i,j) = y.underlying(i,j);
      }
    }
    for (int j=0; j<n_col; ++j) {
      res.underlying(n_row-1,j) = z.underlying(j);
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
