#pragma once


#include "std_e/utils/iterator_range.h"
#include "std_e/future/contract.h"
#include <algorithm>
#include <iterator>
#include <functional>


namespace std_e {


template <class C, class Integer_it>
class reference_wrapper2 {
  public:
  // types
    using T = const typename C::value_type;
   
  // ctors
    constexpr
    reference_wrapper2(C* c_ptr, Integer_it idx_ptr) noexcept
      : c_ptr(c_ptr)
      , idx_ptr(idx_ptr)
    {}

    constexpr reference_wrapper2(const reference_wrapper2&) noexcept = default;
    //constexpr reference_wrapper2(const reference_wrapper2&) noexcept = delete;

    constexpr auto
    operator=(const reference_wrapper2& old) noexcept -> reference_wrapper2& {
      std_e::ASSERT(c_ptr==old.c_ptr);
      *idx_ptr = *old.idx_ptr;
      return *this;
    }
   
  // access
    //constexpr
    //operator T& () const noexcept {
    //  return (*c_ptr)[*idx_ptr];
    //}
    constexpr auto
    get() const noexcept -> T& {
      return (*c_ptr)[*idx_ptr];
    }
  private:
    C* c_ptr;
    Integer_it idx_ptr;
};


template<class C, class Integer_it>
// requires C is a random access collection
// requires Integer_it is an Integer Iterator
class index_iterator {
  public:
  // traits
    using value_type = const typename C::value_type;
    //using value_type = std::reference_wrapper<value_type>; // TODO should be this, but needs DOC
    using reference = std::reference_wrapper<value_type>;
    using const_reference = std::reference_wrapper<const value_type>;
    using iterator = index_iterator<C,Integer_it>;
    using const_iterator = index_iterator<const C,const Integer_it>;

    using difference_type = typename std::iterator_traits<Integer_it>::difference_type;
    using iterator_category = typename std::iterator_traits<Integer_it>::iterator_category;

  // ctors
    constexpr
    index_iterator(C& c, Integer_it idx_it)
      : c_ptr(&c)
      , idx_it(idx_it)
    {}

    constexpr auto
    operator*() -> reference {
      return (*c_ptr)[*idx_it];
      //return {(*c_ptr)[*idx_it],*idx_it};
      //return {c_ptr,idx_it};
    }
    constexpr auto
    operator*() const -> const reference {
      return (*c_ptr)[*idx_it];
      //return {(*c_ptr)[*idx_it],*idx_it};
      //return {c_ptr,idx_it};
    }

    constexpr auto
    operator++() -> index_iterator& {
      ++idx_it;
      return *this;
    }
    constexpr auto
    operator--() -> index_iterator& {
      --idx_it;
      return *this;
    }
    constexpr auto
    operator+=(int i) -> index_iterator& {
      idx_it+=i;
      return *this;
    }
    constexpr auto
    operator-=(int i) -> index_iterator& {
      idx_it-=i;
      return *this;
    }

  // comparison
    template<class C_0, class It_0> friend constexpr auto
    operator==(const index_iterator<C_0,It_0>& x, const index_iterator<C_0,It_0>& y) -> bool;

    template<class C_0, class It_0> friend constexpr auto
    operator-(const index_iterator<C_0,It_0>& x, const index_iterator<C_0,It_0>& y) -> typename index_iterator<C_0,It_0>::difference_type;

    template<class C_0, class It_0> friend constexpr auto
    operator<(const index_iterator<C_0,It_0>& x, const index_iterator<C_0,It_0>& y) -> typename index_iterator<C_0,It_0>::difference_type;
  private:
    C* c_ptr;
    Integer_it idx_it;
};
} //std_e

template<class C, class Integer_it>
struct std::iterator_traits<std_e::index_iterator<C,Integer_it>> {
  using type = std_e::index_iterator<C,Integer_it>;
  using value_type = typename type::value_type;
  using reference = typename type::reference;
  using const_reference = typename type::const_reference;
  using iterator = typename type::iterator;
  using const_iterator = typename type::const_iterator;

  using difference_type = typename type::difference_type;
  using iterator_category = typename type::iterator_category;
};

namespace std_e {
template<class C, class It> constexpr auto
operator==(const index_iterator<C,It>& x, const index_iterator<C,It>& y) -> bool {
  return x.c_ptr==y.c_ptr && x.idx_it==y.idx_it;
}
template<class C, class It> constexpr auto
operator!=(const index_iterator<C,It>& x, const index_iterator<C,It>& y) -> bool {
  return !(x==y);
}
template<class C, class It> constexpr auto
operator+(const index_iterator<C,It>& x, int i) -> index_iterator<C,It> {
  index_iterator<C,It> res(x);
  res += i;
  return res;
}
template<class C, class It> constexpr auto
operator-(const index_iterator<C,It>& x, int i) -> index_iterator<C,It> {
  return x + (-i);
}
template<class C, class It> constexpr auto
operator-(const index_iterator<C,It>& x, const index_iterator<C,It>& y) -> typename index_iterator<C,It>::difference_type {
  return x.idx_it - y.idx_it;
}
template<class C, class It> constexpr auto
operator<(const index_iterator<C,It>& x, const index_iterator<C,It>& y) -> typename index_iterator<C,It>::difference_type {
  return x.idx_it < y.idx_it;
}


template<
  class C, class C_I,
  class iterator_type = typename C_I::const_iterator,
  class index_iterator_type = index_iterator<C,iterator_type>
> constexpr auto
// requires C is a random access collection
// requires C_I is an integer collection
make_indexed_range(C& c, const C_I& indices) -> iterator_range<index_iterator_type> {
  return {
    index_iterator_type{c,begin(indices)},
    index_iterator_type{c,end(indices)}
  };
}


} // std_e
