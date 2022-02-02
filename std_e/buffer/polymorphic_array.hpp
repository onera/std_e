#pragma once

#include <memory>
#include "std_e/future/span.hpp"
#include "std_e/buffer/base.hpp"
#include "std_e/log.hpp"


namespace std_e {


namespace detail {

template<class T>
struct internal_array_base {
  virtual auto data()       ->       T* = 0;
  virtual auto data() const -> const T* = 0;

  virtual auto size() const -> size_t = 0;

  virtual ~internal_array_base() {}
};

template<class Array, class T = typename Array::value_type>
struct internal_array_impl: internal_array_base<T> {
  public:
    constexpr
    internal_array_impl(Array arr)
      : arr(std::move(arr))
    {}

    auto data()       ->       T* override {
      return arr.data();
    }
    auto data() const -> const T* override {
      return arr.data();
    }

    auto size() const -> size_t override {
      return arr.size();
    }
  private:
    Array arr;
};

} // detail

using cleanup_function = void(*)(void*);

template<class T>
class polymorphic_array {
  public:
    using value_type = T;
    using iterator = T*;
    using const_iterator = const T*;
    using reference = T&;
    using const_reference = const T&;

  // ctor
    constexpr
    polymorphic_array()
      : impl(nullptr)
    {}
    polymorphic_array(const polymorphic_array&) = delete;
    polymorphic_array& operator=(const polymorphic_array&) = delete;
    polymorphic_array(polymorphic_array&&) = default;
    polymorphic_array& operator=(polymorphic_array&&) = default;

    // ctor from any contiguous range
    template<class Array>
      #if __cplusplus > 201703L
      requires
        (   !std::is_same_v< polymorphic_array , std::decay_t<Array> >
         &&  std::is_same_v< typename std::decay_t<Array>::value_type , T >
         &&  std::is_rvalue_reference_v< Array&& > )
      #endif
    polymorphic_array(Array&& x)
      : impl(std::make_unique<detail::internal_array_impl<Array>>(std::move(x)))
    {}

    // specialization for span, since span can be efficiently passed-by-value (no need to force move)
    polymorphic_array(span<T> x)
      : impl(std::make_unique<detail::internal_array_impl<span<T>>>(x))
    {}

    // specialization for vector (for clearer error messages)
    polymorphic_array(std::vector<T>&& x)
      : impl(std::make_unique<detail::internal_array_impl<std::vector<T>>>(std::move(x)))
    {}

    auto is_null() const -> bool {
      return impl == nullptr;
    }

    auto data()       ->       T* {
      return impl->data();
    }
    auto data() const -> const T* {
      return impl->data();
    }

    auto size() const -> size_t {
      return impl->size();
    }

    auto operator[](int i) -> T& {
      return data()[i];
    }
    auto operator[](int i) const -> const T& {
      return data()[i];
    }

    auto as_span() -> span<T> {
      return std_e::make_span(data(),size());
    }
    auto as_span() const -> span<const T> {
      return std_e::make_span(data(),size());
    }

    using polymorphic_base = typename detail::internal_array_base<T>;
    auto release() -> polymorphic_base* {
      return impl.release();
    }

    static auto
    cleanup_fn() -> cleanup_function {
      return cleanup_fn_impl;
    }
  private:
  // function
    static auto
    cleanup_fn_impl(void* ptr) -> void {
      auto* type_ptr = static_cast<detail::internal_array_base<T>*>(ptr);
      delete type_ptr;
    }
  // data member
    std::unique_ptr<detail::internal_array_base<T>> impl;
};


// comparisons {
/// with other polymorphic_array {
template<class T0, class T1> auto
operator==(const polymorphic_array<T0>& x, const polymorphic_array<T1>& y) -> bool {
  if (x.is_null() && y.is_null()) return true;
  if (x.is_null()) return false;
  if (y.is_null()) return false;
  return x.as_span() == y.as_span();
}
template<class T0, class T1> auto
operator!=(const polymorphic_array<T0>& x, const polymorphic_array<T1>& y) -> bool {
  return !(x==y);
}
/// with other polymorphic_array }

/// with vector and span {
template<class T0, class T1> auto
operator==(const polymorphic_array<T0>& x, const span<T1>& y) -> bool {
  if (x.is_null()) return false;
  return x.as_span() == y;
}
template<class T0, class T1> auto
operator==(const span<T1>& x, const polymorphic_array<T0>& y) -> bool {
  return y==x;
}
template<class T0, class T1> auto
operator==(const polymorphic_array<T0>& x, const std::vector<T1>& y) -> bool {
  return x.as_span() == make_span(y);
}
template<class T0, class T1> auto
operator==(const std::vector<T1>& x, const polymorphic_array<T0>& y) -> bool {
  return y==x;
}

template<class T0, class T1> auto
operator!=(const polymorphic_array<T0>& x, const span<T1>& y) -> bool {
  return !(x==y);
}
template<class T0, class T1> auto
operator!=(const span<T1>& x, const polymorphic_array<T0>& y) -> bool {
  return !(x==y);
}
template<class T0, class T1> auto
operator!=(const polymorphic_array<T0>& x, const std::vector<T1>& y) -> bool {
  return !(x==y);
}
template<class T0, class T1> auto
operator!=(const std::vector<T1>& x, const polymorphic_array<T0>& y) -> bool {
  return !(x==y);
}
/// with vector and span }
// comparisons }




} // std_e
