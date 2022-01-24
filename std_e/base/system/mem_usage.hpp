#pragma once


#include <sys/sysinfo.h>
#include <malloc.h>
#include "std_e/utils/file.hpp"


namespace std_e {


auto remaining_system_memory_in_bytes() -> long;
auto malloc_mem_info() -> void;
auto resident_memory_in_bytes() -> long;


} // std_e
