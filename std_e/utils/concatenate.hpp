#pragma once


#include "std_e/base/array.hpp"
#include "std_e/future/algorithm.hpp"


namespace std_e {


template<class T, int... Ns> constexpr auto
create_array_for_concatenation(const std_e::array<T,Ns>&...) {
  constexpr int N = (Ns + ...);
  return std_e::array<T,N>{};
}
template<class... Arrays> constexpr auto
create_array_for_concatenation(const Arrays&... xs) {
  size_t n = (xs.size() + ...);
  return std::common_type_t<Arrays...>(n);
}


template<class Array, class... Arrays> constexpr auto
// requires Arrays are all Array<T> (same T)
concatenate_in(Array& cat_xs, const Arrays&... xs) -> Array& {
  auto first = cat_xs.begin();
  ( ( std_e::copy_n(xs.begin(),xs.size(),first),
      std::advance(first , xs.size()          ) ) , ...);
  return cat_xs;
}


template<class... Arrays> constexpr auto
// requires Arrays are all Array<T> (same T)
concatenate(const Arrays&... xs)  {
  auto cat_xs = create_array_for_concatenation(xs...);
  concatenate_in(cat_xs,xs...);
  return cat_xs;
}


} // std_e
