#pragma once


#if defined(__linux__)
  #include <sys/sysinfo.h>
  #include <malloc.h>
#elif defined(__APPLE__)
  #include <malloc/malloc.h>
#endif
#include "std_e/utils/file.hpp"


namespace std_e {


auto remaining_system_memory_in_bytes() -> long;
auto malloc_mem_info() -> void;
auto resident_memory_in_bytes() -> long;


} // std_e
