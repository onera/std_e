#pragma once


#include "std_e/future/contract.hpp"

// Vocabulary: // TODO extract in doc
// Say we have an algorithm A that uses a check C
// Lets write Cx(A) the complexity of A without including C in it and Cx(C) the complexity of C
//
// C is said to be *cheap* (or LVL_0) if Cx(C) / Cx(A) = O(1)
// C is said to be *medium* (or LVL_1) if Cx(C) ~ Cx(A)
// C is said to be *expansive* (or LVL_2) in all other cases
//
// Note that while we typically think in terms of time complexity, we should actually think about the time, memory, and any other relevant kind of complexity (typically: networking/access to memory). The check level should be determined from the least favorable ratio.
//
// Example:
// If A returns the minimum element of an array and C check the array is of size>0, then C is cheap
// If A returns the inverse of an array of doubles and C check the elements are non-zero, then C is medium
// If A is a binary search over a sorted array and C checks the array is sorted, then C is expansive

#ifdef STD_E_ASSERT_LEVEL
  #if STD_E_ASSERT_LEVEL < -1 && STD_E_ASSERT_LEVEL > 2
    #error "STD_E_ASSERT_LEVEL must be defined with value -1 (no check), 0 (cheap checks only), 1 (cheap and medium checks), or 2 (all checks)"
  #endif
#else
  #define STD_E_ASSERT_LEVEL 0 // default
#endif


// First define the macros as doing nothing (in particular, `expr` is not evaluated)...
#define STD_E_ASSERT_LVL0(expr)
#define STD_E_ASSERT_LVL1(expr)
#define STD_E_ASSERT_LVL2(expr)

// ...then override according to `STD_E_ASSERT_LEVEL`
#if STD_E_ASSERT_LEVEL >= 0
  #undef STD_E_ASSERT_LVL0
  #define STD_E_ASSERT_LVL0(expr) ::std_e::throw_assertion_failure(expr,__FILE__,__LINE__,#expr)
#endif
#if STD_E_ASSERT_LEVEL >= 1
  #undef STD_E_ASSERT_LVL1
  #define STD_E_ASSERT_LVL1(expr) ::std_e::throw_assertion_failure(expr,__FILE__,__LINE__,#expr)
#endif
#if STD_E_ASSERT_LEVEL >= 2
  #undef STD_E_ASSERT_LVL2
  #define STD_E_ASSERT_LVL2(expr) ::std_e::throw_assertion_failure(expr,__FILE__,__LINE__,#expr)
#endif


// These ones do the same as `STD_E_ASSERT_LVL*`, but with more explicit names
#define STD_E_PRECOND_LVL0(expr)  STD_E_ASSERT_LVL0(expr)
#define STD_E_PRECOND_LVL1(expr)  STD_E_ASSERT_LVL1(expr)
#define STD_E_PRECOND_LVL2(expr)  STD_E_ASSERT_LVL2(expr)

#define STD_E_INVARIANT_LVL0(expr)  STD_E_ASSERT_LVL0(expr)
#define STD_E_INVARIANT_LVL1(expr)  STD_E_ASSERT_LVL1(expr)
#define STD_E_INVARIANT_LVL2(expr)  STD_E_ASSERT_LVL2(expr)

#define STD_E_POSTCOND_LVL0(expr) STD_E_ASSERT_LVL0(expr)
#define STD_E_POSTCOND_LVL1(expr) STD_E_ASSERT_LVL1(expr)
#define STD_E_POSTCOND_LVL2(expr) STD_E_ASSERT_LVL2(expr)
