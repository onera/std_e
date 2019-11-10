#pragma once


namespace std_e {


template<class Array> constexpr auto
invert_permutation(const Array& p) -> Array {
  using indexing_type = typename Array::difference_type;
  indexing_type sz = p.size();
  Array inv_p(sz);
  for (indexing_type i=0; i<sz; ++i) {
    inv_p[p[i]] = i;
  }
  return inv_p;
}


} // std_e
