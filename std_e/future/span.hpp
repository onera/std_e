#pragma once


#include <algorithm>
#include <type_traits>
#include "std_e/base/dynamic_size.hpp"
#include "std_e/base/macros.hpp"
#include "std_e/future/span_fwd.hpp"
#include "std_e/future/type_traits.hpp"
#include "std_e/utils/to_string.hpp"


namespace std_e {


// span_size {
template<ptrdiff_t N>
struct span_size {
  using size_type = size_t;
  static const size_t extent = N;

  constexpr span_size() {}
  constexpr span_size(size_type) {}

  static FORCE_INLINE constexpr auto
  size() -> size_type {
    return N;
  }

  friend constexpr auto
  operator==(span_size, span_size) -> bool {
    return true;
  }
  friend constexpr auto
  operator<(span_size, span_size) -> bool {
    return false;
  }
  friend constexpr auto
  operator!=(span_size x, span_size y) -> bool {
    return !(x==y);
  }
  friend constexpr auto
  operator>(span_size x, span_size y) -> bool {
    return y<x;
  }
  friend constexpr auto
  operator<=(span_size x, span_size y) -> bool {
    return !(y<x);
  }
  friend constexpr auto
  operator>=(span_size x, span_size y) -> bool {
    return !(x<y);
  }
};

template<>
class span_size<dynamic_size> {
  public:
    using size_type = size_t;
    static const size_t extent = size_t(dynamic_size);

    FORCE_INLINE constexpr
    span_size()
      : n(0)
    {}
    FORCE_INLINE constexpr
    span_size(size_type n)
      : n(n)
    {}

    FORCE_INLINE constexpr auto
    size() const -> size_type {
      return n;
    }

  // comparison
    friend constexpr auto
    operator==(span_size x, span_size y) -> bool {
      return x.n==y.n;
    }
    friend constexpr auto
    operator<(span_size x, span_size y) -> bool {
      return x.n<y.n;
    }
    friend constexpr auto
    operator!=(span_size x, span_size y) -> bool {
      return !(x==y);
    }
    friend constexpr auto
    operator>(span_size x, span_size y) -> bool {
      return y<x;
    }
    friend constexpr auto
    operator<=(span_size x, span_size y) -> bool {
      return !(y<x);
    }
    friend constexpr auto
    operator>=(span_size x, span_size y) -> bool {
      return !(x<y);
    }
  private:
    size_type n;
};
// span_size }


template<class T, ptrdiff_t N=dynamic_size>
class span_base : public span_size<N> {
  public:
  // type traits
    using span_size_type = span_size<N>;

    using element_type    = T;
    using value_type      = std::remove_cv_t<T>; // same as C++20
    using pointer         = T*;
    using const_pointer   = const T*;
    using reference       = T&;
    using const_reference = const T&;
    using iterator        = pointer;
    using const_iterator  = const_pointer;
    using difference_type = ptrdiff_t;

  // ctors
    FORCE_INLINE constexpr span_base() = default;
    // dynamic span ctor
    FORCE_INLINE constexpr explicit
    span_base(T* ptr, ptrdiff_t n)
      // Precondition: [ptr,ptr+N) is valid range
      : span_size_type(n)
      , ptr(ptr)
    {}
    FORCE_INLINE constexpr explicit
    span_base(T* ptr)
      #if __cplusplus > 201703L
        requires (N!=dynamic_size)
      #endif
      // Precondition: [ptr,ptr+N) is valid range
      : span_size_type()
      , ptr(ptr)
    {
      static_assert(N!=dynamic_size);
    }

    FORCE_INLINE constexpr explicit
    span_base(T* first, T* last)
      // Precondition: [first,last) is valid range
      : span_size_type(last-first)
      , ptr(first)
    {}
    FORCE_INLINE constexpr explicit
    span_base(typename std::vector<std::remove_const_t<T>>::iterator first, typename std::vector<std::remove_const_t<T>>::iterator last)
      // Precondition: [first,last) is valid range
      : span_size_type(last-first)
      , ptr(&*first) // TODO
    {}
    template<class Range, std::enable_if_t< !std::is_pointer_v<std_e::remove_cvref_t<Range>> , int > =0> FORCE_INLINE constexpr
    // requires Range::data() -> T*
    span_base(Range&& r)
      : span_size_type(r.size())
      , ptr(r.data())
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

    FORCE_INLINE constexpr auto
    back() -> reference {
      return ptr[size()-1];
    }
    FORCE_INLINE constexpr auto
    back() const -> const_reference {
      return ptr[size()-1];
    }

