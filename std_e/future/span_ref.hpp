#pragma once


#include "std_e/future/span.hpp"


namespace std_e {


template<class T, ptrdiff_t N=dynamic_size>
class span_ref : public span_base<T,N> {
  public:
  // ctors
    FORCE_INLINE constexpr
    span_ref() = delete; // a reference type is not default constructible

    FORCE_INLINE constexpr
    span_ref(span_ref&& r) = default;

    FORCE_INLINE constexpr
    span_ref(const span_ref& r) = default;

    using base = span_base<T,N>;
    using base::base; // inherit other ctors

  // assignment with reference semantics (assign through)
    /// move/copy assign
    FORCE_INLINE constexpr
    span_ref& operator=(span_ref&& r) {
      STD_E_ASSERT(r.size() == this->size());
      std::copy(r.begin(),r.end(),this->begin());
      return *this;
    }
    FORCE_INLINE constexpr
    span_ref& operator=(const span_ref& r) {
      STD_E_ASSERT(r.size() == this->size());
      std::copy(r.begin(),r.end(),this->begin());
      return *this;
    }

    /// others
    template<class Range> FORCE_INLINE constexpr
    span_ref& operator=(Range&& r) {
      STD_E_ASSERT(r.size() == this->size());
      std::copy(r.begin(),r.end(),this->begin());
      return *this;
    }
    // operator= const version to satisfy proxy reference in std::indirectly_writable
    template<class Range> FORCE_INLINE constexpr
    const span_ref& operator=(Range&& r) const {
      STD_E_ASSERT(r.size() == this->size());
      std::copy(r.begin(),r.end(),this->begin());
      return *this;
    }
    template<ptrdiff_t N0>
    span_ref& operator=(const T(&arr)[N0])
    {
      static_assert(N0==N);
      std::copy(arr, arr+N, this->begin());
      return *this;
    }
};

// op== and op!= {
// Needed. Same reason for span: span_base should be picked but is not
template<class T0, class T1, ptrdiff_t N0, ptrdiff_t N1> constexpr auto
operator==(const span_ref<T0,N0>& x, const span_ref<T1,N1>& y) -> bool {
  return span_base<const T0,N0>(x)==span_base<const T1,N1>(y);
}
template<class T0, class T1, ptrdiff_t N0, ptrdiff_t N1> constexpr auto
operator<(const span_ref<T0,N0>& x, const span_ref<T1,N1>& y) -> bool {
  return span_base<const T0,N0>(x)<span_base<const T1,N1>(y);
}
template<class T0, class T1, ptrdiff_t N0, ptrdiff_t N1> constexpr auto
operator!=(const span_ref<T0,N0>& x, const span_ref<T1,N1>& y) -> bool {
  return !(x==y);
}
template<class T0, class T1, ptrdiff_t N0, ptrdiff_t N1> constexpr auto
operator>(const span_ref<T0,N0>& x, const span_ref<T1,N1>& y) -> bool {
  return y<x;
}
template<class T0, class T1, ptrdiff_t N0, ptrdiff_t N1> constexpr auto
operator<=(const span_ref<T0,N0>& x, const span_ref<T1,N1>& y) -> bool {
  return !(y<x);
}
template<class T0, class T1, ptrdiff_t N0, ptrdiff_t N1> constexpr auto
operator>=(const span_ref<T0,N0>& x, const span_ref<T1,N1>& y) -> bool {
  return !(x<y);
}
// op== and op!= }

template<class T, ptrdiff_t N> constexpr auto
swap(span_ref<T,N> x, span_ref<T,N> y) -> void {
  STD_E_ASSERT(x.size()==y.size());
  for (ptrdiff_t i=0; i<ptrdiff_t(x.size()); ++i) {
    using std::swap;
    swap(x[i],y[i]);
  }
}

template<class T> FORCE_INLINE constexpr auto
make_span_ref(T* ptr, ptrdiff_t n) {
  return span_ref<T,dynamic_size>(ptr,n);
}
template<class Contiguous_range> FORCE_INLINE constexpr auto
make_span_ref(Contiguous_range& x) {
  return make_span_ref(x.data(),x.size());
}


} // std_e


template<class T, ptrdiff_t N>
struct std::tuple_size<std_e::span_ref<T,N>> {
  static const size_t value = N;
};
