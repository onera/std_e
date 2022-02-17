#pragma once


#include "std_e/future/span.hpp"
#include <array>


namespace std_e {


template<class T, ptrdiff_t N>
class array : public std::array<T,N> {
  public:
    using base = std::array<T,N>;
    using base::base;

    constexpr
    array(const span_base<T,N>& x) {
      std::copy(x.begin(),x.end(),this->begin());
    }
};
template<class T0, class T1, ptrdiff_t N0, ptrdiff_t N1> constexpr auto
operator==(const array<T0,N0>& x, const span_ref<T1,N1>& y) -> bool {
  return make_span(x.data(),x.size()) == y;
}
template<class T0, class T1, ptrdiff_t N0, ptrdiff_t N1> constexpr auto
operator==(const span_ref<T1,N1>& x, const array<T0,N0>& y) -> bool {
  return x == make_span(y.data(),y.size());
}
template<class T0, class T1, ptrdiff_t N0, ptrdiff_t N1> constexpr auto
operator<(const array<T0,N0>& x, const span_ref<T1,N1>& y) -> bool {
  return make_span(x.data(),x.size()) < y;
}
template<class T0, class T1, ptrdiff_t N0, ptrdiff_t N1> constexpr auto
operator<(const span_ref<T1,N1>& x, const array<T0,N0>& y) -> bool {
  return x < make_span(y.data(),y.size());
}

template<class T0, class T1, ptrdiff_t N0, ptrdiff_t N1> constexpr auto
operator!=(const array<T0,N0>& x, const span_ref<T1,N1>& y) -> bool {
  return !(x==y);
}
template<class T0, class T1, ptrdiff_t N0, ptrdiff_t N1> constexpr auto
operator!=(const span_ref<T1,N1>& x, const array<T0,N0>& y) -> bool {
  return !(x==y);
}
template<class T0, class T1, ptrdiff_t N0, ptrdiff_t N1> constexpr auto
operator>(const array<T0,N0>& x, const span_ref<T1,N1>& y) -> bool {
  return y<x;
}
template<class T0, class T1, ptrdiff_t N0, ptrdiff_t N1> constexpr auto
operator>(const span_ref<T1,N1>& x, const array<T0,N0>& y) -> bool {
  return y<x;
}
template<class T0, class T1, ptrdiff_t N0, ptrdiff_t N1> constexpr auto
operator<=(const array<T0,N0>& x, const span_ref<T1,N1>& y) -> bool {
  return !(y<x);
}
template<class T0, class T1, ptrdiff_t N0, ptrdiff_t N1> constexpr auto
operator<=(const span_ref<T1,N1>& x, const array<T0,N0>& y) -> bool {
  return !(y<x);
}
template<class T0, class T1, ptrdiff_t N0, ptrdiff_t N1> constexpr auto
operator>=(const array<T0,N0>& x, const span_ref<T1,N1>& y) -> bool {
  return !(x<y);
}
template<class T0, class T1, ptrdiff_t N0, ptrdiff_t N1> constexpr auto
operator>=(const span_ref<T1,N1>& x, const array<T0,N0>& y) -> bool {
  return !(x<y);
}
// op== and op!= }


} // std_e;


