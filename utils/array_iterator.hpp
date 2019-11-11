#pragma once


namespace std_e {


template<class T>
class array_iterator;

template<class T>
class array_iterator<T*> {
  public:
  // type traits
    using value_type = T;
    using iterator = array_iterator<T*>;
    using const_iterator = array_iterator<const T*>;
    using reference = T&;
    using const_reference = const T&;

  // ctors
    constexpr
    array_iterator()
      : ptr(nullptr)
      , index(0)
    {}
    constexpr
    array_iterator(T* ptr, int index)
      : ptr(ptr)
      , index(index)
    {}
    constexpr
    operator const_iterator () const noexcept { return {ptr,index}; }

  // accessors
    constexpr auto
    ptr_value() -> T* {
      return ptr+index;
    }
    constexpr auto
    ptr_value() const -> const T* {
      return ptr+index;
    }
    constexpr auto
    operator*() -> reference {
      return *ptr_value();
    }
    constexpr auto
    operator*() const -> const_reference {
      return *ptr_value();
    }
    constexpr auto
    operator->() const -> const T* {
      return ptr_value();
    }
    constexpr auto
    operator->() -> T* {
      return ptr_value();
    }
  private:
  public: // TODO
    T* ptr;
    int index;
};
template<class T0, class T1> constexpr auto
operator==(const array_iterator<T0*>& x, const array_iterator<T1*>& y) -> int {
  return x.ptr==y.ptr && x.index==y.index;
}
template<class T0, class T1> constexpr auto
operator!=(const array_iterator<T0*>& x, const array_iterator<T1*>& y) -> int {
  return !(x==y);
}
template<class T0, class T1> constexpr auto
operator<(const array_iterator<T0*>& x, const array_iterator<T1*>& y) -> int {
  return x.ptr_value() < y.ptr_value();
}
template<class T> constexpr auto
operator+(const array_iterator<T*>& x, int i) -> array_iterator<T*> {
  return {x.ptr,x.index+i};
}
template<class T> constexpr auto
operator-(const array_iterator<T*>& x, int i) -> array_iterator<T*> {
  return {x.ptr,x.index-i};
}
template<class T0, class T1> constexpr auto
operator-(const array_iterator<T0*>& x, const array_iterator<T1*>& y) -> int {
  STD_E_ASSERT(x.ptr==y.ptr); // only diff of pointers of same array is not UB
  return x.index-y.index;
}


} // std_e
