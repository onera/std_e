#pragma once


/// Future (C++23 ?) (should be replaced by std::vector)
/// This file implements a vector compatible with constexpr


#include "std_e/base/index_t.hpp"
#include "std_e/future/algorithm.hpp"
#include "std_e/future/contract.hpp"
#include <array>


namespace std_e {


template<class T, size_t N>
class constexpr_vector {
  public:
    using value_type = T;
    using iterator = T*;
    using difference_type = int;
    static constexpr size_t max_size = N;

  // ctors
    FORCE_INLINE constexpr
    constexpr_vector()
      : sz(0)
      , elts{T{}}
    {}

    FORCE_INLINE explicit constexpr
    constexpr_vector(size_t sz)
      : sz(sz)
      , elts{T{}}
    {
      STD_E_ASSERT(sz<=max_size);
    }

    template<class Iter> constexpr
    constexpr_vector(Iter first, size_t sz)
      : sz(sz)
      , elts{}
    {
      STD_E_ASSERT(sz<=max_size);
      std_e::copy_n(first,sz,elts.begin());
    }

    constexpr
    constexpr_vector(std::initializer_list<T> init)
      : sz(init.size())
      , elts{}
    {
      STD_E_ASSERT(sz<=max_size);
      std_e::copy_n(init.begin(),sz,elts.begin());
    }

  // size
    FORCE_INLINE constexpr auto
    size() const -> size_t {
      return sz;
    }
    FORCE_INLINE constexpr auto
    resize(size_t sz) -> void {
      STD_E_ASSERT(sz<=max_size);
      this->sz = sz;
    }
    FORCE_INLINE constexpr auto
    empty() const -> bool {
      return size()==0;
    }

  // accessors
    FORCE_INLINE constexpr auto
    operator[](index_t i) const -> const T& {
      return elts[i];
    }
    FORCE_INLINE constexpr auto
    operator[](index_t i) -> T& {
      return elts[i];
    }
    FORCE_INLINE constexpr auto
    data() const -> const T* {
      return elts.data();
    }
    FORCE_INLINE constexpr auto
    data() -> T* {
      return elts.data();
    }

    FORCE_INLINE constexpr auto
    begin() -> T* {
      return elts.data();
    }
    FORCE_INLINE constexpr auto
    begin() const -> const T* {
      return elts.data();
    }
    FORCE_INLINE constexpr auto
    end() -> T* {
      return elts.data() + sz;
    }
    FORCE_INLINE constexpr auto
    end() const -> const T* {
      return elts.data() + sz;
    }
  // mutators
    FORCE_INLINE constexpr auto
    push_back(const T& x) -> void {
      STD_E_ASSERT(sz+1<max_size);
      elts[sz] = x;
      ++sz;
    }
    FORCE_INLINE constexpr auto
    pop_back() -> void {
      --sz;
    }

    FORCE_INLINE constexpr auto
    back() const -> const T& {
      return elts[sz-1];
    }
    FORCE_INLINE constexpr auto
    back() -> T& {
      return elts[sz-1];
    }

  private:
    size_t sz;
    std::array<T,max_size> elts;
};


template<class T, size_t N> FORCE_INLINE constexpr auto
begin(constexpr_vector<T,N>& x) -> T* {
  return x.begin();
}
template<class T, size_t N> FORCE_INLINE constexpr auto
begin(const constexpr_vector<T,N>& x) -> const T* {
  return x.begin();
}
template<class T, size_t N> FORCE_INLINE constexpr auto
end(constexpr_vector<T,N>& x) -> T* {
  return x.end();
}
template<class T, size_t N> FORCE_INLINE constexpr auto
end(const constexpr_vector<T,N>& x) -> const T* {
  return x.end();
}


// comparison {
template<class T, size_t N> constexpr auto
operator==(const constexpr_vector<T,N>& x, const constexpr_vector<T,N>& y) -> bool {
  if (x.size() != y.size()) return false;
  return equal(begin(x),end(x),begin(y));
}
template<class T, size_t N> constexpr auto
operator!=(const constexpr_vector<T,N>& x, const constexpr_vector<T,N>& y) -> bool {
  return !(x==y);
}

template<class T, size_t N> constexpr auto
operator<(const constexpr_vector<T,N>& x, const constexpr_vector<T,N>& y) -> bool {
  return lexicographical_compare(begin(x),end(x), begin(y),end(y));
}
// comparison }


/// C++ < C++20 hack {
/// (give linkage to constexpr sub-object so it can be passed as template reference)
template<const auto& x, index_t i>
// requires decltype(t)==constexpr_tree<T>
struct get_element__with_linkage {
  static constexpr auto value = x[i];
};
/// C++ < C++20 hack }

} // std_e
