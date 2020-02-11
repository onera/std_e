#pragma once

#include <utility>

// force inline for gcc
#define FORCE_INLINE inline __attribute__((always_inline))
#define FORCE_NO_INLINE __attribute__ ((noinline))
#define RESTRICT __restrict

#define FWD(x) std::forward<decltype(x)>(x)

#define STD_E_UNUSED(x) ((void)(x))
