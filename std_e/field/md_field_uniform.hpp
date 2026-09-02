#pragma once

#include <ranges>
#include <array>
#include "std_e/multi_index/fortran_order.hpp"
#include "std_e/field/concepts.hpp"
#include "std_e/contract/contract.hpp"


namespace std_e {


template<class T, int... Ns> 
struct md_field_uniform {
  public:
    using value_type = T;
    static constexpr int rank = sizeof...(Ns);
    static constexpr std::array<int,rank> dims = {Ns...};
    static constexpr int dim_tot = (Ns * ... * 1);

    constexpr
    md_field_uniform() = default;

    constexpr
    md_field_uniform(std::initializer_list<T> l)
    {
      static_assert(rank==1);
      int i=0;
      for (const value_type& x : l) {
        (*this)(i) = x;
        ++i;
      }
    }
    constexpr
    md_field_uniform(std::initializer_list<std::initializer_list<T>> ll)
    {
      static_assert(rank==2);
      int i=0;
      for (const auto& l : ll) {
        int j=0;
        for (const value_type& x : l) {
          (*this)(i,j) = x;
          ++j;
        }
        ++i;
      }
    }

    constexpr auto
    operator()(this auto&& self, std::integral auto... is) -> auto&& {
      static_assert(sizeof...(is) == rank);
      auto i = self.index_of_field(is...);
      STD_E_ASSERT_LVL1(i < dim_tot);
      return self._values[i];
    }

  private:
  // member functions
    static constexpr auto
    index_of_field(std::integral auto... is) -> int {
      STD_E_ASSERT_LVL1(((std::in_range<int>(is)) && ...));
      return std_e::fortran_order_from_dimensions(
        dims, std_e::multi_index<int>{int(is)...}
      );
    }

  // data members
    std::array<T, dim_tot> _values;
};


template<int N>
struct vector_field_uniform : md_field_uniform<double, N> {
  using base = md_field_uniform<double, N>;
  using base::base;
};
template<int N0, int N1>
struct tensor_field_uniform : md_field_uniform<double, N0, N1> {
  using base = md_field_uniform<double, N0, N1>;
  using base::base;
};


} // std_e