  private:
    T* ptr;
};

template<class T, ptrdiff_t N> FORCE_INLINE constexpr auto
begin(span_base<T,N>& x) {
  return x.begin();
}
template<class T, ptrdiff_t N> FORCE_INLINE constexpr auto
begin(const span_base<T,N>& x) {
  return x.begin();
}
template<class T, ptrdiff_t N> FORCE_INLINE constexpr auto
end(span_base<T,N>& x) {
  return x.end();
}
template<class T, ptrdiff_t N> FORCE_INLINE constexpr auto
end(const span_base<T,N>& x) {
  return x.end();
}

template<class T0, class T1, ptrdiff_t N0, ptrdiff_t N1> constexpr auto
operator==(const span_base<T0,N0>& x, const span_base<T1,N1>& y) -> bool {
  if (x.size() != y.size()) return false;
  return std::equal(x.begin(),x.end(),y.begin());
}
template<class T0, class T1, ptrdiff_t N0, ptrdiff_t N1> constexpr auto
operator<(const span_base<T0,N0>& x, const span_base<T1,N1>& y) -> bool {
  return std::lexicographical_compare(x.begin(),x.end(),y.begin(),y.end());
}
template<class T0, class T1, ptrdiff_t N0, ptrdiff_t N1> constexpr auto
operator!=(const span_base<T0,N0>& x, const span_base<T1,N1>& y) -> bool {
  return !(x==y);
}
template<class T0, class T1, ptrdiff_t N0, ptrdiff_t N1> constexpr auto
operator>(const span_base<T0,N0>& x, const span_base<T1,N1>& y) -> bool {
  return y<x;
}
template<class T0, class T1, ptrdiff_t N0, ptrdiff_t N1> constexpr auto
operator<=(const span_base<T0,N0>& x, const span_base<T1,N1>& y) -> bool {
  return !(y<x);
}
template<class T0, class T1, ptrdiff_t N0, ptrdiff_t N1> constexpr auto
operator>=(const span_base<T0,N0>& x, const span_base<T1,N1>& y) -> bool {
  return !(x<y);
}




template<class T, ptrdiff_t N> FORCE_INLINE constexpr auto
to_string(const span_base<T,N>& x) {
  return range_to_string(x);
}


// operator== with vector {
template<class T0, class T1, ptrdiff_t N, class A> constexpr auto
operator==(const span_base<T0,N>& x, const std::vector<T1,A>& y) -> bool {
  return x==span_base<const T1>(y);
}
template<class T0, class T1, ptrdiff_t N, class A> constexpr auto
operator!=(const span_base<T0,N>& x, const std::vector<T1,A>& y) -> bool {
  return !(x==y);
}
template<class T0, class T1, ptrdiff_t N, class A> constexpr auto
operator==(const std::vector<T1,A>& x, const span_base<T0,N>& y) -> bool {
  return y==x;
}
template<class T0, class T1, ptrdiff_t N, class A> constexpr auto
operator!=(const std::vector<T1,A>& y, const span_base<T0,N>& x) -> bool {
  return !(x==y);
}
// }
// operator== with dynarray {
template<class T0, class T1, ptrdiff_t N> constexpr auto
operator==(const span_base<T0,N>& x, const std_e::dynarray<T1>& y) -> bool {
  return x==span_base<const T1>(y);
}
template<class T0, class T1, ptrdiff_t N> constexpr auto
operator!=(const span_base<T0,N>& x, const std_e::dynarray<T1>& y) -> bool {
  return !(x==y);
}
template<class T0, class T1, ptrdiff_t N> constexpr auto
operator==(const std_e::dynarray<T1>& x, const span_base<T0,N>& y) -> bool {
  return y==x;
}
template<class T0, class T1, ptrdiff_t N> constexpr auto
operator!=(const std_e::dynarray<T1>& y, const span_base<T0,N>& x) -> bool {
  return !(x==y);
}
// }


template<class T, ptrdiff_t N>
class span : public span_base<T,N> {
  public:
    using base = span_base<T,N>;
    using base::base;
};


// deduction guideline
template<class T>
span(T* ptr, ptrdiff_t n) -> span<T,dynamic_size>;

// op== and op!= {
// Needed. The span_base ones should be taken by the compiler,
// however this is not the case and another (unknown) op== is used...
template<class T0, class T1, ptrdiff_t N0, ptrdiff_t N1> constexpr auto
operator==(const span<T0,N0>& x, const span<T1,N1>& y) -> bool {
  return span_base<const T0,N0>(x)==span_base<const T1,N1>(y);
}
template<class T0, class T1, ptrdiff_t N0, ptrdiff_t N1> constexpr auto
operator!=(const span<T0,N0>& x, const span<T1,N1>& y) -> bool {
  return !(x==y);
}
template<class T0, class T1, ptrdiff_t N0, ptrdiff_t N1> constexpr auto
operator<(const span<T0,N0>& x, const span<T1,N1>& y) -> bool {
  return span_base<const T0,N0>(x)<span_base<const T1,N1>(y);
}

template<class T0, class T1, ptrdiff_t N, class A> constexpr auto
operator==(const span<T0,N>& x, const std::vector<T1,A>& y) -> bool {
  return span_base<const T0,N>(x)==y;
}
template<class T0, class T1, ptrdiff_t N, class A> constexpr auto
operator!=(const span<T0,N>& x, const std::vector<T1,A>& y) -> bool {
  return !(x==y);
}
template<class T0, class T1, ptrdiff_t N, class A> constexpr auto
operator<(const span<T0,N>& x, const std::vector<T1,A>& y) -> bool {
  return span_base<const T0,N>(x)<y;
}

template<class T0, class T1, ptrdiff_t N, class A> constexpr auto
operator==(const std::vector<T1,A>& x, const span<T0,N>& y) -> bool {
  return x==span_base<const T1,N>(y);
}
template<class T0, class T1, ptrdiff_t N, class A> constexpr auto
operator!=(const std::vector<T1,A>& x, const span<T0,N>& y) -> bool {
  return !(x==y);
}
template<class T0, class T1, ptrdiff_t N, class A> constexpr auto
operator<(const std::vector<T1,A>& x, const span<T0,N>& y) -> bool {
  return x<span_base<const T1,N>(y);
}


// op== and op!= }


template<ptrdiff_t N, class T> FORCE_INLINE constexpr auto
make_span(T* ptr) {
  static_assert(N!=dynamic_size,"can't create dynamic span with no size given");
  return span<T,N>(ptr);
}
template<ptrdiff_t N, class T> FORCE_INLINE constexpr auto
make_span(const T* ptr) {
  static_assert(N!=dynamic_size,"can't create dynamic span with no size given");
  return span<const T,N>(ptr);
}

template<class T> FORCE_INLINE constexpr auto
make_span(T* ptr, ptrdiff_t n) {
  return span<T,dynamic_size>(ptr,n);
}
template<class T> FORCE_INLINE constexpr auto
make_span(T* start, T* stop) {
  return span<T,dynamic_size>(start,stop-start);
}

template<class T> FORCE_INLINE constexpr auto
make_span(T* ptr, ptrdiff_t start, ptrdiff_t n) { // TODO deprecate (use make_subspan_n)
  return span<T,dynamic_size>(ptr+start,n);
}
template<class T, class I> FORCE_INLINE constexpr auto
make_subspan_n(T* ptr, I start, I n) {
  return span<T,dynamic_size>(ptr+start,n);
}
template<class T, class I> FORCE_INLINE constexpr auto
make_subspan(T* ptr, I start, I stop) {
  return make_subspan_n(ptr, start, stop-start);
}

template<class Contiguous_range> FORCE_INLINE constexpr auto
make_span(Contiguous_range&& x) {
  return make_span(x.data(),x.size());
}
template<
  class Contiguous_range, class I,
  std::enable_if_t<!std::is_pointer_v<std::remove_reference_t<Contiguous_range>>,int> =0
> FORCE_INLINE constexpr auto
make_span(Contiguous_range& x, I start, I stop) { // TODO deprecate (use make_subspan)
  return make_subspan(x.data(),start,stop);
}
template<
  class Contiguous_range, class I,
  std::enable_if_t<!std::is_pointer_v<std::remove_reference_t<Contiguous_range>>,int> =0
> FORCE_INLINE constexpr auto
make_subspan(Contiguous_range& x, I start, I stop) {
  return make_subspan(x.data(),start,stop);
}
template<class Contiguous_range, class I> FORCE_INLINE constexpr auto
make_span_n(Contiguous_range& x, I start, I n) { // TODO deprecate (use make_subspan_n)
  return make_span(x.data(),start,n);
}
template<class Contiguous_range, class I> FORCE_INLINE constexpr auto
make_subspan_n(Contiguous_range& x, I start, I n) {
  return make_subspan_n(x.data(),start,n);
}


} // std_e


template<class T, ptrdiff_t N>
struct std::tuple_size<std_e::span<T,N>> {
  static const size_t value = N;
};
