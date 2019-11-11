#pragma once


#include "std_e/base/macros.hpp"
#include <exception>
#include <string>


namespace std_e {


class assertion_failure : public std::exception {
  public:
    assertion_failure(const std::string& file, int line, const std::string& assertion_test) noexcept
    {
      msg = "assertion failure at file " + file + ", line " + std::to_string(line) + "\n failed assertion: " + assertion_test;
    }

    const char* what() const noexcept override {
      return msg.c_str();
    }
  private:
    std::string msg;
};


class precondition_violation : public std::exception {
  public:
    precondition_violation() noexcept
      : msg(nullptr)
    {}

    precondition_violation(const char* msg) noexcept
      : msg(msg)
    {}

    const char* what() const noexcept override {
      return msg;
    }
  private:
    const char* msg;
};


} // std_e


#define STD_E_ASSERT(b) if (!(b)) throw ::std_e::assertion_failure(__FILE__,__LINE__,#b);
