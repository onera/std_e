#pragma once

#include <utility>

// workaround to clang and icc pretending they are gcc
// from https://stackoverflow.com/q/38499462/1583122
#if defined(__GNUC__) && !defined(__llvm__) && !defined(__INTEL_COMPILER)
#define REAL_GCC __GNUC__ // probably
#endif

// force inline for gcc
#define FORCE_INLINE inline __attribute__((always_inline))
#define FORCE_NO_INLINE __attribute__ ((noinline))
#define RESTRICT __restrict

#define FWD(x) std::forward<decltype(x)>(x)

#define STD_E_UNUSED(x) ((void)(x))
