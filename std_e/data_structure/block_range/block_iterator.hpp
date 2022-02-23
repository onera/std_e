#pragma once


#include <type_traits>
#include "std_e/future/array.hpp"


// Customization point for std::common_reference
// SEE for https://en.cppreference.com/w/cpp/types/common_reference
template<class T, ptrdiff_t N,  template<class> class TQual0, template<class> class TQual1>
struct std::basic_common_reference< std_e::array<T,N> , std_e::span_ref<T,N> , TQual0,TQual1 > {
  using type = std_e::span_ref<T,N>;
};
template<class T, ptrdiff_t N,  template<class> class TQual1, template<class> class TQual0>
struct std::basic_common_reference< std_e::span_ref<T,N> , std_e::array<T,N> , TQual1,TQual0 > {
  using type = std_e::span_ref<T,N>;
};


namespace std_e {


template<class T, int N>
class block_iterator {
  public:
  // type traits
    using scalar_type = T;
    using index_type = int; // TODO param I or ptrdiff or size

    /// std::iterator type traits
    using value_type = std_e::array<T,N>; // we are using std_e::array (instead of std::) for construction from the reference type
    using reference = std_e::span_ref<T,N>;
    using difference_type = index_type;
    using iterator_category = std::random_access_iterator_tag;

  // static methods
    static constexpr auto
    block_size() -> int {
      return N;
    }

  // ctors
    block_iterator() = default;
    block_iterator(T* ptr)
      : ptr(ptr)
    {}

    auto operator++() -> block_iterator& {
      ptr += N;
      return *this;
    }
    auto operator--() -> block_iterator& {
      ptr -= N;
      return *this;
    }
    auto operator++(int) -> block_iterator {
      block_iterator tmp(*this);
      ++*this;
      return tmp;
    }
    auto operator--(int) -> block_iterator {
      block_iterator tmp(*this);
      --*this;
      return tmp;
    }

    auto operator+=(index_type i) -> block_iterator& {
      ptr += N*i;
      return *this;
    }
    auto operator-=(index_type i) -> block_iterator& {
      ptr -= N*i;
      return *this;
    }
    friend
    auto operator+(block_iterator x, index_type i) -> block_iterator {
      block_iterator res = x;
      res += i;
      return res;
    }
    friend
    auto operator+(index_type i, block_iterator x) -> block_iterator {
      return x+i;
    }
    friend
    auto operator-(block_iterator x, index_type i) -> block_iterator {
      block_iterator res = x;
      res -= i;
      return res;
    }
    friend
    auto operator-(block_iterator x, block_iterator y) -> difference_type {
      return (x.ptr-y.ptr)/N;
    }

    auto operator*() const {
      return reference(ptr);
    }
    auto
    operator[](index_type i) const -> reference {
      return reference(ptr+i);
    }

    auto data() const -> T* {
      return ptr;
    }

    constexpr auto
    operator<=>(const block_iterator& x) const = default;
  private:
    T* ptr;
};


} // std_e


template<class T, int N>
struct std::iterator_traits<std_e::block_iterator<T,N>> {
  using type = std_e::block_iterator<T,N>;
  using value_type = typename type::value_type;
  using reference = typename type::reference;
  using difference_type = typename type::difference_type;
  using iterator_category = typename type::iterator_category;
};
