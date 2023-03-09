#include "std_e/logging/printer_mpi.hpp"
#include "std_e/logging/log.hpp"
#include <memory>

namespace std_e {


auto
add_mpi_stdout_printer(const std::string& logger_name) -> void {
  add_printer_to_logger(logger_name, mpi_stdout_printer{});
}
auto
add_mpi_rank_0_stdout_printer(const std::string& logger_name) -> void {
  add_printer_to_logger(logger_name, mpi_rank_0_stdout_printer{});
}
auto
add_mpi_rank_0_stderr_printer(const std::string& logger_name) -> void {
  add_printer_to_logger(logger_name, mpi_rank_0_stderr_printer{});
}
auto
add_mpi_file_printer(const std::string& logger_name, const std::string& file_name) -> void {
  add_printer_to_logger(logger_name, mpi_file_printer{file_name});
}


} // std_e
