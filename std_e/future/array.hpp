#pragma once


#include "std_e/future/span_ref.hpp"
#include <array>
#include <type_traits>


namespace std_e {


// std_e::array is just like std::array
// except that it can be constructed from a std_e::span_ref
// (necessary to make ranges with reference==span_ref and value_type==array-like type
template<class T, ptrdiff_t N>
class array : public std::array<T,N> {
  public:
    using base = std::array<T,N>;

    constexpr
    array() = default;

    template<class T0, class... Ts0>
      requires (
          sizeof...(Ts0)>0 // when there is more than 1 arg, there is no ctor ambiguity
       || (    !std::is_same_v< span_ref<      T,N> , std::remove_cvref_t<T0> >  // disable because handled by ctor below
            && !std::is_same_v< span_ref<const T,N> , std::remove_cvref_t<T0> > )// disable because handled by ctor below
      )
        constexpr
    array(T0&& x, Ts0&&... xs)
      : base{FWD(x),FWD(xs)...}
    {}

    constexpr
    array(const span_ref<T,N>& x) {
      std::copy(x.begin(),x.end(),this->begin());
    }
    constexpr
    array(const span_ref<const T,N>& x) {
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


} // std_e


// Customization point for std::common_reference
// SEE for https://en.cppreference.com/w/cpp/types/common_reference
template<class T, ptrdiff_t N,  template<class> class TQual0, template<class> class TQual1>
struct std::basic_common_reference< std_e::array<T,N> , std_e::span_ref<T,N> , TQual0,TQual1 > {
  using type = std_e::span_ref<T,N>;
};
template<class T, ptrdiff_t N,  template<class> class TQual1, template<class> class TQual0>
struct std::basic_common_reference< std_e::span_ref<T,N> , std_e::array<T,N> , TQual1,TQual0 > {
  using type = std_e::span_ref<T,N>;
};
