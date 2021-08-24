#pragma once


#include <clocale>
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
class interval_base {
  public:
    using value_type = std::decay_t<I>;

    interval_base() = default;

    interval_base(I f, I l)
      : f(f)
      , l(l)
    {}

    template<class I0>
    interval_base(const interval_base<I0,ik>& other)
      : f(other.first())
      , l(other.last())
    {}
    template<class I0>
    interval_base(interval_base<I0,ik>&& other)
      : f(other.first())
      , l(other.last())
    {}
    template<class I0> auto
    operator=(const interval_base<I0,ik>& other) -> interval_base& {
      f = other.first();
      l = other.last();
      return *this;
    }
    template<class I0> auto
    operator=(interval_base<I0,ik>&& other) -> interval_base& {
      f = other.first();
      l = other.last();
      return *this;
    }
    interval_base(const interval_base& other)
      : f(other.first())
      , l(other.last())
    {}
    interval_base(interval_base&& other)
      : f(other.first())
      , l(other.last())
    {}
    auto
    operator=(const interval_base& other) -> interval_base& {
      f = other.first();
      l = other.last();
      return *this;
    }
    auto
    operator=(interval_base&& other) -> interval_base& {
      f = other.first();
      l = other.last();
      return *this;
    }

    auto first()       ->       I& { return f; }
    auto first() const -> const I& { return f; }
    auto last ()       ->       I& { return l; }
    auto last () const -> const I& { return l; }
  private:
    I f;
    I l;
};

template<class I0, class I1, interval_kind ik> constexpr auto
operator==(const interval_base<I0,ik>& x, const interval_base<I1,ik>& y) -> bool {
  return x.first()==y.first() && x.last()==y.last();
}
template<class I0, class I1, interval_kind ik> constexpr auto
operator!=(const interval_base<I0,ik>& x, const interval_base<I1,ik>& y) -> bool {
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

    interval(I f, I l)
      : base(f,l)
    {
      STD_E_ASSERT(f<=l);
    }
};
template<class I>
class closed_interval : public interval_base<I,interval_kind::closed> {
  public:
    using base = interval_base<I,interval_kind::closed>;
    using base::base;

    closed_interval(I f, I l)
      : base(f,l)
    {
      STD_E_ASSERT(f-1<=l); // so that empty intervals can be represented by [i,i-1]
    }
};

template<class I>        interval(I first, I last) ->        interval<I>;
template<class I> closed_interval(I first, I last) -> closed_interval<I>;

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
