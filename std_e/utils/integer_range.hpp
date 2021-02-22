#pragma once


#include <type_traits>
#include "std_e/future/contract.hpp"


namespace std_e {


// integer_range { // TODO factor with std_e/interval

/// integer_range is a semi-open range [first,last)
template<class I>
// requires I is integral type
struct integer_range {
  I first;
  I last;

  // ctors
  integer_range() = default;

  integer_range(I first, I last)
    : first(first)
    , last(last)
  {
    STD_E_ASSERT(first<=last);
  }

  template<class I0>
  integer_range(const integer_range<I0> x)
    : first(x.first)
    , last(x.last)
  {}

  template<class I0>
  integer_range& operator=(const integer_range<I0> x) {
    first = x.first;
    last = x.last;
  }
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
template<class I> constexpr auto
// requires I is integral type
contiguous(const integer_range<I>& r0, const integer_range<I>& r1) -> bool {
  return r0.last = r1.first;
}

template<class I> using integer_range_ref = integer_range<I&>;
// integer_range }




// closed_integer_range {
template<class I>
// requires I is integral type
struct integer_closed_range {
  I first;
  I last;

  // ctors
  integer_closed_range() = default;

  integer_closed_range(I first, I last)
    : first(first)
    , last(last)
  {
    STD_E_ASSERT(first<=last);
  }

  template<class I0>
  integer_closed_range(const integer_closed_range<I0> x)
    : first(x.first)
    , last(x.last)
  {}

  template<class I0>
  integer_closed_range& operator=(const integer_closed_range<I0> x) {
    first = x.first;
    last = x.last;
  }
};

template<class I> auto
operator==(const integer_closed_range<I>& x, const integer_closed_range<I>& y) -> bool {
  return x.first==y.first && x.last==y.last;
}
template<class I> auto
operator!=(const integer_closed_range<I>& x, const integer_closed_range<I>& y) -> bool {
  return !(x==y);
}

template<class I> auto
to_semi_open(const integer_closed_range<I>& x) {
  return integer_range<std::decay_t<I>>{x.first,x.last+1};
}

template<class I> constexpr auto
// requires I is integral type
in(I i, const integer_closed_range<I>& r) -> bool {
  return in(i,to_semi_open(r));
}

template<class I> constexpr auto
// requires I is integral type
length(const integer_closed_range<I>& r) {
  return length(to_semi_open(r));
}
template<class I> constexpr auto
// requires I is integral type
contiguous(const integer_closed_range<I>& r0, const integer_closed_range<I>& r1) -> bool {
  return r0.last+1 == r1.first;
}

template<class I> using integer_closed_range_ref = integer_closed_range<I&>;
// closed_integer_range }


} // std_e
