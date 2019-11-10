#pragma once


#include "std_e/multi_index/concept.hpp"
#include "std_e/future/algorithm.hpp"
#include "std_e/base/macros.hpp"


namespace std_e {


template<class Multi_index> FORCE_INLINE constexpr auto
cartesian_product(const Multi_index& a) {
  return std_e::accumulate(begin(a),end(a),1,std::multiplies<>());
}


} // std_e
