#pragma once


#include "std_e/logging/printer.hpp"
#include <iostream>
#include <fstream>


namespace std_e {


class stdout_printer : public printer {
  public:
    ~stdout_printer() {
      std::cout << std::flush;
    }
    auto log(const std::string& msg) -> void override {
      std::cout << msg;
    }
};
class file_printer : public printer {
  public:
    file_printer() = default;
    file_printer(file_printer&&) = default;
    file_printer& operator=(file_printer&&) = default;

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
class recording_printer : public printer {
  public:
    recording_printer() = default;
    recording_printer(std::string* buffer_ptr)
      : buffer_ptr(buffer_ptr)
    {}

    auto log(const std::string& msg) -> void override {
      (*buffer_ptr) += msg;
    }
  private:
    std::string* buffer_ptr;
};


auto add_stdout_printer(const std::string& logger_name) -> void;
auto add_file_printer(const std::string& logger_name, const std::string& file_name) -> void;
auto add_recording_printer(const std::string& logger_name, std::string* s_ptr) -> void;


} // std_e
