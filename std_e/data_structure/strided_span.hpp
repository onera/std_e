#pragma once


#include <algorithm>
#include <type_traits>
#include "std_e/base/dynamic_size.hpp"
#include "std_e/base/macros.hpp"


namespace std_e {

template<class I, int N>
struct stride {

  constexpr stride() {}
  constexpr stride(int) {}

  static FORCE_INLINE constexpr auto
  stride_length() -> int {
    return N;
  }

  friend constexpr auto
  operator==(stride, stride) -> bool {
    return true;
  }
  friend constexpr auto
  operator!=(stride x, stride y) -> bool {
    return !(x==y);
  }
};

template<class I>
class stride<I,dynamic_size> {
  public:
    FORCE_INLINE constexpr
    stride()
      : n(0)
    {}
    FORCE_INLINE constexpr
    stride(I n)
      : n(n)
    {}

    FORCE_INLINE constexpr auto
    stride_length() const -> I {
      return n;
    }

  // comparison
    friend constexpr auto
    operator==(stride x, stride y) -> bool {
      return x.n==y.n;
    }
    friend constexpr auto
    operator!=(stride x, stride y) -> bool {
      return !(x==y);
    }
  private:
    I n;
};
// stride }

template<class T, class I, int N>
struct stride_iterator : private stride<I,N> {
  public:
  // type traits
    using base = stride<I,N>;
    /// std::iterator type traits
    using value_type = T;
    using difference_type = I;
    using iterator_category = std::random_access_iterator_tag;

  // ctor
    stride_iterator(T* ptr, const stride<I,N>& str)
      : base(str)
      , ptr(ptr)
    {}

  // iterator interface
    FORCE_INLINE constexpr auto
    operator++() -> stride_iterator& {
      ptr += base::stride_length();
      return *this;
    }

    FORCE_INLINE constexpr auto
    operator*() const -> T& {
      return *ptr;
    }

    FORCE_INLINE friend constexpr auto
    operator==(const stride_iterator& x, const stride_iterator& y) {
      return x.ptr==y.ptr && x.get_stride()==y.get_stride();
    }
    FORCE_INLINE friend constexpr auto
    operator!=(const stride_iterator& x, const stride_iterator& y) {
      return !(x==y);
    }
  private:
    FORCE_INLINE constexpr auto
    get_stride() const -> const base& {
      return *this;
    }
    T* ptr;
};


template<class T, class I, int N=dynamic_size>
class strided_span : private stride<I,N> {
  public:
  // type traits
    using base = stride<I,N>;

    using value_type      = T;
    using iterator        = stride_iterator<T,I,N>;
    using const_iterator  = stride_iterator<const T,I,N>;
    using reference       = T&;
    using const_reference = const T&;

  // ctors
    FORCE_INLINE constexpr
    strided_span()
    {}

    // dynamic stride ctor
    FORCE_INLINE constexpr explicit
    strided_span(T* ptr, I sz, I n)
      : base(n)
      , ptr(ptr)
      , sz(sz)
    {}
    // static stride ctor
    FORCE_INLINE constexpr explicit
    strided_span(T* ptr, I sz)
      : base()
      , ptr(ptr)
      , sz(sz)
    {
      static_assert(N!=dynamic_size,"stride of dynamic length: a length must be specified");
    }

  // inherited behavior
    using base::stride_length;

  // basic accessors
    FORCE_INLINE constexpr auto
    size() const -> I {
      return sz;
    }
    FORCE_INLINE constexpr auto
    data() -> T* {
      return ptr;
    }
    FORCE_INLINE constexpr auto
    data() const -> const T* {
      return ptr;
    }

  // accessors
    FORCE_INLINE constexpr auto
    begin() -> iterator {
      return {ptr,get_stride()};
    }
    FORCE_INLINE constexpr auto
    begin() const -> const_iterator {
      return {ptr,get_stride()};
    }
    FORCE_INLINE constexpr auto
    end() -> iterator {
      return {ptr+sz*stride_length(),get_stride()};
    }
    FORCE_INLINE constexpr auto
    end() const -> const_iterator {
      return {ptr+sz*stride_length(),get_stride()};
    }

    FORCE_INLINE constexpr auto
    operator[](int i) -> reference {
      return ptr[i*stride_length()];
    }
    FORCE_INLINE constexpr auto
    operator[](int i) const -> const_reference {
      return ptr[i*stride_length()];
    }

  private:
    FORCE_INLINE constexpr auto
    get_stride() const -> const base& {
      return *this;
    }
    T* ptr;
    I sz;
};

template<class T, class I, int N> FORCE_INLINE constexpr auto
begin(strided_span<T,I,N>& x) {
  return x.begin();
}
template<class T, class I, int N> FORCE_INLINE constexpr auto
begin(const strided_span<T,I,N>& x) {
  return x.begin();
}
template<class T, class I, int N> FORCE_INLINE constexpr auto
end(strided_span<T,I,N>& x) {
  return x.end();
}
template<class T, class I, int N> FORCE_INLINE constexpr auto
end(const strided_span<T,I,N>& x) {
  return x.end();
}

template<class T0, class T1, class I0, class I1, int N0, int N1> constexpr auto
operator==(const strided_span<T0,I0,N0>& x, const strided_span<T1,I1,N1>& y) -> bool {
  if (x.size() != y.size()) return false;
  return std::equal(x.begin(),x.end(),y.begin());
}
template<class T0, class T1, class I0, class I1, int N0, int N1> constexpr auto
operator!=(const strided_span<T0,I0,N0>& x, const strided_span<T1,I1,N1>& y) -> bool {
  return !(x==y);
}




template<class T, class I> FORCE_INLINE constexpr auto
make_strided_span(T* ptr, I sz, I n) {
  return strided_span<T,I,dynamic_size>(ptr,sz,n);
}
template<class T, class I> FORCE_INLINE constexpr auto
make_strided_span(const T* ptr, I sz, I n) {
  return strided_span<const T,I,dynamic_size>(ptr,sz,n);
}
template<int N, class T, class I> FORCE_INLINE constexpr auto
make_strided_span(T* ptr, I sz) {
  return strided_span<T,I,N>(ptr,sz);
}
template<int N, class T, class I> FORCE_INLINE constexpr auto
make_strided_span(const T* ptr, I sz) {
  return strided_span<const T,I,N>(ptr,sz);
}

template<class Range, class I> FORCE_INLINE constexpr auto
make_strided_span(Range& x, I n) {
  I sz = x.size()/n;
  return make_strided_span(x.data(),sz,n);
}
template<class Range, class I> FORCE_INLINE constexpr auto
make_strided_span(const Range& x, I n) {
  I sz = x.size()/n;
  return make_strided_span(x.data(),sz,n);
}
template<int N, class Range> FORCE_INLINE constexpr auto
make_strided_span(Range& x) {
  auto sz = x.size()/N;
  return make_strided_span<N>(x.data(),sz);
}
template<int N, class Range> FORCE_INLINE constexpr auto
make_strided_span(const Range& x) {
  auto sz = x.size()/N;
  return make_strided_span<N>(x.data(),sz);
}


} // std_e
