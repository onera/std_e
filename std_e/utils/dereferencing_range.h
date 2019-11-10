#pragma once


#include <iterator>


namespace std_e {


template<class iterator_over_iterators>
class derefencing_iterator {
  public:
  // type traits
    using iterator_value = typename std::iterator_traits<iterator_over_iterators>::value_type;

    using value_type = typename std::iterator_traits<iterator_value>::value_type;
    using reference = typename std::iterator_traits<iterator_value>::reference;
    using iterator = derefencing_iterator;

    using difference_type = typename std::iterator_traits<iterator_over_iterators>::difference_type;
    using iterator_category = typename std::iterator_traits<iterator_over_iterators>::iterator_category;
  // ctor
    constexpr
    derefencing_iterator() = default;
    constexpr
    derefencing_iterator(iterator_over_iterators it_of_its)
      : it_of_its(it_of_its)
    {}

  // iterator interface
    constexpr auto
    operator++() -> derefencing_iterator& {
      ++it_of_its;
      return *this;
    }
    constexpr auto
    operator--() -> derefencing_iterator& {
      --it_of_its;
      return *this;
    }
    template<class Integer> constexpr auto
    operator+=(Integer i) -> derefencing_iterator& {
      it_of_its += i;
      return *this;
    }
    template<class Integer> constexpr auto
    operator-=(Integer i) -> derefencing_iterator& {
      it_of_its -= i;
      return *this;
    }

    constexpr auto
    operator*() -> decltype(auto) {
      return **it_of_its;
    }
    constexpr auto
    operator*() const -> decltype(auto) {
      return **it_of_its;
    }

    friend constexpr auto
    operator==(const derefencing_iterator& x, const derefencing_iterator& y) {
      return x.it_of_its == y.it_of_its;
    }
    friend constexpr auto
    operator!=(const derefencing_iterator& x, const derefencing_iterator& y) {
      return !(x==y);
    }
    friend constexpr auto
    operator-(const derefencing_iterator& x, const derefencing_iterator& y) -> difference_type {
      return x.it_of_its - y.it_of_its;
    }
  private:
    iterator_over_iterators it_of_its;
};



} // std_e
namespace std {
  template<class II>
  struct iterator_traits<std_e::derefencing_iterator<II>> {
    using type = std_e::derefencing_iterator<II>;

    using value_type = typename type::value_type;
    using iterator = typename type::iterator;
    using reference = typename type::reference;

    using difference_type = typename type::difference_type;
    using iterator_category = typename type::iterator_category;
  };
} // std
namespace std_e {


// Goal: view  Range<Iterator>  as  Range<Iterator::reference>


// TODO
//template<class Range_of_Iterators>
//struct iterator_type_of_Range_of_Iterators {
//  using type = typename Range_of_Iterators::iterator;
//};
//template<class Range_of_Iterators>
//struct iterator_type_of_Range_of_Iterators<const Range_of_Iterators> {
//  using type = const typename Range_of_Iterators::iterator;
//};

template<class Range_of_Iterators>
class dereferencing_range {
  public:
  // traits
    using iterator_over_iterators = typename Range_of_Iterators::iterator;
    //using iterator_over_iterators = typename iterator_type_of_Range_of_Iterators<Range_of_Iterators>::type;
    using iterator = derefencing_iterator<iterator_over_iterators>;

    //using value_type = typename std::iterator_traits<Iterator>::value_type;
    //using iterator = Iterator;
    //using reference = typename std::iterator_traits<Iterator>::reference;

  // ctors
    constexpr
    dereferencing_range() = default;
    constexpr
    dereferencing_range(Range_of_Iterators& x)
      : first({x.begin()})
      , last({x.end()})
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

    //template<class Integer> constexpr auto 
    //operator[](Integer i) -> reference {
    //  auto it = begin();
    //  std::advance(it,i);
    //  return *it;
    //}
    //template<class Integer> constexpr auto
    //operator[](Integer i) const -> const reference {
    //  auto it = begin();
    //  std::advance(it,i);
    //  return *it;
    //}

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


template<class Range_of_Iterators> constexpr auto
make_dereferencing_range(const Range_of_Iterators& x) {
  return dereferencing_range<const Range_of_Iterators>(x);
}
template<class Range_of_Iterators> constexpr auto
make_dereferencing_range(Range_of_Iterators& x) {
  return dereferencing_range<Range_of_Iterators>(x);
}


} // std_e
