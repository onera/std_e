
// ------------------------------------------------------------------
// External include
#include <iostream>
#include <filesystem>
#include <unistd.h>
// ------------------------------------------------------------------

// ------------------------------------------------------------------
// Internal include
#include "std_e/utils/pretty_print.hpp"
// ------------------------------------------------------------------

namespace std_e {

// ------------------------------------------------------------------
const std::array<std::string, 5> bytes_unit_suffix = {"B", "KB", "MB", "GB", "TB"};

// ------------------------------------------------------------------
std::string pretty_print_bytes(int64_t bytes)
{
  std::string s;
  int i = 0;
  double dbl_bytes = bytes;

  if (bytes > 1024) {
    for (i = 0; (bytes / 1024) > 0 && i < static_cast<int>(bytes_unit_suffix.size()); i++, bytes /= 1024){
      dbl_bytes = bytes / 1024.0;
    }
  }
  std::ostringstream oss;
  oss << dbl_bytes << " " << bytes_unit_suffix[i];
  s += oss.str();
  return s;
}

}  // end of namespace ----------------------------------------------------

