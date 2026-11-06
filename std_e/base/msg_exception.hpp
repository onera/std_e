#pragma once


#include <exception>
#include <string>
#include <string_view>
#include "std_e/utils/string.hpp"


namespace std_e {


class msg_exception : public std::exception {
  public:
    msg_exception() = default;

    msg_exception(std::string_view msg)
      : msg(msg)
    {}
    msg_exception(auto&&... msg_parts)
      : msg(join(std::vector{std::string{msg_parts}...},""))
    {}

    const char* what() const noexcept override {
      return msg.c_str();
    }
  private:
    std::string msg;
};


} // std_e
