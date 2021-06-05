#pragma once


#include <iterator>


namespace std_e {


template<class Iterator>
struct iterator_range { // rename iter_span
// traits
  using value_type = typename std::iterator_traits<Iterator>::value_type;
  using iterator = Iterator;
  using reference = typename std::iterator_traits<Iterator>::reference;

// data members
  Iterator first;
  Iterator last;

// iteration
  constexpr auto
  begin() const -> Iterator {
    return first;
  }
  constexpr auto
  end() const -> Iterator {
    return last;
  }

// range interface
  constexpr auto
  size() const {
    return std::distance(begin(),end());
  }

  template<class Integer> constexpr auto
  operator[](Integer i) -> reference {
    auto it = begin();
    std::advance(it,i);
    return *it;
  }
  template<class Integer> constexpr auto
  operator[](Integer i) const -> const reference {
    auto it = begin();
    std::advance(it,i);
    return *it;
  }
};

template<class It0, class It1> constexpr auto
// requires It0 and It1 are comparable
operator==(const iterator_range<It0>& x, const iterator_range<It1>& y) {
  return x.first==y.first && x.last==y.last;
}
template<class It0, class It1> constexpr auto
operator!=(const iterator_range<It0>& x, const iterator_range<It1>& y) {
  return !(x==y);
}


template<class It> constexpr auto
begin(const iterator_range<It>& x) {
  return x.begin();
}
template<class It> constexpr auto
end(const iterator_range<It>& x) {
  return x.end();
}
template<class It> constexpr auto
begin(iterator_range<It>& x) {
  return x.begin();
}
template<class It> constexpr auto
end(iterator_range<It>& x) {
  return x.end();
}


template<class Range> constexpr auto
to_iterator_range(Range& rng) {
  using iterator_range_type = std_e::iterator_range<typename Range::iterator>;
  return iterator_range_type{rng.begin(),rng.end()};
}


} // std_e
