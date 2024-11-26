#pragma once


#include "std_e/future/contract.hpp"


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
#define STD_E_POSTCOND_LVL0(expr) STD_E_ASSERT_LVL0(expr)
#define STD_E_POSTCOND_LVL1(expr) STD_E_ASSERT_LVL1(expr)
#define STD_E_POSTCOND_LVL2(expr) STD_E_ASSERT_LVL2(expr)
