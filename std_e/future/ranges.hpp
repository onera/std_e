#pragma once


#include "std_e/functional/pipeable.hpp"
#include "std_e/future/ranges/concept.hpp"
#include "std_e/future/ranges/algorithm.hpp"
#include <ranges>


namespace std_e {


constexpr auto
to_vector_fn = [](auto&& rng) {
  using Range = std::remove_cvref_t<decltype(rng)>;
  using T = std::ranges::range_value_t<Range>;
  if constexpr (std_e::ranges::sized_range<Range>) { // possible to allocate once
    std::vector<T> v(rng.size());
    std_e::ranges::copy(FWD(rng),begin(v));
    return v;
  } else { // need to use a back_inserter
    std::vector<T> v;
    std_e::ranges::copy(FWD(rng),back_inserter(v));
    return v;
  }
};
using to_vector_closure = decltype(to_vector_fn);

// If you are like me and don't know how this function work,
// don't panic! Its goal is just to provide
// an additional overload to the `to_vector` function above
// that is usable with the pipe (|) syntax.
// See also this talk by Eric Niebler: https://youtu.be/mFUXNMfaciE
constexpr auto
to_vector() {
  return make_pipeable(to_vector_fn);
}


template<class Rng> constexpr auto
copy_to(Rng&& dest) { // Note: can be an rvalue ref (e.g. span&&)...
  auto copy_to_fn = [&dest](auto&& origin){ // ... it is always capture by lvalue ref
                                            // this is OK: garanteed to be alive till the end of the function
    std_e::ranges::copy(FWD(origin),begin(dest));
  };
  return make_pipeable(copy_to_fn);
}


} // std_e
