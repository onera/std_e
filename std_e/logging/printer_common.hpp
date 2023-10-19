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
class stderr_printer : public printer {
  public:
    ~stderr_printer() {
      std::cerr << std::flush;
    }
    auto log(const std::string& msg) -> void override {
      std::cerr << msg;
    }
};

class file_printer : public printer {
  public:
    file_printer() = default;
    file_printer(file_printer&&) = default;
    file_printer& operator=(file_printer&&) = default;

    file_printer(const std::string& file_name)
      : _file_name(file_name)
      , _file(std::ofstream(file_name.c_str(), std::fstream::out))
    {}
    ~file_printer() {
      _file << std::flush;
    }
    auto file_name() const {
      return _file_name;
    }
    auto log(const std::string& msg) -> void override {
      _file << msg;
    }
  private:
    std::string _file_name;
    std::ofstream _file;
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


} // std_e
