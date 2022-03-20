#pragma once


#include "std_e/concept/array.hpp"
#include "std_e/future/algorithm.hpp"
#include "std_e/utils/array.hpp"
#if __cplusplus > 201703L
  #include <ranges>
#endif


namespace std_e {


template<class Cat_array, class... Arrays> constexpr auto
create_array_for_concatenation(const Arrays&... xs) {
  size_t n = (xs.size() + ...);
  return make_array_of_size<Cat_array>(n);
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
  using cat_array_type = concatenated_array<Arrays...>;
  auto cat_xs = create_array_for_concatenation<cat_array_type>(xs...);
  concatenate_in(cat_xs,xs...);
  return cat_xs;
}
template<class... Arrays> constexpr auto
// requires Arrays are all Array<T> (same T)
concatenate_in_vector(const Arrays&... xs)  {
  #if __cplusplus > 201703L
    using T = std::common_type_t<std::ranges::range_value_t<Arrays>...>;
  #else
    using T = std::common_type_t<typename Arrays::value_type...>;
  #endif
  using cat_array_type = std::vector<T>;
  auto cat_xs = create_array_for_concatenation<cat_array_type>(xs...);
  concatenate_in(cat_xs,xs...);
  return cat_xs;
}


template<class Array0, class Array1> constexpr auto
append(Array0& x, const Array1& y)  {
  std::copy(begin(y),end(y),back_inserter(x));
}
template<class Array0, class Array1> constexpr auto
emplace_back(Array0& x, Array1&& y)  {
  std::move(begin(y),end(y),back_inserter(x));
}


} // std_e
