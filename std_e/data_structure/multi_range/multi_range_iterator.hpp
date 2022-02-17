#pragma once


namespace std_e {


template<class T>
struct _mr_reference {
  using type = typename T::reference;
};
template<class T>
struct _mr_reference<const T> {
  using type = typename T::const_reference;
};

template<class T>
class multi_range_iterator {
  public:
    using index_type = typename T::index_type;
    /// std::iterator type traits
    using value_type = typename T::value_type;
    using reference = typename _mr_reference<T>::type;
    using difference_type = index_type;
    using iterator_category = std::random_access_iterator_tag;

    multi_range_iterator() = default;
    multi_range_iterator(T* ptr, index_type pos)
      : ptr(ptr)
      , pos(pos)
    {}

    auto
    operator++() -> multi_range_iterator& {
      ++pos;
      return *this;
    }
    auto
    operator--() -> multi_range_iterator& {
      --pos;
      return *this;
    }

    auto
    operator++(int) -> multi_range_iterator {
      multi_range_iterator tmp(*this);
      ++*this;
      return tmp;
    }
    auto
    operator--(int) -> multi_range_iterator {
      multi_range_iterator tmp(*this);
      --*this;
      return tmp;

    }

    auto
    operator+=(index_type i) -> multi_range_iterator& {
      pos += i;
      return *this;
    }
    auto
    operator-=(index_type i) -> multi_range_iterator& {
      pos -= i;
      return *this;
    }
    friend auto
    operator+(multi_range_iterator x, index_type i) -> multi_range_iterator {
      multi_range_iterator res = x;
      res += i;
      return res;
    }
    friend auto
    operator+(index_type i, multi_range_iterator x) -> multi_range_iterator {
      return x+i;
    }
    friend auto
    operator-(multi_range_iterator x, index_type i) -> multi_range_iterator {
      multi_range_iterator res = x;
      res -= i;
      return res;
    }
    friend auto
    operator-(multi_range_iterator x, multi_range_iterator y) -> difference_type {
      STD_E_ASSERT(x.ptr == y.ptr);
      return x.pos - y.pos;
    }

    auto
    operator*() const -> reference {
      return (*ptr)[pos];
    }
    auto
    operator[](index_type i) const -> reference {
      return (*ptr)[pos+i];
    }

    friend constexpr auto
    operator==(const multi_range_iterator& x, const multi_range_iterator& y) {
      STD_E_ASSERT(x.ptr == y.ptr);
      return x.pos == y.pos;
    }
    friend constexpr auto
    operator!=(const multi_range_iterator& x, const multi_range_iterator& y) {
      return !(x == y);
    }
    friend constexpr auto
    operator<(const multi_range_iterator& x, const multi_range_iterator& y) {
      STD_E_ASSERT(x.ptr == y.ptr);
      return x.pos < y.pos;
    }
    friend constexpr auto
    operator>(const multi_range_iterator& x, const multi_range_iterator& y) {
      return y<x;
    }
    friend constexpr auto
    operator<=(const multi_range_iterator& x, const multi_range_iterator& y) {
      return !(y<x);
    }
    friend constexpr auto
    operator>=(const multi_range_iterator& x, const multi_range_iterator& y) {
      return !(x<y);
    }
  private:
    T* ptr;
    index_type pos;
};


} // std_e


template<class T>
struct std::iterator_traits<std_e::multi_range_iterator<T>> {
  using type = std_e::multi_range_iterator<T>;
  using value_type = typename type::value_type;
  using reference = typename type::reference;
  using difference_type = typename type::difference_type;
  using iterator_category = typename type::iterator_category;
};
