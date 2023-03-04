#include "std_e/logging/printer_common.hpp"
#include "std_e/logging/log.hpp"
#include <memory>

namespace std_e {


auto
add_stdout_printer(const std::string& logger_name) -> void {
  add_printer_to_logger(logger_name, stdout_printer());
}
auto
add_file_printer(const std::string& logger_name, const std::string& file_name) -> void {
  add_printer_to_logger(logger_name, file_printer(file_name));
}


} // std_e
