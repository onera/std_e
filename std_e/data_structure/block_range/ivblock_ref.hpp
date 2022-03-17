#pragma once


#include <algorithm>
#include <vector>


namespace std_e {


template<class I> class vblock_val; // fwd decl


template<class I>
class ivblock_ref {
  private:
    I* ptr;
  public:
  // traits
    using value_type = I;
    using iterator = I*;
    using const_iterator = const I*;
    using reference = I&;
    using const_reference = const I&;

  // ctors
    ivblock_ref(I* ptr)
      : ptr(ptr)
    {}

  // reference semantics
    ivblock_ref() = delete;
    ivblock_ref(const ivblock_ref&  other) = default;
    ivblock_ref(      ivblock_ref&& other) = default;

    // Should not be needed, see https://stackoverflow.com/q/71488173/1583122
    ivblock_ref(const vblock_val<I>&  other) requires (!std::is_const_v<I>)
    {throw;}
    ivblock_ref(      vblock_val<I>&& other) requires (!std::is_const_v<I>)
    {throw;}

    ivblock_ref& operator=(const ivblock_ref& other) {
      return assign_through(*this,other);
    }
    ivblock_ref& operator=(ivblock_ref&& other) {
      return assign_through(*this,other);
    }

    // operator= const version to satisfy proxy reference in std::indirectly_writable
    const ivblock_ref& operator=(const ivblock_ref& other) const {
      return assign_through(*this,other);
    }
    const ivblock_ref& operator=(ivblock_ref&& other) const {
      return assign_through(*this,other);
    }

    // assign from other range types
    template<class Range>
      requires (!std::is_same_v<std::remove_cvref_t<Range>,ivblock_ref>)
    ivblock_ref& operator=(Range&& other) {
      return assign_through(*this,other);
    }
    template<class Range>
      requires (!std::is_same_v<std::remove_cvref_t<Range>,ivblock_ref>)
    const ivblock_ref& operator=(Range&& other) const {
      return assign_through(*this,other);
    }

  // range interface
    constexpr auto size() const -> I { return *ptr; }

    constexpr auto data ()       ->       I* { return ptr       ; }
    constexpr auto data () const -> const I* { return ptr       ; }
    constexpr auto begin()       ->       I* { return ptr+1       ; }
    constexpr auto begin() const -> const I* { return ptr+1       ; }
    constexpr auto end()         ->       I* { return ptr+1+size(); }
    constexpr auto end()   const -> const I* { return ptr+1+size(); }

    template<class Integer> constexpr auto operator[](Integer i)       ->       I& { return ptr[1+i]; }
    template<class Integer> constexpr auto operator[](Integer i) const -> const I& { return ptr[1+i]; }
  private:
    template<class ivblock_ref_type, class Rng> static constexpr auto
    assign_through(ivblock_ref_type& x, const Rng& y) -> ivblock_ref_type& {
      *x.ptr = y.size();
      std::copy_n(y.begin(), y.size(), x.ptr+1);
      return x;
    }
};
template<class I0, class I1> auto
operator==(const ivblock_ref<I0>& x, const ivblock_ref<I1>& y) -> bool {
  return std::equal(x.data(),x.data()+1+x.size(),y.data(),y.data()+1+y.size());
}
template<class I0, class I1> auto
operator!=(const ivblock_ref<I0>& x, const ivblock_ref<I1>& y) -> bool {
  return !(x == y);
}
template<class I0, class I1> auto
operator< (const ivblock_ref<I0>& x, const ivblock_ref<I1>& y) -> bool {
  return  std::lexicographical_compare(x.data(),x.data()+1+x.size(),y.data(),y.data()+1+y.size());
}
template<class I0, class I1> auto
operator> (const ivblock_ref<I0>& x, const ivblock_ref<I1>& y) -> bool {
  return y<x;
}
template<class I0, class I1> auto
operator<=(const ivblock_ref<I0>& x, const ivblock_ref<I1>& y) -> bool {
  return !(x>y);
}
template<class I0, class I1> auto
operator>=(const ivblock_ref<I0>& x, const ivblock_ref<I1>& y) -> bool {
  return !(x<y);
}

template<class I> constexpr auto begin(const ivblock_ref<I>& x) { return x.begin(); }
template<class I> constexpr auto begin(      ivblock_ref<I>& x) { return x.begin(); }
template<class I> constexpr auto end  (const ivblock_ref<I>& x) { return x.end  (); }
template<class I> constexpr auto end  (      ivblock_ref<I>& x) { return x.end  (); }

template<class I> inline auto
to_string(const ivblock_ref<I>& x) {
  return std_e::range_to_string(x);
}

template<class I0, class I1> auto
operator==(const ivblock_ref<I0>& x, const std::vector<I1>& y) -> bool {
  return std::equal( x.begin() , x.end() , y.begin() , y.end() );
}
template<class I0, class I1> auto
operator==(const std::vector<I1>& x, const ivblock_ref<I0>& y) -> bool {
  return y==x;
}
template<class I0, class I1> auto
operator!=(const ivblock_ref<I0>& x, const std::vector<I1>& y) -> bool {
  return !(x==y);
}
template<class I0, class I1> auto
operator!=(const std::vector<I1>& x, const ivblock_ref<I0>& y) -> bool {
  return !(x==y);
}


} // std_e
