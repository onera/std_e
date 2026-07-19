#include "std_e/base/system/mem_usage.hpp"


#if defined(__linux__)
  #include <sys/sysinfo.h>
  #include <malloc.h>
#elif defined(__APPLE__)
  #include <malloc/malloc.h>
  #include <mach/mach.h>
#endif
#include "std_e/utils/file.hpp"
#include "std_e/utils/string.hpp"
#include "std_e/future/contract.hpp"


namespace std_e {


auto
remaining_system_memory_in_bytes() -> long {
#if defined(__linux__)
  struct sysinfo mem_info;
  sysinfo(&mem_info);

  return mem_info.freeram;
#elif defined(__APPLE__)
  mach_port_t host = mach_host_self();
  vm_size_t page_size;
  host_page_size(host, &page_size);

  vm_statistics64_data_t vm_stats;
  mach_msg_type_number_t count = HOST_VM_INFO64_COUNT;
  if (host_statistics64(host, HOST_VM_INFO64, (host_info64_t)&vm_stats, &count) != KERN_SUCCESS) {
    throw std_e::msg_exception("host_statistics64 failed");
  }

  return static_cast<long>(vm_stats.free_count) * static_cast<long>(page_size);
#else
  throw std_e::msg_exception("remaining_system_memory_in_bytes not implemented on this platform");
#endif
}


auto
malloc_mem_info() -> void {
  #if defined(__GLIBC__)
    malloc_stats(); // NOTE: see also mallinfo()
  #elif defined(__APPLE__)
    malloc_zone_print(nullptr, 0);
  #else
    throw std_e::msg_exception("malloc_stats not implemented on non-glibc system");
  #endif
}


auto
resident_memory_in_bytes() -> long {
#if defined(__linux__)
  auto file_name = "/proc/self/status";
  auto s = virtual_file_to_string(file_name);
  auto lines = split(s,'\n');
  auto rss_line = std::find_if(begin(lines),end(lines),[](const std::string& l){ return begins_with(l,"VmRSS"); });
  STD_E_ASSERT(rss_line != end(lines));
  auto tokens = split(*rss_line,' ');

  int n_tokens = tokens.size();
  STD_E_ASSERT(tokens[n_tokens-1] == "kB");

  auto size_in_kb = tokens[n_tokens-2];
  return std::atol(size_in_kb.c_str())*1024;
#elif defined(__APPLE__)
  mach_task_basic_info_data_t info;
  mach_msg_type_number_t count = MACH_TASK_BASIC_INFO_COUNT;
  if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &count) != KERN_SUCCESS) {
    throw std_e::msg_exception("task_info failed");
  }
  return static_cast<long>(info.resident_size);
#else
  throw std_e::msg_exception("resident_memory_in_bytes not implemented on this platform");
#endif
}

} // std_e
