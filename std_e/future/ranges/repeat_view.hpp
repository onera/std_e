#pragma once


#include <ranges>


namespace std_e {
namespace views {


template<class T>
// requires order_functor_type has method increment(Multi_index_1& indices)
class repeat_view_iterator {
  private:
    T val;
    size_t i;
  public:
  // std::iterator type traits
    using value_type = T;
    using difference_type = ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

  // ctor
    constexpr
    repeat_view_iterator(T x, size_t i)
      : val(std::move(x))
      , i(i)
    {}

  // iterator interface
    constexpr auto
    operator++() -> repeat_view_iterator& {
      ++i;
      return *this;
    }
    constexpr auto
    operator++(int) -> repeat_view_iterator {
      repeat_view_iterator res(*this);
      ++res;
      return res;
    }

    constexpr auto
    operator*() const -> T {
      return val;
    }

    friend constexpr auto
    operator==(const repeat_view_iterator& x, const repeat_view_iterator& y) {
      return x.i==y.i;
    }
    friend constexpr auto
    operator!=(const repeat_view_iterator& x, const repeat_view_iterator& y) {
      return !(x==y);
    }
};


template<class T>
class repeat_view : public std::ranges::view_base {
  private:
    T val;
    size_t n;
  public:
  // type traits
    using iterator = repeat_view_iterator<T>;
  // ctor
    constexpr
    repeat_view(T x, size_t n)
      : val(std::move(x))
      , n(n)
    {}


  // range interface
    constexpr auto
    size() const -> size_t {
      return n;
    }
    constexpr auto
    begin() const -> iterator {
      return {val,0};
    }
    constexpr auto
    end() const -> iterator {
      return {val,n};
    }
};


} // views
} // std_e
