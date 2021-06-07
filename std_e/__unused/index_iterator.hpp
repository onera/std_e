#pragma once


#include "std_e/iterator/iterator_range.hpp"
#include "std_e/future/contract.hpp"
#include <algorithm>
#include <iterator>
#include <functional>


namespace std_e {

/*
concept Index_iterator_traits
  has value_type: Regular
  has reference: Reference_of<value_type>

*/

template<class index_iterator_traits_type, class Rng, class Index_it>
// requires index_iterator_traits_type is an index_iterator_traits_type
// requires Rng is a random access collection
// requires Index_it is an Integer Iterator
class index_iterator {
  public:
  // traits
    using difference_type = typename std::iterator_traits<Index_it>::difference_type;
    using value_type = const typename Rng::value_type;
    using reference = typename index_iterator_traits_type::reference;
    using value_type = typename index_iterator_traits_type::value_type;
    using iterator_category = typename std::iterator_traits<Index_it>::iterator_category;

  // ctors
    constexpr
    index_iterator(Rng& c, Index_it idx_it)
      : rng_first(&c)
      , idx_it(idx_it)
    {}

    constexpr auto
    operator*() const -> reference {
      auto rng_pos = rng_first + *idx_it;
      return reference(rng_pos,idx_it);
    }

    constexpr auto operator++() -> index_iterator& { ++idx_it; return *this; }
    constexpr auto operator--() -> index_iterator& { --idx_it; return *this; }
    template<class I> constexpr auto operator+=(I i) -> index_iterator& { idx_it += i; return *this; }
    template<class I> constexpr auto operator-=(I i) -> index_iterator& { idx_it -= i; return *this; }

  // comparison
    friend constexpr auto
    operator==(const index_iterator& x, const index_iterator& y) -> bool;

    friend constexpr auto
    operator< (const index_iterator& x, const index_iterator& y) -> bool;
  private:
    Rng_it rng_first;
    Index_it idx_it;
};

template<class Rng, class It> constexpr auto
operator==(const index_iterator<Rng,It>& x, const index_iterator<Rng,It>& y) -> bool {
  return x.rng_first==y.rng_first && x.idx_it==y.idx_it;
}
template<class Rng, class It> constexpr auto
operator!=(const index_iterator<Rng,It>& x, const index_iterator<Rng,It>& y) -> bool {
  return !(x==y);
}
template<class Rng, class It, class I> constexpr auto
operator+(const index_iterator<Rng,It>& x, I i) -> index_iterator<Rng,It> {
  index_iterator<Rng,It> res(x);
  res += i;
  return res;
}
template<class Rng, class It, class I> constexpr auto
operator-(const index_iterator<Rng,It>& x, I i) -> index_iterator<Rng,It> {
  return x + (-i);
}
template<class Rng, class It> constexpr auto
operator-(const index_iterator<Rng,It>& x, const index_iterator<Rng,It>& y) -> typename index_iterator<Rng,It>::difference_type {
  return x.idx_it - y.idx_it;
}
template<class Rng, class It> constexpr auto
operator<(const index_iterator<Rng,It>& x, const index_iterator<Rng,It>& y) -> typename index_iterator<Rng,It>::difference_type {
  return x.idx_it < y.idx_it;
}


template<
  class Value_type, Reference_type,
  class Rng, class Index_range,
  class iterator_type = typename Index_range::const_iterator,
  class index_iterator_type = index_iterator<Rng,iterator_type>
> constexpr auto
// requires Rng is a random access range
// requires Index_range is an integer range
make_indexed_range(Rng& c, const Index_range& indices) -> iterator_range<index_iterator_type> {
  return {
    index_iterator_type{c,begin(indices)},
    index_iterator_type{c,end(indices)}
  };
}


} // std_e
