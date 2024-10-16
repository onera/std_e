#pragma once


#include <functional>


namespace std_e {


// much like std::unique, but the unique sequence does not have to begin where the input sequence begins
// contrary to std::unique_copy, this algorithm is well-defined if the source and destination ranges overlap
// Precondition: either source and destination ranges do not overlap, or d_first <= first
template<class ForwardIt, class Bin_pred = std::equal_to<> > auto
unique(ForwardIt first, ForwardIt last, ForwardIt d_first, Bin_pred p = {}) -> ForwardIt {
  if (first == last) return d_first;

  *d_first = *first;
  while (++first != last) {
    if (!p(*d_first,*first) && ++d_first != first) {
      *d_first = std::move(*first);
    }
  }

  return ++d_first;
}


} // std_e
