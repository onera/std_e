#pragma once


// A for loop that will always be flattened
// provided that recursive functions are inlined
// (note: used to work around icpc <= 17 and gcc 8 not being able to flatten an inner loop in a SIMD context)


#include "std_e/base/macros.h"
#include "std_e/base/index_t.h"


namespace std_e {


template<index_t N, class callable_type>
struct inline_for__impl {
  template<index_t start, class... arg_types> static FORCE_INLINE constexpr auto
  loop(arg_types&&... args) -> void {
    callable_type::template call<start>(args...);
    inline_for__impl<N-1,callable_type>::template loop<start+1>(args...);
  }
};
template<class callable_type>
struct inline_for__impl<0,callable_type> {
  template<index_t,class... arg_types> static FORCE_INLINE constexpr auto
  loop(arg_types&&...) -> void {}
};


template<index_t N, class callable_type, class... arg_types>
// requires callable_type has static method template<index_t>call(arg_types...) -> void 
FORCE_INLINE constexpr void
inline_for(arg_types&&... args) {
  // Precondition: Domain(callable_type::call) contains [0,N)

  // Note : the following equivalent does not flatten in spite of the pragma
  //#pragma unroll(N)
  //for (index_t i=0; i<N; ++i) {
  //    f(i);
  //}
  inline_for__impl<N,callable_type>::template loop<0>(args...);
}


} // std_e
