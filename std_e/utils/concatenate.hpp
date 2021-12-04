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


// concatenation as a reduction {
template<class T> auto
concatenate_in_vector__impl(T x) -> std::vector<T> {
  return {std::move(x)};
}
template<class T> auto
concatenate_in_vector__impl(std::vector<T>& v, T x) -> std::vector<T>& {
  v.emplace_back(std::move(x));
  return v;
}
// Intel needs inline
inline auto concatenate_in_vector = [](auto&&... xs){ return concatenate_in_vector__impl(FWD(xs)...); };
// concatenation as a reduction }


template<class Array0, class Array1> constexpr auto
append(Array0& x, const Array1& y)  {
  std::copy(begin(y),end(y),back_inserter(x));
}
template<class Array0, class Array1> constexpr auto
emplace_back(Array0& x, Array1&& y)  {
  std::move(begin(y),end(y),back_inserter(x));
}


} // std_e
