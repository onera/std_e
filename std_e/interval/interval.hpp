#pragma once


namespace std_e {

/**
concept Interval: Random_access_range
  length(Interval) -> value_type
  inf(Interval) -> value_type
  sup(Interval) -> value_type
*/

template<class I>
// requires I is integral type
struct interval {
  using value_type = I;
  I first;
  I last;
};

template<class I> interval(I first, I last) -> interval<I>;

template<class I> [[deprecated("use length()")]] constexpr auto
size(interval<I> r) -> I {
  return r.last - r.first;
}

template<class I> constexpr auto
operator==(const interval<I>& x, const interval<I>& y) -> bool {
  return x.first==y.first && x.last==y.last;
}
template<class I> constexpr auto
operator!=(const interval<I>& x, const interval<I>& y) -> bool {
  return !(x==y);
}

template<class I> constexpr auto
length(interval<I> r) -> I {
  return r.last - r.first;
}
template<class I> constexpr auto
inf(interval<I> r) -> I {
  return r.first;
}
template<class I> constexpr auto
sup(interval<I> r) -> I {
  return r.last;
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
