#pragma once


#include <deque>
#include "std_e/utils/to_string_fwd.hpp"


namespace std_e {


template<class T, class A> auto
to_string(const std::deque<T,A>& x) -> std::string {
  return range_to_string(x);
}


} // std_e
