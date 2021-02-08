#pragma once


#include "std_e/buffer/base.hpp"


namespace std_e {


class owning_buffer {
  private:
    void* ptr;
    deallocator_function dealloc;
  public:
  // ctors / assign / dtor
    owning_buffer(void* ptr, deallocator_function dealloc)
      : ptr(ptr)
      , dealloc(dealloc)
    {}

    owning_buffer(const owning_buffer&) = delete;
    owning_buffer& operator=(const owning_buffer&) = delete;

    owning_buffer(owning_buffer&& old)
      : ptr(old.ptr)
      , dealloc(old.dealloc)
    {
      old.dealloc = nullptr;
    }
    owning_buffer& operator=(owning_buffer&& old) {
      ptr = old.ptr;
      dealloc = old.dealloc;
      old.dealloc = nullptr;
      return *this;
    }

    ~owning_buffer() {
      if (is_owner()) {
        dealloc(ptr);
      }
      dealloc = nullptr;
    }

  // buffer interface
    auto is_owner() const -> bool {
      return dealloc != nullptr;
    }
    auto release() -> deallocator_function {
      auto dealloctor = dealloc;
      dealloc = nullptr;
      return dealloctor;
    }

    auto data()       ->       void* { return ptr; }
    auto data() const -> const void* { return ptr; }
};


} // std_e
