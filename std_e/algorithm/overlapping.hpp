#pragma once


#include <functional>


namespace std_e {


// much like std::unique, but the unique sequence does not have to begin where the input sequence begins
// contrary to std::unique_copy, this algorithm is well-defined if the source and destination ranges overlap
// Precondition: either source and destination ranges do not overlap, or d_first <= first
template<class Fwd_it, class Bin_pred = std::equal_to<> > auto
unique(Fwd_it first, Fwd_it last, Fwd_it d_first, Bin_pred p = {}) -> Fwd_it {
  if (first == last) return d_first;

  *d_first = *first;
  while (++first != last) {
    if (!p(*d_first,*first) && ++d_first != first) {
      *d_first = std::move(*first);
    }
  }

  return ++d_first;
}


// much like std::remove_if, but the unique sequence does not have to begin where the input sequence begins
// contrary to std::remove_if_copy, this algorithm is well-defined if the source and destination ranges overlap
// Precondition: either source and destination ranges do not overlap, or d_first <= first
// See also std_e::unique() that has the same idea
template<class Fwd_it, class Unary_pred> auto
remove_if(Fwd_it first, Fwd_it last, Fwd_it d_first, Unary_pred&& p) -> Fwd_it {
  while (first != last) {
    if (!(p(*first))) {
      *d_first++ = std::move(*first);
    }
    ++first;
  }
  return d_first;
}
template<class Fwd_it, class T> auto
remove(Fwd_it first, Fwd_it last, Fwd_it d_first, const T& value) -> Fwd_it {
  auto p = [&value](const T& x){ return x==value; };
  return remove_if(first, last, d_first, p);
}


} // std_e
