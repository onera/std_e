#pragma once

#include <iostream>
#include <fstream>
#include <memory>
#include "std_e/utils/enum.hpp"


namespace std_e {


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

struct stdout_printer : log_printer {
  ~stdout_printer() {
    std::cout << std::flush;
  }
  auto log(const std::string& msg) -> void override {
    std::cout << msg;
  }
};
struct file_printer : log_printer {
  std::string file_name = "log.txt";
  std::ofstream file = std::ofstream(file_name.c_str(), std::fstream::out);
  ~file_printer() {
    file << std::flush;
  }
  auto log(const std::string& msg) -> void override {
    file << msg;
  }
};
// log_printer: base + common }

struct logger {
  std::string name;
  logging_level lvl;
  std::unique_ptr<log_printer> printer;
};

auto has_logger(const std::string& name) -> bool;
auto get_logger(const std::string& name) -> logger&;
auto add_logger(logger l) -> logger&;

// log functions {
auto log(logger& l, const std::string& msg) -> void;

/// log by matching logger's name
auto log(const std::string& log_name, const std::string& msg) -> void;

/// log by matching lvl
auto log(logging_level log_lvl, const std::string& msg) -> void;

/// same, with default values
auto log(const std::string& msg) -> void;
// log functions }


} // std_e
