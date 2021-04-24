#pragma once

// from https://stackoverflow.com/a/45187436/1583122

// SFINAE-friendliness and noexcept-correctness
#if __GNUC__ > 7
#define RETURNS(...) \
  noexcept(noexcept(__VA_ARGS__)) \
  -> decltype(__VA_ARGS__){ return __VA_ARGS__; }
#else
#define RETURNS(...) \
  -> decltype(__VA_ARGS__){ return __VA_ARGS__; }
#endif


// LIFT(f) converts an overload set named "f"
// into a generic lambda, callable with exactly the same arguments that would be possible for "f"
#define LIFT(f) [](auto&&... xs) RETURNS(f(::std::forward<decltype(xs)>(xs)...))
