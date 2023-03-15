#include "std_e/logging/build_printer_from_strings.hpp"
#include "std_e/logging/configure_loggers_exception.hpp"
#include "std_e/logging/log.hpp"
#include "std_e/logging/printer_common.hpp"
#if STD_E_HAS_MPI
  #include "std_e/logging/printer_mpi.hpp"
#endif


namespace std_e {


auto
build_printer(const std::string& printer_name, const std::vector<std::string>& printer_args, const std::string& error_msg_context) -> std::unique_ptr<printer> {
  // 0 arg
  if (printer_name == "stdout_printer") {
    return _build_printer_impl_0_arg<stdout_printer>(printer_name,printer_args,error_msg_context);
  }
  if (printer_name == "stderr_printer") {
    return _build_printer_impl_0_arg<stderr_printer>(printer_name,printer_args,error_msg_context);
  }
  #if STD_E_HAS_MPI
    if (printer_name == "mpi_stdout_printer") {
      return _build_printer_impl_0_arg<mpi_stdout_printer>(printer_name,printer_args,error_msg_context);
    }
    if (printer_name == "mpi_stderr_printer") {
      return _build_printer_impl_0_arg<mpi_stderr_printer>(printer_name,printer_args,error_msg_context);
    }
    if (printer_name == "mpi_rank_0_stdout_printer") {
      return _build_printer_impl_0_arg<mpi_rank_0_stdout_printer>(printer_name,printer_args,error_msg_context);
    }
    if (printer_name == "mpi_rank_0_stderr_printer") {
      return _build_printer_impl_0_arg<mpi_rank_0_stderr_printer>(printer_name,printer_args,error_msg_context);
    }
  #endif

  // 1 arg
  if (printer_name == "file_printer") {
    return _build_printer_impl_1_string<file_printer>(printer_name,printer_args,error_msg_context);
  }
  #if STD_E_HAS_MPI
    if (printer_name == "mpi_file_printer") {
      return _build_printer_impl_1_string<mpi_file_printer>(printer_name,printer_args,error_msg_context);
    }
  #endif
  else {
    throw configure_loggers_exception(error_msg_context + ": unknown printer \"" + printer_name + "\"");
  }
}


auto
add_printer(const std::string& logger_name, const std::string& printer_type, const std::vector<std::string>& printer_args) -> void {
  const std::string error_msg_context = "in add_printer";

  auto p = build_printer(printer_type, printer_args, error_msg_context);

  logger& l = get_logger(logger_name);
  l.printers.emplace_back(std::move(p));
}


} // std_e
