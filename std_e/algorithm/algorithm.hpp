#pragma once


#include <algorithm>


namespace std_e {


template<class InputIt1, class InputIt2, class Integer> constexpr bool
equal_n(InputIt1 first1, InputIt2 first2, Integer n) {
    for (Integer i=0; i<n; ++first1, ++first2, ++i) {
        if (!(*first1 == *first2)) {
            return false;
        }
    }
    return true;
}
template<class InputIt1, class InputIt2, class Integer, class BinaryPredicate> constexpr bool
equal_n(InputIt1 first1, InputIt2 first2, Integer n, BinaryPredicate p) {
    for (Integer i=0; i<n; ++first1, ++first2, ++i) {
        if (!p(*first1, *first2)) {
            return false;
        }
    }
    return true;
}

template<class Fwd_it, class S, class T, class Bin_op, class Bin_pred> auto
accumulate_while_adjacent(Fwd_it first, S last, T init, Bin_op op, Bin_pred p) -> std::pair<Fwd_it,T> {
  if (first==last) return std::make_pair(first,init);

  init = op(init,*first);
  Fwd_it next = std::next(first);
  while( next!=last && p(*first,*next) ) {
    init = op(init,*next);
    ++next;
  }
  return std::make_pair(next,init);
}


template<class Random_access_range, class T> constexpr auto
lower_bound_position(const Random_access_range& r, const T& value) {
  auto it = std::lower_bound(begin(r),end(r),value);
  return it-begin(r);
}


template<class Range, class I> auto 
// requires I is an arithmetic type
// requires Range::value_type==I
offset(Range& r, I value) -> void {
  for (auto& elt : r) {
    elt += value;
  }
}


template<class Range, class T> auto
// requires Range::value_type==T
contains(const Range& r, const T& value) -> bool {
  auto it = std::find(r.begin(),r.end(),value);
  return it != r.end();
}


} // std_e
