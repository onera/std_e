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

template<class Array, class T = Array::value_type>
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

using cleanup_function = void(*)(void* );

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
      requires
        (   !std::is_same_v< polymorphic_array , std::decay_t<Array> >
         &&  std::is_same_v< typename std::decay_t<Array>::value_type , T >
         &&  std::is_rvalue_reference_v< Array&& > )
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

    // TODO move out of here
    static auto
    cleanup_fn_impl(void* ptr) -> void {
      auto* type_ptr = static_cast<detail::internal_array_base<T>*>(ptr);
      delete type_ptr;
    }
    auto
    cleanup_fn() const -> cleanup_function {
      return cleanup_fn_impl;
    }

  private:
    std::unique_ptr<detail::internal_array_base<T>> impl;
};

template<class T> auto
operator==(const polymorphic_array<T>& x, const polymorphic_array<T>& y) -> bool {
  if (x.is_null() && y.is_null()) return true;
  if (x.is_null()) return false;
  if (y.is_null()) return false;
  return x.as_span() == y.as_span();
}
template<class T> auto
operator!=(const polymorphic_array<T>& x, const polymorphic_array<T>& y) -> bool {
  return !(x==y);
}



} // std_e
