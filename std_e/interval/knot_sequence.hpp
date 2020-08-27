#pragma once


#include "std_e/interval/interval.hpp"
#include "std_e/future/span.hpp"
#include <vector>
#include <numeric>
#include "std_e/future/algorithm.hpp"
#include "std_e/future/contract.hpp"


namespace std_e {

// fwd decls
template<class Number> constexpr auto
to_knot_vector(std::vector<Number> v);

/**
concept Knot_sequence : Random_access_range, Interval
  value_type is Number
  ordered sequence

  nb_intervals() -> Integer
  interval() -> interval<value_type>
  interval(i) -> interval<value_type>

  length(i) -> value_type
  inf(i) -> value_type
  sup(i) -> value_type
*/

template<class Random_access_range>
class knot_sequence : private Random_access_range {
  public:
  // class invariants:
  //  - ordered
    using base = Random_access_range;
    using this_type = knot_sequence<Random_access_range>;
    using value_type = typename base::value_type;

  // ctors
    knot_sequence() = default;

    knot_sequence(value_type n)
      : base(n+1)
    {}
    
    knot_sequence(value_type* first, value_type n)
      : base(first,first+n+1)
    {}

    knot_sequence(std::initializer_list<value_type> l)
      : base(l)
    {}

  // Knot_sequence interface
    using base::size;
    using base::begin;
    using base::end;
    using base::data;
    using base::operator[];
    using base::back;

    auto nb_intervals() const -> int {
      return this->size()-1;
    }

    auto inf() const -> int {
      return (*this)[0];
    };
    auto inf(int i) const -> int {
      STD_E_ASSERT(i<nb_intervals());
      return (*this)[i];
    };

    auto sup() const -> int {
      return back();
    };
    auto sup(int i) const -> int {
      STD_E_ASSERT(i<nb_intervals());
      return (*this)[i+1];
    };

    auto length() const -> int {
      return back() - (*this)[0];
    };
    auto length(int i) const -> int {
      STD_E_ASSERT(i<nb_intervals());
      return (*this)[i+1] - (*this)[i];
    };

    auto interval() const -> std_e::interval<value_type> {
      return {inf(),sup()};
    }
    auto interval(int i) const -> std_e::interval<value_type> {
      return {inf(i),sup(i)};
    }

    auto push_back(value_type x) -> value_type& {
      STD_E_ASSERT(size()==0 || back()<x);
      base::push_back(x);
      return back();
    };

  // utility
    auto as_base() const -> const base& {
      return *this;
    }
  private:
    knot_sequence(std::vector<value_type> v)
      : base(std::move(v))
    {}
    constexpr auto friend to_knot_vector<>(std::vector<value_type> v);
};

template<class Rng> constexpr auto
operator==(const knot_sequence<Rng>& x, const knot_sequence<Rng>& y) {
  return x.as_base() == y.as_base();
}
template<class Rng> constexpr auto
operator!=(const knot_sequence<Rng>& x, const knot_sequence<Rng>& y) {
  return !(x==y);
}


template<class Number> using knot_vector = knot_sequence<std::vector<Number>>;
template<class Number> using knot_span = knot_sequence<std_e::span<Number>>;

using int_knot_vector = knot_vector<int>;
using int_knot_span = knot_span<int>;


template<class Number> constexpr auto
to_knot_span(span<Number> s) {
  STD_E_ASSERT(std::is_sorted(begin(s),end(s)));
  return knot_span<Number>(s.data(),s.size()-1);
}
template<class Number> constexpr auto
to_knot_vector(std::vector<Number> v) {
  STD_E_ASSERT(std::is_sorted(begin(v),end(v)));
  return knot_vector<Number>(std::move(v));
}


// algorithms {
template<class Knot_sequence, class T = typename Knot_sequence::value_type> constexpr auto
interval_lengths(const Knot_sequence& ks) -> std::vector<T> {
  std::vector<T> res(ks.nb_intervals());
  std::adjacent_difference(begin(ks)+1,end(ks),begin(res));
  res[0] = ks.length(0);
  return res;
}
template<class Random_access_range, class T = typename Random_access_range::value_type> constexpr auto
offsets_from_sizes(const Random_access_range& r) -> knot_vector<T> {
  knot_vector<int> offsets(r.size());
  offsets[0] = 0;
  std_e::inclusive_scan(begin(r),end(r),begin(offsets)+1);
  return offsets;
}
template<class Number, class Knot_sequence> auto 
interval_index(Number x, const Knot_sequence& ks) {
  auto it = std::upper_bound(begin(ks),end(ks),x);
  return it-begin(ks)-1;
}
// algorithms }


} // std_e
