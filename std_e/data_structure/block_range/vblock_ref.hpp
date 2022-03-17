#pragma once


#include <algorithm>
#include <vector>


namespace std_e {


template<class T> class vblock_val; // fwd decl


template<class T, class I>
class vblock_ref {
  private:
    T* ptr;
    I* offsets_ptr;
  public:
  // traits
    using value_type = T;
    using iterator = T*;
    using const_iterator = const T*;
    using reference = T&;
    using const_reference = const T&;

  // ctors
    vblock_ref(T* ptr, I* offsets_ptr)
      : ptr(ptr)
      , offsets_ptr(offsets_ptr)
    {}

  // reference semantics
    vblock_ref() = delete;
    vblock_ref(const vblock_ref&  other) = default;
    vblock_ref(      vblock_ref&& other) = default;

    // Should not be needed, see https://stackoverflow.com/q/71488173/1583122
    vblock_ref(const vblock_val<T>&  other) requires (!std::is_const_v<T>)
    {throw;}
    vblock_ref(      vblock_val<T>&& other) requires (!std::is_const_v<T>)
    {throw;}

    vblock_ref& operator=(const vblock_ref& other) {
      return assign_through(*this,other);
    }
    vblock_ref& operator=(vblock_ref&& other) {
      return assign_through(*this,other);
    }

    // operator= const version to satisfy proxy reference in std::indirectly_writable
    const vblock_ref& operator=(const vblock_ref& other) const {
      return assign_through(*this,other);
    }
    const vblock_ref& operator=(vblock_ref&& other) const {
      return assign_through(*this,other);
    }

    // assign from other range types
    template<class Range>
      requires (!std::is_same_v<std::remove_cvref_t<Range>,vblock_ref>)
    vblock_ref& operator=(Range&& other) {
      return assign_through(*this,other);
    }
    template<class Range>
      requires (!std::is_same_v<std::remove_cvref_t<Range>,vblock_ref>)
    const vblock_ref& operator=(Range&& other) const {
      return assign_through(*this,other);
    }

  // range interface
    constexpr auto size() const -> I { return offsets_ptr[1]-offsets_ptr[0]; }

    constexpr auto data ()       ->       T* { return ptr       ; }
    constexpr auto data () const -> const T* { return ptr       ; }
    constexpr auto begin()       ->       T* { return ptr       ; }
    constexpr auto begin() const -> const T* { return ptr       ; }
    constexpr auto end()         ->       T* { return ptr+size(); }
    constexpr auto end()   const -> const T* { return ptr+size(); }

    template<class Integer> constexpr auto operator[](Integer i)       ->       T& { return ptr[i]; }
    template<class Integer> constexpr auto operator[](Integer i) const -> const T& { return ptr[i]; }
  private:
    template<class vblock_ref_type, class Rng> static constexpr auto
    assign_through(vblock_ref_type& x, const Rng& y) -> vblock_ref_type& {
      std::copy_n(y.begin(), y.size(), x.ptr);
      x.offsets_ptr[1] = x.offsets_ptr[0] + y.size();
      return x;
    }
};
template<class T0, class I0, class T1, class I1> auto
operator==(const vblock_ref<T0,I0>& x, const vblock_ref<T1,I1>& y) -> bool {
  return std::equal( x.begin() , x.end() , y.begin() , y.end() );
}
template<class T0, class I0, class T1, class I1> auto
operator!=(const vblock_ref<T0,I0>& x, const vblock_ref<T1,I1>& y) -> bool {
  return !(x == y);
}
template<class T0, class I0, class T1, class I1> auto
operator< (const vblock_ref<T0,I0>& x, const vblock_ref<T1,I1>& y) -> bool {
  return
       x.size()< y.size()
   || (x.size()==y.size() && std::lexicographical_compare(x.begin(),x.end(),y.begin(),y.end()));
}
template<class T0, class I0, class T1, class I1> auto
operator> (const vblock_ref<T0,I0>& x, const vblock_ref<T1,I1>& y) -> bool {
  return y<x;
}
template<class T0, class I0, class T1, class I1> auto
operator<=(const vblock_ref<T0,I0>& x, const vblock_ref<T1,I1>& y) -> bool {
  return !(x>y);
}
template<class T0, class I0, class T1, class I1> auto
operator>=(const vblock_ref<T0,I0>& x, const vblock_ref<T1,I1>& y) -> bool {
  return !(x<y);
}

template<class T, class I> constexpr auto begin(const vblock_ref<T,I>& x) { return x.begin(); }
template<class T, class I> constexpr auto begin(      vblock_ref<T,I>& x) { return x.begin(); }
template<class T, class I> constexpr auto end  (const vblock_ref<T,I>& x) { return x.end  (); }
template<class T, class I> constexpr auto end  (      vblock_ref<T,I>& x) { return x.end  (); }

template<class T, class I> inline auto
to_string(const vblock_ref<T,I>& x) {
  return std_e::range_to_string(x);
}

template<class T0, class I0, class T1> auto
operator==(const vblock_ref<T0,I0>& x, const std::vector<T1>& y) -> bool {
  return std::equal( x.begin() , x.end() , y.begin() , y.end() );
}
template<class T0, class I0, class T1> auto
operator==(const std::vector<T1>& x, const vblock_ref<T0,I0>& y) -> bool {
  return y==x;
}
template<class T0, class I0, class T1> auto
operator!=(const vblock_ref<T0,I0>& x, const std::vector<T1>& y) -> bool {
  return !(x==y);
}
template<class T0, class I0, class T1> auto
operator!=(const std::vector<T1>& x, const vblock_ref<T0,I0>& y) -> bool {
  return !(x==y);
}


} // std_e
