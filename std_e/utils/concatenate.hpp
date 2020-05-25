#pragma once


#include "std_e/concept/array.hpp"
#include "std_e/future/algorithm.hpp"
#include "std_e/utils/array.hpp"


namespace std_e {


template<class... Arrays> constexpr auto
create_array_for_concatenation(const Arrays&... xs) {
  size_t n = (xs.size() + ...);
  using cat_array_type = concatenated_array<Arrays...>;
  return make_array_of_size<cat_array_type>(n);
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
