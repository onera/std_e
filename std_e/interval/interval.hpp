#pragma once


#include <type_traits>
#include <utility>


namespace std_e {

/**
concept Interval: Random_access_range
  length(Interval) -> value_type
  inf(Interval) -> value_type
  sup(Interval) -> value_type
*/

template<class I>
// requires I is integral type
class interval : private std::pair<I,I> {
  public:
    using value_type = std::decay_t<I>;
    using base = std::pair<I,I>;

    interval(I f, I l)
      : base(f,l)
    {}

    template<class I0>
    interval(const interval<I0>& other)
      : base(other.as_base())
    {}
    template<class I0> auto
    operator=(const interval<I0>& other) -> interval& {
      as_base() = other.as_base();
      return *this;
    }

    auto first()       ->       I& { return base::first; }
    auto first() const -> const I& { return base::first; }
    auto last ()       ->       I& { return base::second; }
    auto last () const -> const I& { return base::second; }
  private:
    template<class I0> friend class interval;
    auto as_base()       ->       base& { return *this; }
    auto as_base() const -> const base& { return *this; }
};

template<class I> interval(I first_, I last_) -> interval<I>;

template<class I> constexpr auto
operator==(const interval<I>& x, const interval<I>& y) -> bool {
  return x.first()==y.first() && x.last()==y.last();
}
template<class I> constexpr auto
operator!=(const interval<I>& x, const interval<I>& y) -> bool {
  return !(x==y);
}

template<class I> constexpr auto
length(interval<I> r) {
  return r.last() - r.first();
}
template<class I> constexpr auto
inf(interval<I> r) -> I {
  return r.first();
}
template<class I> constexpr auto
sup(interval<I> r) -> I {
  return r.last();
}


template<class I> constexpr auto
is_in(I i, interval<I> r) -> bool {
  return (r.first() <= i) && (i < r.last());
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
