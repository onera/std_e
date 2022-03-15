#pragma once


#include "std_e/data_structure/block_range/vblock_ref.hpp"
#include <vector>


// Should not be needed, see https://stackoverflow.com/q/71488173/1583122
namespace std_e {


// std_e::vblock_val is just like std::vector
// except that it can be constructed from a std_e::vblock_ref
// (necessary to make ranges with reference==vblock_ref and value_type==vblock_val
template<class T>
class vblock_val : public std::vector<T> {
  public:
    using base = std::vector<T>;

    constexpr
    vblock_val() = default;

    template<class I> constexpr
    vblock_val(const vblock_ref<T,I>& x) {
      std::copy(x.begin(),x.end(),this->begin());
    }
    template<class I> constexpr
    vblock_val(const vblock_ref<const T,I>& x) {
      std::copy(x.begin(),x.end(),this->begin());
    }
};
template<class T0, class T1, class I> constexpr auto
operator==(const vblock_val<T0>& x, const vblock_ref<T1,I>& y) -> bool {
  return make_span(x.data(),x.size()) == y;
}
template<class T0, class T1, class I> constexpr auto
operator==(const vblock_ref<T1,I>& x, const vblock_val<T0>& y) -> bool {
  return x == make_span(y.data(),y.size());
}
template<class T0, class T1, class I> constexpr auto
operator<(const vblock_val<T0>& x, const vblock_ref<T1,I>& y) -> bool {
  return make_span(x.data(),x.size()) < y;
}
template<class T0, class T1, class I> constexpr auto
operator<(const vblock_ref<T1,I>& x, const vblock_val<T0>& y) -> bool {
  return x < make_span(y.data(),y.size());
}

template<class T0, class T1, class I> constexpr auto
operator!=(const vblock_val<T0>& x, const vblock_ref<T1,I>& y) -> bool {
  return !(x==y);
}
template<class T0, class T1, class I> constexpr auto
operator!=(const vblock_ref<T1,I>& x, const vblock_val<T0>& y) -> bool {
  return !(x==y);
}
template<class T0, class T1, class I> constexpr auto
operator>(const vblock_val<T0>& x, const vblock_ref<T1,I>& y) -> bool {
  return y<x;
}
template<class T0, class T1, class I> constexpr auto
operator>(const vblock_ref<T1,I>& x, const vblock_val<T0>& y) -> bool {
  return y<x;
}
template<class T0, class T1, class I> constexpr auto
operator<=(const vblock_val<T0>& x, const vblock_ref<T1,I>& y) -> bool {
  return !(y<x);
}
template<class T0, class T1, class I> constexpr auto
operator<=(const vblock_ref<T1,I>& x, const vblock_val<T0>& y) -> bool {
  return !(y<x);
}
template<class T0, class T1, class I> constexpr auto
operator>=(const vblock_val<T0>& x, const vblock_ref<T1,I>& y) -> bool {
  return !(x<y);
}
template<class T0, class T1, class I> constexpr auto
operator>=(const vblock_ref<T1,I>& x, const vblock_val<T0>& y) -> bool {
  return !(x<y);
}
// op== and op!= }


} // std_e


// Customization point for std::common_reference
// SEE for https://en.cppreference.com/w/cpp/types/common_reference
template<class T, class I,  template<class> class TQual0, template<class> class TQual1>
struct std::basic_common_reference< std_e::vblock_val<T> , std_e::vblock_ref<T,I> , TQual0,TQual1 > {
  using type = std_e::vblock_ref<T,I>;
};
template<class T, class I,  template<class> class TQual1, template<class> class TQual0>
struct std::basic_common_reference< std_e::vblock_ref<T,I> , std_e::vblock_val<T> , TQual1,TQual0 > {
  using type = std_e::vblock_ref<T,I>;
};
