#pragma once


#include "std_e/buffer/base.hpp"
#include <type_traits>


namespace std_e {


class buffer_span {
  private:
    void* ptr;
  public:
    buffer_span(void* ptr)
      : ptr(ptr)
    {}
    template<class Range>
    buffer_span(Range& rng)
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
