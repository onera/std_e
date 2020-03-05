#pragma once


#include "std_e/interval/interval.hpp"
#include <type_traits>
#include <functional>
#include <numeric>


namespace std_e {


template<class T, class I, class Func, class Binary_op> inline auto
// requires I is integral type
// requires Func(I) -> T && Binary_op(T,T) -> T
apply_accumulate(const interval<I>& range, Func f, T init, Binary_op op) {
  // Precondition: range.first <= range.last
  for (I i=range.first; i<range.last; ++i) {
    init = op(init,f(i));
  }
  return init;
}

template<class Func, class I> inline auto
// requires I is integral type
// requires Func(I) -> T && T(0) returns neutral element for addition
sum(const interval<I>& range, Func f, std::invoke_result_t<Func,I> init = 0) -> std::invoke_result_t<Func,I> {
  return apply_accumulate(range,f,init,std::plus<std::result_of_t<Func(I)>>());
}

template<class Func, class I> inline auto
// requires I is integral type
// requires Func(I) -> T && T(1) returns neutral element for multiplication
prod(const interval<I>& range, Func f, std::invoke_result_t<Func,I> init = 1) -> std::invoke_result_t<Func,I> {
  return apply_accumulate(range,f,init,std::multiplies<std::result_of_t<Func(I)>>());
}

template<class I, size_t N> auto
index_with_fortran_memory_layout(const std::array<I,N>& coords, const std::array<I,N>& dims) -> I {
  // Preconditions: 0 <= coords[i] < dims[i]   for all 0 <= i < dims.size()
  return sum(/*a=*/interval<I>{0,N},                                                              [&](I a) { return
                                    coords[a] * prod(/*b=*/interval<I>{0,a},                      [&](I b) { return
                                                                            dims[b];
              });
         });
}
template<class I, size_t N> auto
index_with_c_memory_layout(const std::array<I,N>& coords, const std::array<I,N>& dims) -> I {
  // Preconditions: 0 <= coords[i] < dims[i]   for all 0 <= i < dims.size()
  return sum(/*a=*/interval<I>{0,N},                                                              [&](I a) { return
                                    coords[a] * prod(/*b=*/interval<I>{a+1,N},                    [&](I b) { return
                                                                              dims[b];
              });
         });
}


} // std_e
