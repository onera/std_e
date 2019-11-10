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
  begin() -> Iterator& {
    return first;
  }
  constexpr auto
  end() const -> Iterator {
    return last;
  }
  constexpr auto
  end() -> Iterator& {
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

template<class Iterator0, class Iterator1> constexpr auto
// requires Iterator0 and Iterator1 are comparable
operator==(const iterator_range<Iterator0>& x, const iterator_range<Iterator1>& y) {
  return x.first==y.first && x.last==y.last;
}
template<class Iterator0, class Iterator1> constexpr auto
operator!=(const iterator_range<Iterator0>& x, const iterator_range<Iterator1>& y) {
  return !(x==y);
}


template<class Iterator> constexpr auto
begin(const iterator_range<Iterator>& x) {
  return x.begin();
}
template<class Iterator> constexpr auto
end(const iterator_range<Iterator>& x) {
  return x.end();
}
template<class Iterator> constexpr auto
begin(iterator_range<Iterator>& x) {
  return x.begin();
}
template<class Iterator> constexpr auto
end(iterator_range<Iterator>& x) {
  return x.end();
}


template<class Iterator> constexpr auto
make_iterator_range(Iterator f, Iterator l) -> iterator_range<Iterator> {
  return {f,l};
}


} // std_e
