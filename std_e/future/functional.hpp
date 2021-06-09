#pragma once

#include "std_e/base/macros.hpp"

namespace std_e {

template<class F> auto
not_fn(F&& fn) {
  return [f = FWD(fn)](auto&&... xs){ return !f(FWD(xs)...); };
}


} // std_e
