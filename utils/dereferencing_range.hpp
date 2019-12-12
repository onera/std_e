#pragma once


#include <iterator>


namespace std_e {


template<class iterator_over_iterators>
class dereferencing_iterator {
  public:
  // type traits
    using iterator_value = typename std::iterator_traits<iterator_over_iterators>::value_type;

    using value_type = typename std::iterator_traits<iterator_value>::value_type;
    using reference = typename std::iterator_traits<iterator_value>::reference;
    //using const_reference = const reference;
    using iterator = dereferencing_iterator;

    using difference_type = typename std::iterator_traits<iterator_over_iterators>::difference_type;
    using iterator_category = typename std::iterator_traits<iterator_over_iterators>::iterator_category;
  // ctor
    constexpr
    dereferencing_iterator() = default;
    constexpr
    dereferencing_iterator(iterator_over_iterators it_of_its)
      : it_of_its(it_of_its)
    {}

  // iterator interface
    constexpr auto
    operator++() -> dereferencing_iterator& {
      ++it_of_its;
      return *this;
    }
    constexpr auto
    operator--() -> dereferencing_iterator& {
      --it_of_its;
      return *this;
    }
    template<class Integer> constexpr auto
    operator+=(Integer i) -> dereferencing_iterator& {
      it_of_its += i;
      return *this;
    }
    template<class Integer> constexpr auto
    operator-=(Integer i) -> dereferencing_iterator& {
      it_of_its -= i;
      return *this;
    }

    constexpr auto
    operator*() -> const reference {
      return **it_of_its;
    }
    constexpr auto
    operator*() const -> const reference {
      return **it_of_its;
    }

    friend constexpr auto
    operator==(const dereferencing_iterator& x, const dereferencing_iterator& y) {
      return x.it_of_its == y.it_of_its;
    }
    friend constexpr auto
    operator!=(const dereferencing_iterator& x, const dereferencing_iterator& y) {
      return !(x==y);
    }
    friend constexpr auto
    operator-(const dereferencing_iterator& x, const dereferencing_iterator& y) -> difference_type {
      return x.it_of_its - y.it_of_its;
    }
  private:
    iterator_over_iterators it_of_its;
};



} // std_e
namespace std {
  template<class II>
  struct iterator_traits<std_e::dereferencing_iterator<II>> {
    using type = std_e::dereferencing_iterator<II>;

    using value_type = typename type::value_type;
    using iterator = typename type::iterator;
    using reference = typename type::reference;

    using difference_type = typename type::difference_type;
    using iterator_category = typename type::iterator_category;
  };
} // std
namespace std_e {


// Goal: view  Range<Iterator>  as  Range<Iterator::reference>
template<class iterator_over_iterators>
class dereferencing_range {
  public:
  // traits
    using iterator = dereferencing_iterator<iterator_over_iterators>;
    using value_type = typename std::iterator_traits<iterator>::value_type;
    using reference = typename std::iterator_traits<iterator>::reference;

  // ctors
    constexpr
    dereferencing_range() = default;

    constexpr
    dereferencing_range(iterator_over_iterators f, iterator_over_iterators l)
      : first({f})
      , last({l})
    {}

  // iteration
    constexpr auto
    begin() const -> iterator {
      return first;
    }
    constexpr auto
    begin() -> iterator& {
      return first;
    }
    constexpr auto
    end() const -> iterator {
      return last;
    }
    constexpr auto
    end() -> iterator& {
      return last;
    }

  // range interface
    constexpr auto
    size() const {
      return std::distance(begin(),end());
    }

    constexpr auto 
    operator[](int i) -> reference {
      auto it = begin();
      std::advance(it,i);
      return *it;
    }
    constexpr auto
    operator[](int i) const -> const reference {
      auto it = begin();
      std::advance(it,i);
      return *it;
    }

  private:
    iterator first;
    iterator last;
};

template<class R> constexpr auto
begin(const dereferencing_range<R>& x) {
  return x.begin();
}
template<class R> constexpr auto
end(const dereferencing_range<R>& x) {
  return x.end();
}

template<class R> constexpr auto
operator==(const dereferencing_range<R>& x, const dereferencing_range<R>& y) {
  return begin(x)==begin(y) && end(x)==end(y);
}
template<class R> constexpr auto
operator!=(const dereferencing_range<R>& x, const dereferencing_range<R>& y) {
  return !(x==y);
}

// Note: make_dereferencing_range as no non-const overload
// because it is not needed: even if the range is const,
// its elements (the iterators) are not
template<class Range_of_Iterators> constexpr auto
make_dereferencing_range(const Range_of_Iterators& x) {
  return dereferencing_range(begin(x),end(x));
}


} // std_e
