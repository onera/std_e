#pragma once


#include <algorithm>
#include "std_e/algorithm/overlapping.hpp"


namespace std_e {


// Preconditions:
//   - `array_idx` is an index interval of `array`
//   - function `f` has the following property:
//       if called with: `d_last = f(first,last,d_first)`
//       then d_last-d_first <= first-last
//     in other words, the output range is of smaller or equal size compared to the input range
// Implementation precondition (could be reworked)
//   - `array.resize` must exist
//   - `array_idx` starts at 0
template<class Integer_rng, class Rng, class F> auto
for_each_block(Integer_rng& array_idx, Rng& array, F&& f) -> void {
  using I = typename Integer_rng::value_type;
  using T = typename Rng::value_type;
  const I n_elt = array_idx.size()-1;

  T* first   = array.data();
  T* d_first = array.data();
  for (I i=0; i<n_elt; ++i) {
    T* next = array.data()+array_idx[i+1];
    // loop invariants:
    //     d_first <= first
    //     first <= next
    //     array_idx[i]   = d_first - array.data()
    //     array_idx[i+1] has not been yet modified

    auto d_next = f(first, next, d_first);
    I idx = d_next - array.data();
    
    first = next;
    d_first = d_next;
    array_idx[i+1] = idx;
  }

  array.resize(array_idx[n_elt]);
}

template<class Integer_rng, class Rng> auto
sort_unique_by_block(Integer_rng& array_idx, Rng& array) -> void {
  using T = typename Rng::value_type;
  auto f = [](T* first, T* last, T* d_first) {
    std::sort(first, last);
    return unique(first, last, d_first);
  };
  for_each_block(array_idx, array, f);
}


template<class Integer_rng, class Rng, class Pred> auto
remove_if_by_block(Integer_rng& array_idx, Rng& array, Pred&& p) -> void {
  using T = typename Rng::value_type;
  auto f = [&p](T* first, T* last, T* d_first) {
    return remove_if(first,last,d_first,p);
  };
  for_each_block(array_idx, array, f);
}

template<class Integer_rng, class Rng, class T> auto
remove_by_block(Integer_rng& array_idx, Rng& array, const T& value) -> void {
  auto p = [&value](const auto& x){ return x==value; };
  remove_if_by_block(array_idx, array, p);
}


} // std_e
