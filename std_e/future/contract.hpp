#pragma once

#include "std_e/base/assertion_failure.hpp"
#include <cassert>

namespace std_e {
  constexpr auto
  throw_assertion_failure(bool assertion, const char* file_name, int line_number, const char* assertion_str) {
    if (!(assertion)) throw ::std_e::assertion_failure(file_name,line_number,assertion_str);
  }
}

#ifdef NDEBUG
#define STD_E_ASSERT(b) (void(b))
#define STD_E_ASSERT_ABORT(b) (void(b))
#else
#define STD_E_ASSERT(b) ::std_e::throw_assertion_failure(b,__FILE__,__LINE__,#b)
#define STD_E_ASSERT_ABORT assert
#endif
