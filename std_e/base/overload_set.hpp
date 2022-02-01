#pragma once

// from https://stackoverflow.com/a/45187436/1583122

// SFINAE-friendliness and noexcept-correctness
#if __GNUC__ > 7
#define STD_E_RETURNS(...) \
  noexcept(noexcept(__VA_ARGS__)) \
  -> decltype(__VA_ARGS__){ return __VA_ARGS__; }
#else
#define STD_E_RETURNS(...) \
  -> decltype(__VA_ARGS__){ return __VA_ARGS__; }
#endif


// STD_E_OVERLOAD_SET(f) converts an overload set named "f"
// into a generic lambda, callable with exactly the same arguments that would be possible for "f"
#define STD_E_OVERLOAD_SET(f) [](auto&&... xs) STD_E_RETURNS(f(::std::forward<decltype(xs)>(xs)...))
