#pragma once


#include <cstdlib>
#include <algorithm>


namespace std_e {


struct mallocator {
  static auto allocate(size_t n) -> void* {
    return malloc(n);
  }
  static auto deallocate(void* p) -> void {
    free(p);
  }

  template<class T>
  static auto fill(T* first, T* last, const T& x) {
    std::fill(first, last, x);
  }
  //template<class T>
  //static auto copy(const T* first, const T* last, T* d_first) {
  template<class In_it, class S, class Out_it>
  static auto copy(In_it first, S last, Out_it d_first) {
    std::copy(first, last, d_first);
  }
};

//struct aligned_mallocator {
//  static auto allocate(size_t n) -> void* {
//    void* p;
//    posix_memalign(&p, 64, n);
//    return p;
//  }
//  static auto deallocate(void* p) -> void {
//    free(p);
//  }
//};


} // std_e
