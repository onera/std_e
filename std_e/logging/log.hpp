#pragma once

#include <memory>
#include "std_e/utils/enum.hpp"
#include "std_e/logging/printer.hpp"


namespace std_e {


// TODO DELETE, too complicated (just use on/off, and create as many logger as you want)
STD_E_ENUM(logging_level,
  off,
  explicit_call,
  fatal,
  error,
  warn,
  info,
  debug,
  full
);


struct logger {
  // types
  using printers_type = std::vector<std::unique_ptr<printer>>;

  // attributes
  std::string name;
  printers_type printers;
  bool active = true;

  // ctors
  logger() = default;
  //logger(const logger&) = delete;
  //logger& operator=(const logger&) = delete;
  logger(const logger&) { throw; }
  logger& operator=(const logger&) { throw; }
  logger(logger&&) = default;
  logger& operator=(logger&&) = default;

  logger(std::string name)
    : name(std::move(name))
  {}
  logger(std::string name, std::unique_ptr<printer> printer)
    : logger(std::move(name))
  {
    printers.emplace_back(std::move(printer));
  }
  logger(std::string name, printers_type&& printers)
    : name(std::move(name))
    , printers(std::move(printers))
  {}
};

auto has_logger(const std::string& name) -> bool;
auto get_logger(const std::string& name) -> logger&;

auto add_logger(std::string name) -> logger&;
auto add_logger(std::string name, std::unique_ptr<printer> printer) -> logger&;

auto add_logger_if_absent(std::string name, std::unique_ptr<printer> printer) -> logger&;
auto add_or_replace_logger(logger l) -> logger&;

auto add_printer_to_logger(const std::string& logger_name, std::unique_ptr<printer> printer) -> void;

auto turn_on(const std::string& logger_name) -> void;
auto turn_off(const std::string& logger_name) -> void;


auto log(const std::string& log_name, const std::string& msg) -> void;


auto add_stdout_printer(const std::string& logger_name) -> void;
auto add_file_printer(const std::string& logger_name, const std::string& file_name) -> void;


} // std_e
