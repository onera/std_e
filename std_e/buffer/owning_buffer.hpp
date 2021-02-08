#pragma once


#include "std_e/buffer/base.hpp"


namespace std_e {


template<deallocator_function dealloc>
class owning_buffer {
  private:
    void* ptr;
    bool owns;
  public:
  // ctors / assign / dtor
    owning_buffer(void* ptr)
      : ptr(ptr)
      , owns(true)
    {}

    owning_buffer(const owning_buffer&) = delete;
    owning_buffer& operator=(const owning_buffer&) = delete;

    owning_buffer(owning_buffer&& old)
      : ptr(old.ptr)
      , owns(old.owns)
    {
      old.owns = false;
    }
    owning_buffer& operator=(owning_buffer&& old) {
      ptr = old.ptr;
      owns = old.owns;
      old.owns = false;
      return *this;
    }

    ~owning_buffer() {
      if (is_owner()) {
        dealloc(ptr);
      }
      owns = false;
    }

  // buffer interface
    auto is_owner() const -> bool {
      return owns;
    }
    auto release() -> bool {
      bool was_owner = owns;
      owns = false;
      return was_owner;
    }
    auto deallocator() const -> deallocator_function { return dealloc; }

    auto data()       ->       void* { return ptr; }
    auto data() const -> const void* { return ptr; }
};


} // std_e
