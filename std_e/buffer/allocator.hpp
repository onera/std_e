#pragma once


#include <cstdlib>
#include "std_e/buffer/base.hpp"


namespace std_e {


template<allocator_function alloc, deallocator_function dealloc>
struct buffer_allocator {
  static constexpr auto alloc_function = alloc;
  static constexpr auto dealloc_function = dealloc;

  static auto
  allocate(size_t n) -> void* {
    return alloc(n);
  }
  static auto
  deallocate(void* ptr) -> void {
    dealloc(ptr);
  }
};


template<auto a, auto d> inline auto
operator==(const buffer_allocator<a,d>& x, const buffer_allocator<a,d>& y) -> bool {
  return true; // no state
}
template<auto a, auto d> inline auto
operator!=(const buffer_allocator<a,d>& x, const buffer_allocator<a,d>& y) -> bool {
  return !(x==y);
}


using buffer_mallocator = buffer_allocator<malloc,free>;


} // std_e
