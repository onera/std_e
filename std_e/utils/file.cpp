#include "std_e/utils/file.hpp"
#include <fstream>
#include <limits>


namespace std_e {


auto
file_to_string(const std::string& file_name) -> std::string {
  std::ifstream t(file_name);

  t.seekg(0, std::ios::end);
  size_t size = t.tellg();

  // if the size is unreasonable, it may be because we are reading a virtual file (UNIX)
  if (size > std::numeric_limits<size_t>::max()/8) {
    return virtual_file_to_string(file_name);
  }

  std::string buffer(size, ' ');
  t.seekg(0);
  t.read(&buffer[0], size);
  return buffer;
}


auto
virtual_file_to_string(const std::string& file_name) -> std::string {
  std::ifstream file(file_name);

  std::string buffer;
  std::string line;
  while (std::getline(file, line)) {
    buffer += line + "\n";
  }

  return buffer;
}


} // std_e
