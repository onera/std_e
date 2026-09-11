#pragma once

#include <array>
#include "std_e/contract/contract.hpp"


namespace std_e {


template <typename T, size_t N>
class static_vector {
  public:
    using value_type      = T;
    using iterator        = T*;
    using const_iterator  = const T*;

    using size_type       = size_t;
    using difference_type = ptrdiff_t;

    constexpr
    static_vector() = default;

    template<int N0>
      requires (N0 <= N)
    constexpr
    static_vector(const T(&a)[N0])
      : sz(N0)
    {
      std::copy_n(a, N0, arr.data());
    }

    constexpr size_type size() const     { return sz; }
    constexpr auto data (this auto&& self) { return self.arr.data(); }
    constexpr auto begin(this auto&& self) { return self.arr.data(); }
    constexpr auto end(this auto&& self)   { return self.arr.data() + self.sz; }

    constexpr auto
    operator[](this auto&& self, difference_type i) -> auto& {
      return self.arr[i];
    }

    constexpr auto
    emplace_back(T x) -> T& {
      STD_E_ASSERT_LVL0(sz < N);
      arr[sz] = std::move(x);
      return arr[sz++];
    }

  private:
    size_type sz = 0;
    std::array<T, N> arr = {};
};


} // std_e
