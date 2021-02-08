#pragma once

#include "std_e/buffer/allocator.hpp"
#include <memory>


namespace std_e {


namespace pm_buf_alloc_detail {

struct internal_base {
  virtual auto allocate(size_t n) -> void* = 0;
  virtual auto deallocate(void* ptr) -> void = 0;

  virtual ~internal_base() {}
};

template<class Buffer_allocator>
struct internal_impl: internal_base {
  public:
    internal_impl(Buffer_allocator buf)
      : buf(std::move(buf))
    {}

    auto
    allocate(size_t n) -> void* override {
      return buf.allocate(n);
    }
    auto
    deallocate(void* ptr) -> void override {
      buf.deallocate(ptr);
    }
  private:
    Buffer_allocator buf;
};

} // pm_buf_alloc_detail


class polymorphic_buffer_allocator {
  public:
    polymorphic_buffer_allocator()
      : impl(nullptr)
    {}

    template<
      class Buffer_allocator,
      std::enable_if_t<!std::is_same_v<polymorphic_buffer_allocator, std::decay_t<Buffer_allocator>>, int> =0
    >
    polymorphic_buffer_allocator(Buffer_allocator&& x):
      impl(std::make_unique<pm_buf_alloc_detail::internal_impl<std::decay_t<Buffer_allocator>>>(std::forward<Buffer_allocator>(x)))
    {}

    auto allocate(size_t n) -> void* {
      return impl->allocate(n);
    }
    auto deallocate(void* ptr) -> void {
      return impl->deallocate(ptr);
    }
  private:
    std::unique_ptr<pm_buf_alloc_detail::internal_base> impl;
};


} // std_e
