#pragma once

#include "std_e/interval/interval.hpp"
#include "std_e/future/span.hpp"
#include <vector>
#include <numeric>
#include "std_e/future/algorithm.hpp"
#include "std_e/future/contract.hpp"


namespace std_e {

// fwd decls
template<class Number> auto
to_interval_vector(std::vector<Number> v);

/**
concept Interval_sequence : Random_access_range, Interval
  value_type is Number
  ordered sequence

  n_interval() -> Integer
  interval() -> interval<value_type>
  interval(i) -> interval<value_type>

  length(i) -> value_type
  inf(i) -> value_type
  sup(i) -> value_type
*/

template<class Random_access_range>
class interval_sequence : private Random_access_range {
  public:
  // class invariants:
  //  - ordered
    using base = Random_access_range;
    using this_type = interval_sequence<Random_access_range>;
    using value_type = typename base::value_type;

  // ctors
    interval_sequence() = default;

    interval_sequence(int n)
      : base(n+1)
    {}
    interval_sequence(int n, value_type x)
      : base(n+1,x)
    {}

    template<class Iterator>
    interval_sequence(Iterator first, value_type n)
      : base(first,first+n+1)
    {}
    template<class Iterator>
    interval_sequence(Iterator first, Iterator last)
      : base(first,last)
    {}

    interval_sequence(std::initializer_list<value_type> l)
      : base(l)
    {}

  // Interval_sequence interface
    using base::size;
    using base::begin;
    using base::end;
    using base::data;
    using base::operator[];
    using base::back;

    auto n_interval() const -> int {
      return this->size()-1;
    }

    auto inf() const -> int {
      return (*this)[0];
    };
    auto inf(int i) const -> int {
      STD_E_ASSERT(i<n_interval());
      return (*this)[i];
    };

    auto sup() const -> int {
      return back();
    };
    auto sup(int i) const -> int {
      STD_E_ASSERT(i<n_interval());
      return (*this)[i+1];
    };

    auto length() const -> int {
      return back() - (*this)[0];
    };
    auto length(int i) const -> int {
      STD_E_ASSERT(i<n_interval());
      return (*this)[i+1] - (*this)[i];
    };

    auto interval() const -> std_e::interval<value_type> {
      return {inf(),sup()};
    }
    auto interval(int i) const -> std_e::interval<value_type> {
      return {inf(i),sup(i)};
    }

    auto push_back(value_type x) -> value_type& {
      STD_E_ASSERT(size()==0 || back()<=x);
      base::push_back(x);
      return back();
    };
    auto push_back_length(value_type x) -> value_type& {
      STD_E_ASSERT(size()>0);
      push_back( back() + x );
      return back();
    };

  // utility
    auto as_base() const -> const base& {
      return *this;
    }
  private:
    // Compiler hack around too eager template class non-template member instantiation
    // SEE https://stackoverflow.com/q/63810583/1583122
    template<class T0, std::enable_if_t<std::is_same_v<T0,std::vector<value_type>>,int> =0>
    interval_sequence(T0 v)
      : base(std::move(v))
    {}
    constexpr auto friend to_interval_vector<>(std::vector<value_type> v);
};

template<class Rng0, class Rng1> constexpr auto
operator==(const interval_sequence<Rng0>& x, const interval_sequence<Rng1>& y) {
  return x.as_base() == y.as_base();
}
template<class Rng0, class Rng1> constexpr auto
operator!=(const interval_sequence<Rng0>& x, const interval_sequence<Rng1>& y) {
  return !(x==y);
}
template<class Rng> auto
begin(const interval_sequence<Rng>& x) {
  return x.begin();
}
template<class Rng> auto
begin(interval_sequence<Rng>& x) {
  return x.begin();
}
template<class Rng> auto
end(const interval_sequence<Rng>& x) {
  return x.end();
}
template<class Rng> auto
end(interval_sequence<Rng>& x) {
  return x.end();
}
template<class Rng> auto
to_string(const interval_sequence<Rng>& x) -> std::string {
  return to_string(x.as_base());
}


template<class Number> using interval_vector = interval_sequence<std::vector<Number>>;
template<class Number> using interval_span = interval_sequence<std_e::span<Number>>;

// TODO deprecate
using int_interval_vector = interval_vector<int>;
using int_interval_span = interval_span<int>;

template<class Number> auto
to_interval_span(span<Number> s) {
  STD_E_ASSERT(std::is_sorted(begin(s),end(s)));
  return interval_span<Number>(s.data(),s.size()-1);
}
template<class Rng, class I = typename Rng::value_type> auto
to_interval_span(const interval_sequence<Rng>& x) -> interval_span<const I> {
  return interval_span<const I>(x.data(),x.data()+x.size());
}

template<class Number> auto
to_interval_vector(std::vector<Number> v) {
  STD_E_ASSERT(std::is_sorted(begin(v),end(v)));
  return interval_vector<Number>(std::move(v));
}


// algorithms {
template<class Interval_sequence, class T = std::remove_const_t<typename Interval_sequence::value_type>> constexpr auto
interval_lengths(const Interval_sequence& is) -> std::vector<T> {
  std::vector<T> res(is.n_interval());
  std::adjacent_difference(is.begin()+1,is.end(),begin(res));
  res[0] = is.length(0);
  return res;
}
template<class Random_access_range, class T = typename Random_access_range::value_type> auto
indices_from_strides(const Random_access_range& r) -> interval_vector<T> {
  interval_vector<T> indices(r.size());
  indices[0] = 0;
  std_e::inclusive_scan(begin(r),end(r),begin(indices)+1);
  return indices;
}
template<class Number, class Interval_sequence> auto
interval_index(Number x, const Interval_sequence& is) {
  auto it = std::upper_bound(begin(is),end(is),x);
  return it-begin(is)-1;
}
// algorithms }


} // std_e
