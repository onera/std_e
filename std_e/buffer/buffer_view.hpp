#pragma once


#include "std_e/buffer/base.hpp"
#include <type_traits>


namespace std_e {


/// buffer_view can be thought of as a type-erased std::span
class buffer_view {
  private:
    void* ptr;
  public:
    buffer_view(void* ptr)
      : ptr(ptr)
    {}
    template<class Range>
    buffer_view(Range& rng)
      : ptr(rng.data())
    {
      static_assert(!std::is_const_v<typename Range::value_type>,"buffer is not const");
    }

    auto is_owner() const -> bool { return false; }
    auto release() -> deallocator_function { return nullptr; }

    auto data()       ->       void* { return ptr; }
    auto data() const -> const void* { return ptr; }
};


} // std_e
