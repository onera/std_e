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

enum class interval_kind {
  closed_open,
  closed
};


// interval_base {
template<class I, interval_kind ik>
// requires I is integral type
class interval_base : private std::pair<I,I> {
  public:
    using value_type = std::decay_t<I>;
    using base = std::pair<I,I>;

    interval_base(I f, I l)
      : base(f,l)
    {
      STD_E_ASSERT(f<=l);
    }

    template<class I0>
    interval_base(const interval_base<I0,ik>& other)
      : base(other.as_base())
    {}
    template<class I0> auto
    operator=(const interval_base<I0,ik>& other) -> interval_base& {
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

template<class I, interval_kind ik> constexpr auto
operator==(const interval_base<I,ik>& x, const interval_base<I,ik>& y) -> bool {
  return x.first()==y.first() && x.last()==y.last();
}
template<class I, interval_kind ik> constexpr auto
operator!=(const interval_base<I,ik>& x, const interval_base<I,ik>& y) -> bool {
  return !(x==y);
}

template<class I, interval_kind ik> constexpr auto
inf(interval_base<I,ik> r) -> I {
  return r.first();
}
template<class I, interval_kind ik> constexpr auto
sup(interval_base<I,ik> r) -> I {
  return r.last();
}

// interval_base }


template<class I>
class interval : public interval_base<I,interval_kind::closed_open> {
  public:
    using base = interval_base<I,interval_kind::closed_open>;
    using base::base;
};
template<class I>
class closed_interval : public interval_base<I,interval_kind::closed> {
  public:
    using base = interval_base<I,interval_kind::closed>;
    using base::base;
};

template<class I>        interval(I first_, I last_) ->        interval<I>;
template<class I> closed_interval(I first_, I last_) -> closed_interval<I>;

template<class I> constexpr auto length(       interval<I> r) { return r.last() - r.first(); }
template<class I> constexpr auto length(closed_interval<I> r) { return r.last() - r.first() +1 ; }

template<class I> constexpr auto is_in(       interval<I> r, I i) -> bool { return (r.first() <= i) && (i <  r.last()); }
template<class I> constexpr auto is_in(closed_interval<I> r, I i) -> bool { return (r.first() <= i) && (i <= r.last()); }

template<class I> [[deprecated("switch arguments order")]] constexpr auto
is_in(I i, interval<I> r) -> bool {
  return is_in(r,i);
}

template<class I> constexpr auto
contiguous(const interval<I>& r0, const interval<I>& r1) -> bool {
  return r0.last() == r1.first();
}
template<class I> constexpr auto
contiguous(const closed_interval<I>& r0, const closed_interval<I>& r1) -> bool {
  return r0.last()+1 == r1.first();
}

template<class I> auto
to_closed_open(const closed_interval<I>& x) -> interval<I> {
  return {x.first(),x.last()+1};
}


} // std_e
