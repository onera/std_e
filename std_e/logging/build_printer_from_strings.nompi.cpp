#include "std_e/logging/build_printer_from_strings.hpp"
#include "std_e/logging/printer_common.hpp"


namespace std_e {


auto
build_printer(const std::string& printer_name, const std::vector<std::string>& printer_args, const std::string& error_msg_context) -> std::unique_ptr<printer> {
  if (printer_name == "stdout_printer") {
    return _build_printer_impl_0_arg<stdout_printer>(printer_name,printer_args,error_msg_context);
  }

  if (printer_name == "file_printer") {
    return _build_printer_impl_1_arg<file_printer>(printer_name,printer_args,error_msg_context);
  }
  else {
    throw configure_loggers_exception(error_msg_context + ": unknown printer \"" + printer_name + "\"");
  }
}


} // std_e
