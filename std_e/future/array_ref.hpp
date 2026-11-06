#pragma once


#include <cstdint>
#include <array>
#include <iterator>
#include "std_e/contract/contract.hpp"


namespace std_e {


template<class Tpp> // T**, but potentially const
class array_ref_iterator {
  public:
    using index_type = ptrdiff_t;
    using T = std::remove_pointer_t<std::remove_pointer_t<Tpp>>;
    using value_type = T;
    using reference = T&;
    using difference_type = index_type;
    using iterator_category = std::random_access_iterator_tag;

    array_ref_iterator() = default;
    array_ref_iterator(Tpp ptr)
      : ptr(ptr)
    {}

    auto
    operator++() -> array_ref_iterator& {
      ++ptr;
      return *this;
    }
    auto
    operator--() -> array_ref_iterator& {
      --ptr;
      return *this;
    }

    auto
    operator++(int) -> array_ref_iterator {
      array_ref_iterator tmp(*this);
      ++*this;
      return tmp;
    }
    auto
    operator--(int) -> array_ref_iterator {
      array_ref_iterator tmp(*this);
      --*this;
      return tmp;

    }

    auto
    operator+=(index_type i) -> array_ref_iterator& {
      ptr += i;
      return *this;
    }
    auto
    operator-=(index_type i) -> array_ref_iterator& {
      ptr -= i;
      return *this;
    }
    friend auto
    operator+(array_ref_iterator x, index_type i) -> array_ref_iterator {
      array_ref_iterator res = x;
      res += i;
      return res;
    }
    friend auto
    operator+(index_type i, array_ref_iterator x) -> array_ref_iterator {
      return x+i;
    }
    friend auto
    operator-(array_ref_iterator x, index_type i) -> array_ref_iterator {
      array_ref_iterator res = x;
      res -= i;
      return res;
    }
    friend auto
    operator-(array_ref_iterator x, array_ref_iterator y) -> difference_type {
      return x.ptr - y.ptr;
    }

    auto
    operator*() const -> reference {
      return **ptr;
    }
    auto
    operator[](index_type i) const -> reference {
      return **(ptr+i);
    }

  // comparisons
    auto operator<=>(const array_ref_iterator& x) const = default;
  private:
    Tpp ptr;
};


template<class T, size_t N>
class array_ref {
  public:
    using value_type = T&;
    using reference = T&;
    using const_reference = const T&;
    using iterator       = array_ref_iterator<      T>;
    using const_iterator = array_ref_iterator<const T>;
    using index_type = ptrdiff_t;

    static constexpr size_t size() { return N; }

    template<class... T0s>
    array_ref(T0s&... xs)
      : _impl{&xs...}
    {}

    auto operator[](this auto& self, index_type i) -> auto& { return *(self._impl[i]); }

    template<class Rng>
    auto operator=(const Rng& x) -> array_ref& {
      STD_E_ASSERT_LVL0(x.size() == N);
      for (int i=0; i<N; ++i) {
        *(_impl[i]) = x[i];
      }
      return *this;
    }

    auto begin(this auto& self) { return array_ref_iterator{self._impl.data()}; }
    auto end  (this auto& self) { return array_ref_iterator{self._impl.data() + N}; }

    template<std::ranges::range Rng>
      //requires (std::is_same_v<typename Rng::value_type,T>)
    auto operator<=>(const Rng& x) const {
      auto comp = [](const auto& x0, const auto& x1){ return (*x0) <=> x1; };
      return std::lexicographical_compare_three_way(
        this->_impl.data(),
        this->_impl.data()+N,
        x.begin(),
        x.begin()+x.size(),
        comp
      );
    }

    template<std::ranges::range Rng>
      //requires (std::is_same_v<typename Rng::value_type,T>)
    auto operator==(const Rng& x) const {
      auto comp = [](const auto& x0, const auto& x1){ return (*x0) == x1; };
      return std::equal(
        this->_impl.data(),
        this->_impl.data()+N,
        x.begin(),
        x.begin()+x.size(),
        comp
      );
    }
  private:
    std::array<T*,N> _impl;
};


template<size_t I, class T, size_t N> constexpr auto
get(array_ref<T,N>& x) -> T& {
  return x[I];
}
template<size_t I, class T, size_t N> constexpr auto
get(const array_ref<T,N>& x) -> const T& {
  return x[I];
}
template<size_t I, class T, size_t N> constexpr auto
get(array_ref<T,N>&& x) -> T&& {
  return std::move(x[I]);
}
template<size_t I, class T, size_t N> constexpr auto
get(const array_ref<T,N>&& x) -> const T&& {
  return std::move(x[I]);
}

} // std_e

namespace std {
  template<class T, size_t N>
  struct tuple_size<std_e::array_ref<T,N>> {
    static constexpr size_t value = N;
  };
  template<size_t Index, class T, size_t N>
  struct tuple_element<Index, std_e::array_ref<T,N>> {
    using type = T;
  };
}
