#pragma once


#include "std_e/base/memory_view.h"
#include "std_e/base/index_t.h"


namespace std_e {


constexpr index_t dynamic_extent = -1;


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
class span_size<dynamic_extent> {
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


template<class T, index_t N>
class span : public memory_view<T*>, public span_size<N> {
  public:
  // type traits
    using memory_view_type = memory_view<T*>;
    using span_size_type = span_size<N>;

    using value_type           = T;
    using pointer_type         = T*;
    using const_pointer_type   = const T*;
    using reference_type       = T&;
    using const_reference_type = const T&;

  // ctors
    FORCE_INLINE constexpr
    span()
    {}

    // dynamic span ctor
    FORCE_INLINE constexpr explicit
    span(T* ptr, index_t n)
      // Precondition: [ptr,ptr+N) is valid range
      : memory_view_type(ptr)
      , span_size_type(n)
    {}

    // static span ctor
    FORCE_INLINE constexpr explicit
    span(T* ptr)
      // Precondition: [ptr,ptr+N) is valid range
      : memory_view_type(ptr)
      , span_size_type()
    {}

  // inherited behavior
    using span_size_type::size;

    using memory_view_type::data;
    using memory_view_type::operator[];
    using memory_view_type::begin;

  // accessors
    FORCE_INLINE constexpr auto
    end() -> pointer_type {
      return data()+size();
    }
    FORCE_INLINE constexpr auto
    end() const -> const_pointer_type {
      return data()+size();
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
};

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
  static_assert(N!=dynamic_extent,"do not use this function to create a dynamic span");
  return span<T,N>(ptr);
}
template<class T> FORCE_INLINE constexpr auto
make_span(T* ptr, index_t n) {
  return span<T,dynamic_extent>(ptr,n);
}
template<class T> FORCE_INLINE constexpr auto
make_span(T* ptr, index_t offset, index_t n) {
  return span<T,dynamic_extent>(ptr+offset,n);
}


} // std_e


namespace std {

template<class T, index_t N>
struct tuple_size<std_e::span<T,N>> {
  static const size_t value = N;
};

} // std
