#pragma once


namespace std_e {


template<class I>
// requires I is integral type
struct integer_range {
  I first;
  I last;
};

template<class I> constexpr auto
// requires I is integral type
in(I i, const integer_range<I>& r) -> bool {
  return (r.first <= i) && (i < r.last);
}

template<class I> constexpr auto
// requires I is integral type
length(const integer_range<I>& r) -> I {
  return r.last - r.first;
}


} // std_e
