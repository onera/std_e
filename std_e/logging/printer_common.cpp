#include "std_e/logging/printer_common.hpp"
#include "std_e/logging/log.hpp"
#include <memory>

namespace std_e {


auto
add_stdout_printer(const std::string& logger_name) -> void {
  add_printer_to_logger(logger_name, stdout_printer{});
}
auto
add_file_printer(const std::string& logger_name, const std::string& file_name) -> void {
  add_printer_to_logger(logger_name, file_printer{file_name});
}
auto
add_recording_printer(const std::string& logger_name, std::string* s_ptr) -> void {
  add_printer_to_logger(logger_name, recording_printer{s_ptr});
}



} // std_e
