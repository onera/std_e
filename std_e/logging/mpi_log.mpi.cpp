#include "std_e/logging/mpi_log.hpp"

namespace std_e {


auto
add_mpi_stdout_printer(const std::string& logger_name) -> void {
  add_printer_to_logger(logger_name, std::make_unique<mpi_stdout_printer>());
}
auto
add_mpi_rank_0_stdout_printer(const std::string& logger_name) -> void {
  add_printer_to_logger(logger_name, std::make_unique<mpi_rank_0_stdout_printer>());
}
auto
add_mpi_file_printer(const std::string& logger_name, const std::string& file_name) -> void {
  add_printer_to_logger(logger_name, std::make_unique<mpi_file_printer>(file_name));
}


} // std_e
