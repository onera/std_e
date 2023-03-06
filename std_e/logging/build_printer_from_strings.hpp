#pragma once

#include "std_e/logging/printer.hpp"
#include "std_e/logging/configure_loggers_exception.hpp"
#include <memory>
#include <vector>


namespace std_e {


template<class printer_type> auto
_build_printer_impl_0_arg(
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
_build_printer_impl_1_arg(
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
build_printer(const std::string& printer_name, const std::vector<std::string>& printer_args, const std::string& error_msg_context) -> std::unique_ptr<printer>;


} // std_e
