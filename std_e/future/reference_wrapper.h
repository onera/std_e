#pragma once


namespace std_e {


/** 
 * Similar to std::reference_wrapper, 
 * but 
 *   - default-constructible to fix std_e::constexpr_vector ctors
 *   - stores an iterator, not necessarily a pointer
 *       and thus 
 *        - is templated by the iterator
 *        - is constructed from an iterator
*/
template<class Iterator>
class reference_wrapper {
  public:
  // ctors
    //using T = typename Iterator::value_type;
    using T = std::remove_pointer_t<std::decay_t<Iterator>>;
    using type = T;
    //using iterator = Iterator;
    //using const_iterator = typename Iterator::const_iterator;
    using iterator = Iterator;
    using const_iterator = const Iterator;
   
    constexpr
    reference_wrapper() noexcept
      : iter(Iterator{})
    {}

    constexpr
    reference_wrapper(Iterator iter) noexcept
      : iter(iter)
    {}

    constexpr
    reference_wrapper(const reference_wrapper&) noexcept = default;
    constexpr auto
    operator=(const reference_wrapper& x) noexcept -> reference_wrapper& = default;
   
  // access
    constexpr
    operator const T& () const noexcept { return *iter; }
    constexpr
    operator T& () noexcept { return *iter; }

    constexpr auto
    get() const noexcept -> const T& { return *iter; }
    constexpr auto
    get() noexcept -> T& { return *iter; }

    constexpr auto
    get_iterator() const noexcept -> const_iterator { return iter; }
    constexpr auto
    get_iterator() noexcept -> iterator { return iter; }


  private:
    Iterator iter;
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
get_iterator(const reference_wrapper<T>& x) noexcept {
  return x.get_iterator();
}
template<class T> constexpr auto
get_iterator(reference_wrapper<T>& x) noexcept {
  return x.get_iterator();
}


} // std_e
