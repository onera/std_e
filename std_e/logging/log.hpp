#pragma once

#include <memory>
#include "std_e/utils/enum.hpp"
#include "std_e/logging/printer.hpp"


namespace std_e {


struct logger {
  // types
  using printers_type = std::vector<std::unique_ptr<printer>>;

  // attributes
  std::string name;
  printers_type printers;
  bool active = true;

  // ctors
  logger() = default;
  logger(const logger&) = delete;
  logger& operator=(const logger&) = delete;
  logger(logger&&) = default;
  logger& operator=(logger&&) = default;

  logger(std::string name)
    : name(std::move(name))
  {}
  template<class Printer>
  logger(std::string name, Printer p)
    : logger(std::move(name))
  {
    add_printer(std::move(p));
  }
  logger(std::string name, printers_type&& printers)
    : name(std::move(name))
    , printers(std::move(printers))
  {}

  // member function
  template<class Printer> auto
  add_printer(Printer p) -> void {
    auto p_ptr = std::make_unique<Printer>(std::move(p));
    printers.emplace_back(std::move(p_ptr));
  }
};


auto has_logger(const std::string& name) -> bool;
auto get_logger(const std::string& name) -> logger&;


auto add_logger(logger l, bool replace) -> logger&;
auto add_or_replace_logger(logger l) -> logger&;
auto add_logger_if_absent(logger l) -> logger&;


template<class Printer> auto
add_printer_to_logger(const std::string& logger_name, Printer p) -> void {
  auto& l = get_logger(logger_name);
  l.add_printer(std::move(p));
}

auto turn_on(const std::string& logger_name) -> void;
auto turn_off(const std::string& logger_name) -> void;


auto log(const std::string& log_name, const std::string& msg) -> void;


auto add_stdout_printer(const std::string& logger_name) -> void;
auto add_file_printer(const std::string& logger_name, const std::string& file_name) -> void;


} // std_e
