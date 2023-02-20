#include "std_e/logging/build_printer_from_strings.hpp"
#include "std_e/logging/configure_loggers_exception.hpp"
#include "std_e/logging/printer_common.hpp"
#include "std_e/logging/printer_mpi.hpp"


namespace std_e {


template<class printer_type> auto
build_printer_impl_0_arg(
  const std::string& printer_name,
  const std::vector<std::string>& printer_args,
  const std::string& error_msg_context
)
 -> std::unique_ptr<printer>
{
  int n_args = printer_args.size();

  if (n_args>0) {
    throw configure_loggers_exception(error_msg_context + ": " + printer_name + " needs 0 argument, but " + std::to_string(n_args) + " given");
  } else {
    return std::make_unique<printer_type>();
  }
}

template<class printer_type> auto
build_printer_impl_1_arg(
  const std::string& printer_name,
  const std::vector<std::string>& printer_args,
  const std::string& error_msg_context
)
 -> std::unique_ptr<printer>
{
  int n_args = printer_args.size();

  if (n_args != 1) {
    throw configure_loggers_exception(error_msg_context + ": " + printer_name + " needs 1 argument, but " + std::to_string(n_args) + " given");
  } else {
    return std::make_unique<printer_type>(printer_args[0]);
  }
}

auto
build_printer(const std::string& printer_name, const std::vector<std::string>& printer_args, const std::string& error_msg_context) -> std::unique_ptr<printer> {
  if (printer_name == "stdout_printer") {
    return build_printer_impl_0_arg<stdout_printer>(printer_name,printer_args,error_msg_context);
  }
  if (printer_name == "mpi_stdout_printer") {
    return build_printer_impl_0_arg<mpi_stdout_printer>(printer_name,printer_args,error_msg_context);
  }
  if (printer_name == "mpi_rank_0_stdout_printer") {
    return build_printer_impl_0_arg<mpi_stdout_printer>(printer_name,printer_args,error_msg_context);
  }

  if (printer_name == "file_printer") {
    return build_printer_impl_1_arg<file_printer>(printer_name,printer_args,error_msg_context);
  }
  if (printer_name == "mpi_file_printer") {
    return build_printer_impl_1_arg<mpi_file_printer>(printer_name,printer_args,error_msg_context);
  }
  else {
    throw configure_loggers_exception(error_msg_context + ": unknown printer \"" + printer_name + "\"");
  }
}


} // std_e
