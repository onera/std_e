#pragma once


namespace std_e {


/** 
 * Similar to std::reference_wrapper, but default-constructible
*/
template<class T>
class reference_wrapper {
  public:
  // ctors
    using type = T;
   
    constexpr
    reference_wrapper() noexcept = default;

    constexpr
    reference_wrapper(T& x) noexcept
      : ptr(&x)
    {}

    constexpr
    reference_wrapper(const reference_wrapper&) noexcept = default;
    constexpr auto
    operator=(const reference_wrapper& x) noexcept -> reference_wrapper& = default;
   
  // access
    constexpr
    operator const T& () const noexcept { return *ptr; }
    constexpr
    operator T& () noexcept { return *ptr; }

    constexpr auto
    get() const noexcept -> const T& { return *ptr; }
    constexpr auto
    get() noexcept -> T& { return *ptr; }

    constexpr auto
    get_pointer() const noexcept -> const T* { return ptr; }
    constexpr auto
    get_pointer() noexcept -> T* { return ptr; }


  private:
    T* ptr;
};

template<class T> constexpr auto
get(const reference_wrapper<T>& x) noexcept {
  return x.get();
}
template<class T> constexpr auto
get(reference_wrapper<T>& x) noexcept {
  return x.get();
}
template<class T> constexpr auto
get_pointer(const reference_wrapper<T>& x) noexcept {
  return x.get_pointer();
}
template<class T> constexpr auto
get_pointer(reference_wrapper<T>& x) noexcept {
  return x.get_pointer();
}


} // std_e
