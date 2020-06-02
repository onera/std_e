#pragma once


#include <algorithm>
#include "std_e/interval/interval.hpp"


namespace std_e {


template<class I> auto
rotated_position(I index, I first, I n_first, I last) -> I {
  // Returns the new index of a sequence [first,last) having been rotated around n_first
  // Preconditions:
  // - first <= n_first <= last
  I first_range_size = n_first-first;
  I second_range_size = last-n_first;
  if (is_in(index,{first,n_first})) {
    return index + second_range_size;
  }
  if (is_in(index,{n_first,last})) {
    return index - first_range_size;
  }
  return index;
}


} // std_e
