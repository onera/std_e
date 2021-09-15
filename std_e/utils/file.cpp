#include "std_e/utils/file.hpp"
#include <fstream>


namespace std_e {


auto
file_to_string(const std::string& file_name) -> std::string {
  std::ifstream t(file_name);

  t.seekg(0, std::ios::end);
  size_t size = t.tellg();

  std::string buffer(size, ' ');
  t.seekg(0);
  t.read(&buffer[0], size); 
  return buffer;
}


} // std_e
