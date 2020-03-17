#pragma once


namespace std_e {


template<class I>
// requires I is integral type
struct interval {
  using value_type = I;
  I first;
  I last;
};

template<class I> constexpr auto
size(interval<I> r) -> I {
  return r.last - r.first;
}


template<class I> constexpr auto
is_in(I i, interval<I> r) -> bool {
  return (r.first <= i) && (i < r.last);
}


template<class Int_array> auto 
is_interval(const Int_array& x) -> bool {
  using I = typename Int_array::value_type;
  I sz = x.size();
  if (sz==0) return true;

  I next = x[0];
  for (I i=1; i<sz; ++i) {
    ++next;
    if (x[i] != next) {
      return false;
    }
  }
  return true;
}


} // std_e
