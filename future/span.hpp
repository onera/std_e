#pragma once


#include "std_e/base/dynamic_size.hpp"


namespace std_e {


// span_size {
template<index_t N>
struct span_size {
  constexpr span_size() {}

  static FORCE_INLINE constexpr auto
  size() -> size_t {
    return N;
  }

  friend constexpr auto
  operator==(span_size, span_size) -> bool {
    return true;
  }
  friend constexpr auto
  operator!=(span_size x, span_size y) -> bool {
    return !(x==y);
  }
};

template<>
class span_size<dynamic_size> {
  public:
    FORCE_INLINE constexpr
    span_size()
      : n(0)
    {}
    FORCE_INLINE constexpr
    span_size(size_t n)
      : n(n)
    {}
    FORCE_INLINE constexpr
    span_size(index_t n)
      : n(n)
    {}

    FORCE_INLINE constexpr auto
    size() const -> size_t {
      return n;
    }

  // comparison
    friend constexpr auto
    operator==(span_size x, span_size y) -> bool {
      return x.n==y.n;
    }
    friend constexpr auto
    operator!=(span_size x, span_size y) -> bool {
      return !(x==y);
    }
  private:
    size_t n;
};
// span_size }


template<class T, index_t N=dynamic_size>
class span : public span_size<N> {
  public:
  // type traits
    using span_size_type = span_size<N>;

    using value_type      = T;
    using pointer         = T*;
    using const_pointer   = const T*;
    using reference       = T&;
    using const_reference = const T&;

  // ctors
    FORCE_INLINE constexpr
    span()
    {}

    // dynamic span ctor
    FORCE_INLINE constexpr explicit
    span(T* ptr, index_t n)
      // Precondition: [ptr,ptr+N) is valid range
      : span_size_type(n)
      , ptr(ptr)
    {}

    // static span ctor
    FORCE_INLINE constexpr explicit
    span(T* ptr)
      // Precondition: [ptr,ptr+N) is valid range
      : span_size_type()
      , ptr(ptr)
    {}

  // inherited behavior
    using span_size_type::size;

  // accessors
    FORCE_INLINE constexpr auto
    data() -> pointer {
      return ptr;
    }
    FORCE_INLINE constexpr auto
    data() const -> const_pointer {
      return ptr;
    }

    FORCE_INLINE constexpr auto
    begin() -> pointer {
      return ptr;
    }
    FORCE_INLINE constexpr auto
    begin() const -> const_pointer {
      return ptr;
    }
    FORCE_INLINE constexpr auto
    end() -> pointer {
      return data()+size();
    }
    FORCE_INLINE constexpr auto
    end() const -> const_pointer {
      return data()+size();
    }

    FORCE_INLINE constexpr auto
    operator[](int i) -> reference {
      return ptr[i];
    }
    FORCE_INLINE constexpr auto
    operator[](int i) const -> const_reference {
      return ptr[i];
    }

  // comparison
    friend constexpr auto
    operator==(const span& x, const span& y) -> bool {
      if (x.size() != y.size()) return false;
      return equal(x.begin(),x.end(),y.begin());
    }
    friend constexpr auto
    operator!=(const span& x, const span& y) -> bool {
      return !(x==y);
    }
  private:
    T* ptr;
};

template<class T, index_t N> FORCE_INLINE constexpr auto
begin(span<T,N>& x) {
  return x.begin();
}
template<class T, index_t N> FORCE_INLINE constexpr auto
begin(const span<T,N>& x) {
  return x.begin();
}
template<class T, index_t N> FORCE_INLINE constexpr auto
end(span<T,N>& x) {
  return x.end();
}
template<class T, index_t N> FORCE_INLINE constexpr auto
end(const span<T,N>& x) {
  return x.end();
}




template<index_t N, class T> FORCE_INLINE constexpr auto
make_span(T* ptr) {
  static_assert(N!=dynamic_size,"can't create dynamic span with no size given");
  return span<T,N>(ptr);
}
template<index_t N, class T> FORCE_INLINE constexpr auto
make_span(const T* ptr) {
  static_assert(N!=dynamic_size,"can't create dynamic span with no size given");
  return span<const T,N>(ptr);
}

template<class T> FORCE_INLINE constexpr auto
make_span(T* ptr, index_t n) {
  return span<T,dynamic_size>(ptr,n);
}
template<class T> FORCE_INLINE constexpr auto
make_span(const T* ptr, index_t n) {
  return span<const T,dynamic_size>(ptr,n);
}

template<class T> FORCE_INLINE constexpr auto
make_span(T* ptr, index_t offset, index_t n) {
  return span<T,dynamic_size>(ptr+offset,n);
}
template<class T> FORCE_INLINE constexpr auto
make_span(const T* ptr, index_t offset, index_t n) {
  return span<const T,dynamic_size>(ptr+offset,n);
}

template<class Container> FORCE_INLINE constexpr auto
make_span(Container& x) {
  using T = typename Container::value_type;
  return span<T,dynamic_size>(x.data(),x.size());
}
template<class Container> FORCE_INLINE constexpr auto
make_span(const Container& x) {
  using T = typename Container::value_type;
  return span<const T,dynamic_size>(x.data(),x.size());
}


} // std_e


namespace std {

template<class T, index_t N>
struct tuple_size<std_e::span<T,N>> {
  static const size_t value = N;
};

} // std
