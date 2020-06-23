#pragma once

#include "std_e/base/assertion_failure.hpp"

#ifdef NDEBUG
#define STD_E_ASSERT(b) (void(0))
#else
#define STD_E_ASSERT(b) if (!(b)) throw ::std_e::assertion_failure(__FILE__,__LINE__,#b)
#endif
