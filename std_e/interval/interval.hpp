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
is_interval(const Int_array& a) -> bool {
  if (a.size()==0) return true;

  using I = typename Int_array::value_type;
  I next = a[0];
  for (I i=1; i<a.size(); ++i) {
    ++next;
    if (a[i] != next) {
      return false;
    }
  }
  return true;
}


} // std_e
