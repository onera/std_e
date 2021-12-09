#pragma once


#include <type_traits>
#include <array>
#include "std_e/future/span.hpp"


namespace std_e {


template<
  class T, int N,
  class val_type = std::array<T,N>,
  class ref_type = std_e::span_ref<T,N>
>
class block_iterator {
  public:
  // type traits
    using scalar_type = T;
    using index_type = int;

    static constexpr auto
    block_size() -> int {
      return N;
    }

    /// std::iterator type traits
    using value_type = val_type;
    using reference = ref_type;
    using difference_type = index_type;
    using iterator_category = std::random_access_iterator_tag;

    block_iterator() = default;
    block_iterator(T* ptr)
      : ptr(ptr)
    {}

    auto operator++() -> block_iterator& {
      ptr += N;
      return *this;
    }
    auto operator++(int) -> block_iterator {
      block_iterator tmp(*this);
      ++*this;
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
    auto operator+(block_iterator it, index_type i) -> block_iterator {
      block_iterator res = it;
      res += i;
      return res;
    }
    friend
    auto operator-(block_iterator it, index_type i) -> block_iterator {
      block_iterator res = it;
      res -= i;
      return res;
    }

    auto operator*() const {
      return reference(ptr);
    }

    auto data() const -> T* {
      return ptr;
    }

    friend constexpr auto
    operator==(const block_iterator& it0, const block_iterator& it1) {
      return it0.ptr == it1.ptr;
    }
    friend constexpr auto
    operator!=(const block_iterator& it0, const block_iterator& it1) {
      return !(it0 == it1);
    }
  private:
    T* ptr;
};


} // std_e


template<class T, int N, class VT, class RT>
struct std::iterator_traits<std_e::block_iterator<T,N,VT,RT>> {
  using type = std_e::block_iterator<T,N,VT,RT>;
  using value_type = typename type::value_type;
  using reference = typename type::reference;
  using difference_type = typename type::difference_type;
  using iterator_category = typename type::iterator_category;
};
