#include "std_e/base/system/mem_usage.hpp"


#include <sys/sysinfo.h>
#include <malloc.h>
#include "std_e/utils/file.hpp"
#include "std_e/utils/string.hpp"
#include "std_e/future/contract.hpp"


namespace std_e {


auto
remaining_system_memory_in_bytes() -> long {
  struct sysinfo mem_info;
  sysinfo(&mem_info);

  return mem_info.freeram;
}


auto
malloc_mem_info() -> void {
  malloc_stats(); // NOTE: see also mallinfo()
}


auto
resident_memory_in_bytes() -> long {
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
}

} // std_e
