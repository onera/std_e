#pragma once


namespace std_e {


template<class T_ptr>
// requires T_ptr is a pointer
class memory_view {
  public:
  // type traits
    using value_type = std::remove_pointer_t<T_ptr>;
    using pointer = T_ptr;
    using const_pointer = const T_ptr;
    using reference = value_type&;
    using const_reference = const value_type&;

  // ctors
    FORCE_INLINE constexpr
    memory_view()
      : ptr(nullptr)
    {}

    FORCE_INLINE constexpr explicit
    memory_view(T_ptr ptr)
      : ptr(ptr)
    {}

    FORCE_INLINE constexpr memory_view(const memory_view&) = default;
    FORCE_INLINE constexpr memory_view(memory_view&&) = default;
    FORCE_INLINE constexpr memory_view& operator=(const memory_view&) = default;
    FORCE_INLINE constexpr memory_view& operator=(memory_view&&) = default;

    template<class T_ptr0> FORCE_INLINE constexpr // conversion from non-const to const
    // requires std::remove_const_t<T_ptr0> = T_ptr
    memory_view(const memory_view<T_ptr0>& mem)
      : ptr(mem.data())
    {}
    template<class T_ptr0> FORCE_INLINE constexpr // conversion from non-const to const
    // requires std::remove_const_t<T_ptr0> = T_ptr
    memory_view(memory_view<T_ptr0>&& mem)
      : ptr(mem.data())
    {}

  // accessors
    FORCE_INLINE constexpr auto
    data() -> T_ptr {
      return ptr;
    }
    FORCE_INLINE constexpr auto
    data() const -> const T_ptr {
      return ptr;
    }

    FORCE_INLINE constexpr auto
    begin() -> T_ptr {
      return ptr;
    }
    FORCE_INLINE constexpr auto
    begin() const -> const T_ptr {
      return ptr;
    }

    FORCE_INLINE constexpr auto
    operator[](int i) -> reference {
      return ptr[i];
    }
    FORCE_INLINE constexpr auto
    operator[](int i) const -> const_reference {
      return ptr[i];
    }
  private:
    T_ptr ptr;
};


template<class T_ptr> FORCE_INLINE constexpr auto
begin(memory_view<T_ptr>& x) {
  return x.begin();
}
template<class T_ptr> FORCE_INLINE constexpr auto
begin(const memory_view<T_ptr>& x) {
  return x.begin();
}


} // std_e
