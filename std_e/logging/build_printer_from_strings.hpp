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
_build_printer_impl_1_string(
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
    auto& quoted_arg = printer_args[0];
    if (quoted_arg.size()<2) {
      throw configure_loggers_exception(error_msg_context + ": string argument " + quoted_arg + " must be surrounded by matching single or double quotes");
    }
    bool has_single_quotes = quoted_arg[0] == '\'' && quoted_arg.back() == '\'';
    bool has_double_quotes = quoted_arg[0] == '"'  && quoted_arg.back() == '"' ;
    bool has_matching_quotes = has_single_quotes || has_double_quotes;
    if (!has_matching_quotes) {
      throw configure_loggers_exception(error_msg_context + ": string argument " + quoted_arg + " must be surrounded by matching single or double quotes");
    }
    std::string printer_name(begin(quoted_arg)+1,end(quoted_arg)-1);
    if (printer_name.size() == 0) {
      throw configure_loggers_exception(error_msg_context + ": empty string argument");
    }
    return std::make_unique<printer_type>(quoted_arg);
  }
}


auto
build_printer(const std::string& printer_name, const std::vector<std::string>& printer_args, const std::string& error_msg_context) -> std::unique_ptr<printer>;

auto
add_printer(const std::string& logger_name, const std::string& printer_type, const std::vector<std::string>& printer_args) -> void;


} // std_e
