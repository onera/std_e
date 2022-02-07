#pragma once


// REF CppCon 2015: Chandler Carruth
//     Tuning C++: Benchmarks, and CPUs, and Compilers! Oh My!
//     https://youtu.be/nXaxk27zwlk


namespace std_e {


// don't let the compiler optimize uses of p
inline auto
do_not_optimize(void* p) -> void {
  asm volatile("" : : "g"(p) : "memory");
}

// do as if all memory was read by this call
inline auto
clobber_memory() -> void {
  asm volatile("" : : : "memory");
}


} // std_e
