#pragma once

#include "std_e/buffer/base.hpp"
#include <memory>


namespace std_e {


namespace detail {

struct internal_base {
  virtual auto is_owner() const -> bool = 0;
  virtual auto release() -> deallocator_function = 0;

  virtual auto data()       ->       void* = 0;
  virtual auto data() const -> const void* = 0;

  virtual ~internal_base() {}
};

template<class Buffer>
struct internal_impl: internal_base {
  public:
    internal_impl(Buffer buf)
      : buf(std::move(buf))
    {}

    auto is_owner() const -> bool override {
      return buf.is_owner();
    }
    auto release() -> deallocator_function override {
      return buf.release();
    }

    auto data()       ->       void* override {
      return buf.data();
    }
    auto data() const -> const void* override {
      return buf.data();
    }
  private:
    Buffer buf;
};

} // detail


struct null_buffer {
  auto is_owner() const -> bool { return false; }
  auto release() const -> deallocator_function { return nullptr; }
  auto data() const -> void* { return nullptr; }
};

class polymorphic_buffer {
  public:
    polymorphic_buffer()
      : polymorphic_buffer(null_buffer{})
    {}

    template<
      class Buffer,
      std::enable_if_t<!std::is_same_v<polymorphic_buffer, std::decay_t<Buffer>>, int> =0
    >
    polymorphic_buffer(Buffer&& x):
      impl(std::make_unique<detail::internal_impl<std::decay_t<Buffer>>>(std::forward<Buffer>(x)))
    {}

    auto is_owner() const -> bool {
      return impl->is_owner();
    }
    auto release() -> deallocator_function {
      return impl->release();
    }

    auto data()       ->       void* {
      return impl->data();
    }
    auto data() const -> const void* {
      return impl->data();
    }
  private:
    std::unique_ptr<detail::internal_base> impl;
};


} // std_e
