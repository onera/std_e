#pragma once

#include <iostream>
#include <fstream>
#include <memory>
#include "std_e/utils/enum.hpp"


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

// log_printer: base + common {
struct log_printer {
  virtual auto log(const std::string& msg) -> void = 0;
  virtual ~log_printer() = default;
};

class stdout_printer : public log_printer {
  public:
    ~stdout_printer() {
      std::cout << std::flush;
    }
    auto log(const std::string& msg) -> void override {
      std::cout << msg;
    }
};
class file_printer : public log_printer {
  public:
    file_printer() = default;
    file_printer(const std::string& file_name)
      : file(std::ofstream(file_name.c_str(), std::fstream::out))
    {}
    ~file_printer() {
      file << std::flush;
    }
    auto log(const std::string& msg) -> void override {
      file << msg;
    }
  private:
    std::ofstream file;
};
// log_printer: base + common }

struct logger {
  using printers_type = std::vector<std::unique_ptr<log_printer>>;

  std::string name;
  printers_type printers;
  bool active = true;

  logger() = default;
  logger(std::string name)
    : name(std::move(name))
  {}
  logger(std::string name, std::unique_ptr<log_printer> printer)
    : logger(std::move(name))
  {
    printers.emplace_back(std::move(printer));
  }
};

auto has_logger(const std::string& name) -> bool;
auto get_logger(const std::string& name) -> logger&;
auto add_logger(std::string name, std::unique_ptr<log_printer> printer) -> logger&;
auto add_printer_to_logger(const std::string& logger_name, std::unique_ptr<log_printer> printer) -> void;

auto turn_on(const std::string& logger_name);
auto turn_off(const std::string& logger_name);


auto log(const std::string& log_name, const std::string& msg) -> void;


auto add_stdout_printer(const std::string& logger_name) -> void;
auto add_file_printer(const std::string& logger_name, const std::string& file_name) -> void;

} // std_e
