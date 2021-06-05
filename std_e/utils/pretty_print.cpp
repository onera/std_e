#include "std_e/utils/pretty_print.hpp"

#include "std_e/future/make_array.hpp"
#include <sstream>


namespace std_e {


const auto bytes_unit_suffix = std_e::make_array<std::string>("B", "KB", "MB", "GB", "TB");
const int n_byte_units = bytes_unit_suffix.size();

auto
pretty_print_bytes(int64_t bytes) -> std::string {
  int i = 0;
  double dbl_bytes = bytes;
  while ((bytes /= 1024)>0 && i<n_byte_units-1) {
    dbl_bytes /= 1024.;
    ++i;
  }

  std::ostringstream oss;
  oss << dbl_bytes;
  return oss.str()+ " " + bytes_unit_suffix[i];
}


} // std_e
